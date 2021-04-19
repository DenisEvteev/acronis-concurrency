#pragma once

#include <vector>
#include <thread>
#include <functional>

namespace AcronisLabs
{

	template<class LockType>
	class TesterSpinlock
	{
	 public:
		class Timer
		{
		 public:
			Timer();
			~Timer();
			void estimate();
		};
	 public:
		void JustWork();
		void TwoLocks();
		size_t ThreadsIncreaseCounters(size_t kIter, size_t kThreads);
		void MaxTimeLockUnlock(size_t kThreads, size_t kIter);
	 private:
		void CreateThreads(size_t kThreads, const std::function<void()>& callBack);
	 private:
		LockType lock_; // it should have Lock() and Unlock()
	};

	template<class LockType>
	void TesterSpinlock<LockType>::MaxTimeLockUnlock(const size_t kThreads, const size_t kIter)
	{
		if (kThreads == 0)
			throw std::runtime_error("divison on zero is prohibited!");

		size_t per_thread = kIter / kThreads;
		auto simple = [per_thread, this]()
		{
		  for (size_t i = 0; i < per_thread; ++i)
		  {
			  TesterSpinlock::Timer clock;
			  lock_.Lock();
			  clock.estimate();
			  lock_.Unlock();
			  //we gonna write the results to some data structure by calling the destructor
		  }
		};
		CreateThreads(kThreads, simple);
	}

	template<class LockType>
	void TesterSpinlock<LockType>::JustWork()
	{
		lock_.Lock();
		lock_.Unlock();
	}

	template<class LockType>
	void TesterSpinlock<LockType>::TwoLocks()
	{
		lock_.Lock();
		lock_.Unlock();

		lock_.Lock();
		lock_.Unlock();
	}

	template<class LockType>
	size_t TesterSpinlock<LockType>
	::ThreadsIncreaseCounters(const size_t kIter, const size_t kThreads)
	{
		size_t shared_counter = 0;
		auto test = [&]()
		{
		  for (size_t i = 0; i < kIter; ++i)
		  {
			  lock_.Lock();
			  size_t current = shared_counter;
			  std::this_thread::sleep_for(
				  std::chrono::milliseconds(10));
			  shared_counter = current + 1;
			  lock_.Unlock();
		  }
		};

		CreateThreads(kThreads, test);
		return shared_counter;
	}

	template<class LockType>
	void TesterSpinlock<LockType>::CreateThreads(const size_t kThreads,
		const std::function<void()>& callBack)
	{
		std::vector<std::thread> pool;
		for (int i = 0; i < kThreads; ++i)
		{
			pool.emplace_back(callBack);
		}

		for (auto& el: pool)
			el.join();

	}
}