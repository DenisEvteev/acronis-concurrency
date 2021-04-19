#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "TicketLock.h"

using namespace AcronisLabs;

TEST(Ticket, SimpleTest)
{
	TicketLock lock;
	lock.Lock();
	lock.Unlock();
}

TEST(Ticket, TwoLocks)
{
	TicketLock lock;

	lock.Lock();
	lock.Unlock();

	lock.Lock();
	lock.Unlock();

}

TEST(Ticket, ThreadsIncreaseCounters)
{
	const size_t kIterations = 1000;
	TicketLock ticketLock;
	size_t shared_counter = 0;

	auto test = [&]()
	{
	  for (size_t i = 0; i < kIterations; ++i)
	  {
		  ticketLock.Lock();
		  size_t current = shared_counter;
		  std::this_thread::sleep_for(
			  std::chrono::milliseconds(10));
		  shared_counter = current + 1;
		  ticketLock.Unlock();
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