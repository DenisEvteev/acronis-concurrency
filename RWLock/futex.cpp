#include "futex.h"

namespace block
{
	int futex(unsigned int* uaddr, int futex_op, int val, const struct timespec* timeout,
		int* uaddr2, int val3)
	{
		return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
	}

	int FutexWait(uint32_t* addr, uint32_t expected)
	{
		return futex(addr, FUTEX_WAIT_PRIVATE, expected, nullptr, nullptr, 0);
	}

	int FutexWake(uint32_t* addr, int count)
	{
		return futex(addr, FUTEX_WAKE_PRIVATE, count, nullptr, nullptr, 0);
	}

}//namespace block
