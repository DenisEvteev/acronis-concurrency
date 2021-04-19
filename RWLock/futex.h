#pragma once

#define _GNU_SOURCE
#include <unistd.h>
#include <cstdint>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/time.h>

namespace block
{

/*There is no glibc wrapper for this system call*/
	int futex(int* uaddr, int futex_op, int val, const struct timespec* timeout,
		int* uaddr2, int val3);
	int FutexWait(uint32_t* addr, uint32_t expected);
	int FutexWake(uint32_t* addr, int count);

} //namespace block