#include <iostream>
#include <thread>
#include <stdlib.h> 
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "Thread.h"
#include "ThreadPool.h"
#include "TimerWheel.h"
#include <functional>
using namespace std;
using std::function;

class Daniel
{
public:
	void start()
	{
		timer.setRelativeTimer(1000, std::bind(&Daniel::print, this));
	}

	void print()
	{
		cout << "Hello Daniel===" << endl;
		timer.setRelativeTimer(1000, std::bind(&Daniel::print, this));
	}
private:
	TimerWheel timer;
};



//int main()
//{
//	std::mutex mu;
//	std::lock_guard<std::mutex> lock1(mu);
//	std::lock_guard<std::mutex> lock2(mu);
//	while (1);
//	return 0;
//}

int main()
{

	Daniel daniel;
	daniel.start();
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	return 0;
}
