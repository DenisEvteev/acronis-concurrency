#pragma once

#include <thread>
#include <algorithm>

namespace AcronisLabs
{
/* i) The choice of 100 spins for the default spin count
is a completely arbitrary choice that has not been evaluated thoroughly
using modern hardware.
   ii) The maximum number of times a thread should spin on the lock before
  relinquishing the CPU
  */
#define kMaxIterations 100
#define ReverseSmoothingFactor 8

	static inline void SpinLockPause()
	{
		asm volatile("pause\n" : : : "memory");
	}

//TODO: add some other options how to perform efficient spinning
	class SpinWait
	{
	 public:
		void operator()();
	 private:
		void ExponentialSmoothingSpin();
		int estimate{ 0 };

	};

}