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
		struct timespec ts;  // 精确到纳秒（10 的 -9 次方秒）
		// 使用 clock_gettime 函数时，有些系统需连接 rt 库，加 -lrt 参数，有些不需要连接 rt 库
		clock_gettime(CLOCK_MONOTONIC, &ts);  // 获取时间。其中，CLOCK_MONOTONIC 表示从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
		return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);  // 返回毫秒时间
#endif
	}

	// 将双向循环链表的新结点插入到结点 pPrev 和 pNext 之间
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

	// 使用新结点 pNew 替换 pOld 在双向循环链表中的位置。如果双向链表中仅有一个结点 pOld，使用 pNew 替换后，同样，仅有一个结点 pNew
	void ListTimerReplace(struct LIST_TIMER *pOld, struct LIST_TIMER *pNew)
	{
		pNew->pNext = pOld->pNext;
		pNew->pNext->pPrev = pNew;
		pNew->pPrev = pOld->pPrev;
		pNew->pPrev->pNext = pNew;
	}

	// 使用新结点 pNew 替换 pOld 在双向循环链表中的位置。
	void ListTimerReplaceInit(struct LIST_TIMER *pOld, struct LIST_TIMER *pNew)
	{
		ListTimerReplace(pOld, pNew);
		// 使用 pNew 替换 pOld 在双向循环链表中的位置后，pOld 结点从链表中独立出来了，所以要让 pOld 指向自己
		pOld->pNext = pOld;
		pOld->pPrev = pOld;
	}

	// 初始化时间轮中的所有 tick。初始化后，每个 tick 中的双向链表只有一个头结点
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

	// 睡 uMs 毫秒
	void sleepMilliseconds(uint32_t uMs)
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = uMs * 1000;  // tv.tv_usec 单位是微秒
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

// 回调函数的执行线程和TimerWheel::runTimer在同一个线程;
// 如果在回调函数里面再设置定时器则不加锁

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

	uExpires = pTmr->uExpires; // 定时器到期的时刻
	uDueTime = uExpires - uJiffies;
	if (uDueTime < TVR_SIZE)   // idx < 256 (2的8次方)
	{
		i = uExpires & TVR_MASK; // expires & 255
		pHead = &arrListTimer1[i];
	}
	else if (uDueTime < 1 << (TVR_BITS + TVN_BITS)) // idx < 16384 (2的14次方)
	{
		i = (uExpires >> TVR_BITS) & TVN_MASK;      // i = (expires>>8) & 63
		pHead = &arrListTimer2[i];
	}
	else if (uDueTime < 1 << (TVR_BITS + 2 * TVN_BITS)) // idx < 1048576 (2的20次方)
	{
		i = (uExpires >> (TVR_BITS + TVN_BITS)) & TVN_MASK; // i = (expires>>14) & 63
		pHead = &arrListTimer3[i];
	}
	else if (uDueTime < 1 << (TVR_BITS + 3 * TVN_BITS)) // idx < 67108864 (2的26次方)
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
		sleepMilliseconds(1);  // 线程睡 1 毫秒
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
		// unint32 共 32bit，idx 为当前时间的低 8bit，INDEX(0) 为次 6bit，INDEX(1) 为再次 6bit，INDEX(2) 为再次 6bit，INDEX(3) 为高 6bit
		// 如果 1 级时间轮的 256 毫秒走完了，会遍历把 2 级时间轮中的计时器，将其中的计时器根据到期时间加入到指定时间轮。这样 1 级时间轮中就有计时器了。
		// 如果 1、2 级时间轮的 256*64 毫秒都走完了，会遍历 3 级时间轮，将其中的计时器加入到指定时间轮。这样 1 级和 2 级时间轮中就有计时器了。
		// 如果 1、2、3 级时间轮的 256*64*64 毫秒都走完了，...
		// 如果 1、2、3、4 级时间轮的 256*64*64*64 毫秒都走完了，...
		idx = this->uJiffies & TVR_MASK;
		if (!idx && (!cascadeTimer(this->arrListTimer2, INDEX(0))) &&
			(!cascadeTimer(this->arrListTimer3, INDEX(1))) &&
			(!cascadeTimer(this->arrListTimer4, INDEX(2))))
		{
			cascadeTimer(this->arrListTimer5, INDEX(3));
		}
		pListTmrExpire = &listTmrExpire;
		// 新结点 pListTmrExpire 替换 arrListTimer1[idx] 后，双向循环链表 arrListTimer1[idx] 就只有它自己一个结点了。pListTmrExpire 成为双向循环链表的入口
		ListTimerReplaceInit(&arrListTimer1[idx], pListTmrExpire);
		// 遍历时间轮 arrListTimer1 的双向循环链表，执行该链表所有定时器的回调函数
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

// 遍历时间轮 arrlistTimer 的双向循环链表，将其中的计时器根据到期时间加入到指定的时间轮中
uint32_t TimerWheel::cascadeTimer(struct LIST_TIMER *arrListTimer, uint32_t idx)
{
	struct LIST_TIMER listTmr, *pListTimer;
	struct TIMER_NODE *pTmr;

	ListTimerReplaceInit(&arrListTimer[idx], &listTmr);
	pListTimer = listTmr.pNext;
	// 遍历双向循环链表，添加定时器
	while (pListTimer != &listTmr)
	{
		// 根据结构体 struct TIMER_NODE 的成员 ltTimer 的指针地址和该成员在结构体中的便宜量，计算结构体 struct TIMER_NODE 的地址
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
