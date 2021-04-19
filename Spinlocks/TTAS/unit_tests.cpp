#include <gtest/gtest.h>

#include "../unit_test.hpp"
#include "TTASSpinlock.hpp"

using namespace AcronisLabs;

TEST(TTAS, JustWork)
{
	//come up with some implementation of time limit of the execution of a test
	TesterSpinlock<TTASSpinlock> spinlock;
	spinlock.JustWork();
}

TEST(TTAS, TwoLocks)
{
	//same
	TesterSpinlock<TTASSpinlock> spinlock;
	spinlock.TwoLocks();
}

TEST(TTAS, BigNumberOfThreads)
{
	TesterSpinlock<TTASSpinlock> spinlock;
	auto res = spinlock.ThreadsIncreaseCounters(1000, 10);
	ASSERT_EQ(res, 1000 * 10);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}