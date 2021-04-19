#pragma once

#include <atomic>
#include <thread>
#include <algorithm>

#include "../SpinWait.h"

/*List of optimizations:
 *
 * i: correct memory_order
 * ii: lack of ping-pong messages in cache coherency protocol
 * iii: pause instruction
 * iiii: exponential smoothing loop precision
 * */

namespace AcronisLabs
{

	class TTASSpinlock
	{
	 public:
		void Lock();
		void Unlock();
	 private:
		std::atomic<bool> lock_{ false };
	};

} // namespace AcronisLabs