#include <gtest/gtest.h>
#include <chrono>
#include <thread>

#include "MCSLock.h"

using namespace AcronisLabs;

TEST(MCS, SimpleTest)
{
	MCSLock spinlock;
	{
		auto lock = MCSLock::Guard(spinlock); //copy elision
		//cs
	}
}

TEST(MCS, TwoLocks)
{
	MCSLock spinlock;
	{
		MCSLock::Guard lock(spinlock);
	}
	{
		MCSLock::Guard lock(spinlock);
	}
}

TEST(MCS, ThreadsIncreaseCounters)
{
	const size_t kIterations = 1000;
	MCSLock spinlock;
	size_t shared_counter = 0;

	auto test = [&]()
	{
	  for (size_t i = 0; i < kIterations; ++i)
	  {
		  MCSLock::Guard lock(spinlock);
		  size_t current = shared_counter;
		  std::this_thread::sleep_for(
			  std::chrono::milliseconds(10));
		  shared_counter = current + 1;
	  }
	};

	const size_t kThreads = 5;
	std::vector<std::thread> pool;
	for (int i = 0; i < kThreads; ++i)
	{
		pool.emplace_back(test);
	}

	for (auto& el: pool)
		el.join();
	ASSERT_EQ(shared_counter, kIterations * kThreads);

}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
