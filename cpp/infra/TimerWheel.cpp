#include "TimerWheel.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/time.h>
#endif //_LINUX_

namespace
{
	uint32_t GetJiffies(void)
	{
#ifdef WIN32
		return GetTickCount64();
#else
		struct timespec ts;  // ��ȷ�����루10 �� -9 �η��룩
		// ʹ�� clock_gettime ����ʱ����Щϵͳ������ rt �⣬�� -lrt ��������Щ����Ҫ���� rt ��
		clock_gettime(CLOCK_MONOTONIC, &ts);  // ��ȡʱ�䡣���У�CLOCK_MONOTONIC ��ʾ��ϵͳ������һ����ʼ��ʱ,����ϵͳʱ�䱻�û��ı��Ӱ��
		return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);  // ���غ���ʱ��
#endif
	}

	// ��˫��ѭ��������½����뵽��� pPrev �� pNext ֮��
	void ListTimerInsert(struct LIST_TIMER *pNew, struct LIST_TIMER *pPrev, struct LIST_TIMER *pNext)
	{
		pNext->pPrev = pNew;
		pNew->pNext = pNext;
		pNew->pPrev = pPrev;
		pPrev->pNext = pNew;
	}

	void ListTimerInsertHead(struct LIST_TIMER *pNew, struct LIST_TIMER *pHead)
	{
		ListTimerInsert(pNew, pHead, pHead->pNext);
	}

	void ListTimerInsertTail(struct LIST_TIMER *pNew, struct LIST_TIMER *pHead)
	{
		ListTimerInsert(pNew, pHead->pPrev, pHead);
	}

	// ʹ���½�� pNew �滻 pOld ��˫��ѭ�������е�λ�á����˫�������н���һ����� pOld��ʹ�� pNew �滻��ͬ��������һ����� pNew
	void ListTimerReplace(struct LIST_TIMER *pOld, struct LIST_TIMER *pNew)
	{
		pNew->pNext = pOld->pNext;
		pNew->pNext->pPrev = pNew;
		pNew->pPrev = pOld->pPrev;
		pNew->pPrev->pNext = pNew;
	}

	// ʹ���½�� pNew �滻 pOld ��˫��ѭ�������е�λ�á�
	void ListTimerReplaceInit(struct LIST_TIMER *pOld, struct LIST_TIMER *pNew)
	{
		ListTimerReplace(pOld, pNew);
		// ʹ�� pNew �滻 pOld ��˫��ѭ�������е�λ�ú�pOld ���������ж��������ˣ�����Ҫ�� pOld ָ���Լ�
		pOld->pNext = pOld;
		pOld->pPrev = pOld;
	}

	// ��ʼ��ʱ�����е����� tick����ʼ����ÿ�� tick �е�˫������ֻ��һ��ͷ���
	void InitArrayListTimer(struct LIST_TIMER *arrListTimer, uint32_t nSize)
	{
		uint32_t i;
		for (i = 0; i<nSize; i++)
		{
			arrListTimer[i].pPrev = &arrListTimer[i];
			arrListTimer[i].pNext = &arrListTimer[i];
		}
	}

	void DeleteArrayListTimer(struct LIST_TIMER *arrListTimer, uint32_t uSize)
	{
		struct LIST_TIMER listTmr, *pListTimer;
		struct TIMER_NODE *pTmr;
		uint32_t idx;

		for (idx = 0; idx<uSize; idx++)
		{
			ListTimerReplaceInit(&arrListTimer[idx], &listTmr);
			pListTimer = listTmr.pNext;
			while (pListTimer != &listTmr)
			{
				//pTmr = (struct TIMER_NODE *)((uint8 *)pListTimer - offsetof(struct TIMER_NODE, ltTimer)); //reinterpret_cast
				pTmr = (struct TIMER_NODE *)(pListTimer);
				pListTimer = pListTimer->pNext;
				free(pTmr);
			}
		}
	}

	// ˯ uMs ����
	void sleepMilliseconds(uint32_t uMs)
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = uMs * 1000;  // tv.tv_usec ��λ��΢��
		::select(0, NULL, NULL, NULL, &tv);
	}

}

TimerWheel::TimerWheel() :
mMutex(), mThread(std::bind(&TimerWheel::run, this), "timerTask")
{
	uJiffies = GetJiffies();
	uExitFlag = false;
	InitArrayListTimer(arrListTimer1, sizeof(arrListTimer1) / sizeof(arrListTimer1[0]));
	InitArrayListTimer(arrListTimer2, sizeof(arrListTimer2) / sizeof(arrListTimer2[0]));
	InitArrayListTimer(arrListTimer3, sizeof(arrListTimer3) / sizeof(arrListTimer3[0]));
	InitArrayListTimer(arrListTimer4, sizeof(arrListTimer4) / sizeof(arrListTimer4[0]));
	InitArrayListTimer(arrListTimer5, sizeof(arrListTimer5) / sizeof(arrListTimer5[0]));
	mThread.start();
}

TimerWheel::~TimerWheel()
{
	uExitFlag = true;
	DeleteArrayListTimer(arrListTimer1, sizeof(arrListTimer1) / sizeof(arrListTimer1[0]));
	DeleteArrayListTimer(arrListTimer2, sizeof(arrListTimer2) / sizeof(arrListTimer2[0]));
	DeleteArrayListTimer(arrListTimer3, sizeof(arrListTimer3) / sizeof(arrListTimer3[0]));
	DeleteArrayListTimer(arrListTimer4, sizeof(arrListTimer4) / sizeof(arrListTimer4[0]));
	DeleteArrayListTimer(arrListTimer5, sizeof(arrListTimer5) / sizeof(arrListTimer5[0]));
}

// �ص�������ִ���̺߳�TimerWheel::runTimer��ͬһ���߳�;
// ����ڻص��������������ö�ʱ���򲻼���

class TimerListAutoLock
{
public:
	TimerListAutoLock(const Thread& thread, std::mutex& mutex) :
		mThread(thread), mMutex(mutex)
	{
		if (mThread.getId() != std::this_thread::get_id())
		{
			mutex.lock();
		}
	}
	~TimerListAutoLock()
	{
		if (mThread.getId() != std::this_thread::get_id())
		{
			mMutex.unlock();
		}
	}
private:
	std::mutex& mMutex;
	const Thread& mThread;
};

void TimerWheel::addTimer(uint32_t uDueTime, uint32_t uPeriod, const TimerCallback& cb)
{
	if (NULL == cb)
		return;
	LPTIMERNODE pTmr = new TIMERNODE;
	if (pTmr == NULL)
		return;

	pTmr->uPeriod = uPeriod;
	pTmr->cb = cb;
	{
		TimerListAutoLock lock(mThread, mMutex);
		pTmr->uExpires = uJiffies + uDueTime;
		addTimer(pTmr);
	}
}

void TimerWheel::addTimer(LPTIMERNODE pTmr)
{
	struct LIST_TIMER *pHead;
	uint32_t i, uDueTime, uExpires;

	uExpires = pTmr->uExpires; // ��ʱ�����ڵ�ʱ��
	uDueTime = uExpires - uJiffies;
	if (uDueTime < TVR_SIZE)   // idx < 256 (2��8�η�)
	{
		i = uExpires & TVR_MASK; // expires & 255
		pHead = &arrListTimer1[i];
	}
	else if (uDueTime < 1 << (TVR_BITS + TVN_BITS)) // idx < 16384 (2��14�η�)
	{
		i = (uExpires >> TVR_BITS) & TVN_MASK;      // i = (expires>>8) & 63
		pHead = &arrListTimer2[i];
	}
	else if (uDueTime < 1 << (TVR_BITS + 2 * TVN_BITS)) // idx < 1048576 (2��20�η�)
	{
		i = (uExpires >> (TVR_BITS + TVN_BITS)) & TVN_MASK; // i = (expires>>14) & 63
		pHead = &arrListTimer3[i];
	}
	else if (uDueTime < 1 << (TVR_BITS + 3 * TVN_BITS)) // idx < 67108864 (2��26�η�)
	{
		i = (uExpires >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK; // i = (expires>>20) & 63
		pHead = &arrListTimer4[i];
	}
	else if ((signed long)uDueTime < 0)
	{
		/*
		* Can happen if you add a timer with expires == jiffies,
		* or you set a timer to go off in the past
		*/
		pHead = &arrListTimer1[(uJiffies & TVR_MASK)];
	}
	else
	{
		/* If the timeout is larger than 0xffffffff on 64-bit
		* architectures then we use the maximum timeout:
		*/
		if (uDueTime > 0xffffffffUL)
		{
			uDueTime = 0xffffffffUL;
			uExpires = uDueTime + uJiffies;
		}
		i = (uExpires >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK; // i = (expires>>26) & 63
		pHead = &arrListTimer5[i];
	}
	ListTimerInsertTail(&pTmr->ltTimer, pHead);
}

void TimerWheel::run()
{
	while (!uExitFlag)
	{
		runTimer();
		sleepMilliseconds(1);  // �߳�˯ 1 ����
	}
}

void TimerWheel::runTimer()
{
#define INDEX(N) ((uJiffies >> (TVR_BITS + (N) * TVN_BITS)) & TVN_MASK)

	uint32_t idx, uJiffies;
	struct LIST_TIMER listTmrExpire, *pListTmrExpire;
	struct TIMER_NODE *pTmr;
	uJiffies = GetJiffies();
	std::lock_guard<std::mutex> lock(mMutex);
	while (TIME_AFTER_EQ(uJiffies, this->uJiffies))
	{
		// unint32 �� 32bit��idx Ϊ��ǰʱ��ĵ� 8bit��INDEX(0) Ϊ�� 6bit��INDEX(1) Ϊ�ٴ� 6bit��INDEX(2) Ϊ�ٴ� 6bit��INDEX(3) Ϊ�� 6bit
		// ��� 1 ��ʱ���ֵ� 256 ���������ˣ�������� 2 ��ʱ�����еļ�ʱ���������еļ�ʱ�����ݵ���ʱ����뵽ָ��ʱ���֡����� 1 ��ʱ�����о��м�ʱ���ˡ�
		// ��� 1��2 ��ʱ���ֵ� 256*64 ���붼�����ˣ������ 3 ��ʱ���֣������еļ�ʱ�����뵽ָ��ʱ���֡����� 1 ���� 2 ��ʱ�����о��м�ʱ���ˡ�
		// ��� 1��2��3 ��ʱ���ֵ� 256*64*64 ���붼�����ˣ�...
		// ��� 1��2��3��4 ��ʱ���ֵ� 256*64*64*64 ���붼�����ˣ�...
		idx = this->uJiffies & TVR_MASK;
		if (!idx && (!cascadeTimer(this->arrListTimer2, INDEX(0))) &&
			(!cascadeTimer(this->arrListTimer3, INDEX(1))) &&
			(!cascadeTimer(this->arrListTimer4, INDEX(2))))
		{
			cascadeTimer(this->arrListTimer5, INDEX(3));
		}
		pListTmrExpire = &listTmrExpire;
		// �½�� pListTmrExpire �滻 arrListTimer1[idx] ��˫��ѭ������ arrListTimer1[idx] ��ֻ�����Լ�һ������ˡ�pListTmrExpire ��Ϊ˫��ѭ����������
		ListTimerReplaceInit(&arrListTimer1[idx], pListTmrExpire);
		// ����ʱ���� arrListTimer1 ��˫��ѭ������ִ�и��������ж�ʱ���Ļص�����
		pListTmrExpire = pListTmrExpire->pNext;
		while (pListTmrExpire != &listTmrExpire)
		{
			//pTmr = (struct TIMER_NODE *)((uint8 *)pListTmrExpire - offsetof(struct TIMER_NODE, ltTimer));
			pTmr = (struct TIMER_NODE *)(pListTmrExpire);
			pListTmrExpire = pListTmrExpire->pNext;
			pTmr->cb();
			if (pTmr->uPeriod != 0)
			{
				pTmr->uExpires = this->uJiffies + pTmr->uPeriod;
				addTimer(pTmr);
			}
			else
			{
				delete pTmr;
			}
		}
		this->uJiffies++;
	}
}

// ����ʱ���� arrlistTimer ��˫��ѭ�����������еļ�ʱ�����ݵ���ʱ����뵽ָ����ʱ������
uint32_t TimerWheel::cascadeTimer(struct LIST_TIMER *arrListTimer, uint32_t idx)
{
	struct LIST_TIMER listTmr, *pListTimer;
	struct TIMER_NODE *pTmr;

	ListTimerReplaceInit(&arrListTimer[idx], &listTmr);
	pListTimer = listTmr.pNext;
	// ����˫��ѭ��������Ӷ�ʱ��
	while (pListTimer != &listTmr)
	{
		// ���ݽṹ�� struct TIMER_NODE �ĳ�Ա ltTimer ��ָ���ַ�͸ó�Ա�ڽṹ���еı�����������ṹ�� struct TIMER_NODE �ĵ�ַ
		// pTmr = (struct TIMER_NODE *)((uint8 *)pListTimer - offsetof(struct TIMER_NODE, ltTimer));
		pTmr = (struct TIMER_NODE *)(pListTimer);
		pListTimer = pListTimer->pNext;
		addTimer(pTmr);
	}
	return idx;
}

void TimerWheel::setLoopTimer(uint32_t timerLen, const TimerCallback& cb)
{
	addTimer(timerLen, timerLen, cb);
}

void TimerWheel::setRelativeTimer(uint32_t timerLen, const TimerCallback& cb)
{
	addTimer(timerLen, 0, cb);
}
