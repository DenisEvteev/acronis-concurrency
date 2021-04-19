#pragma once
#include <iostream>

#include "atomic.hpp"

namespace AcronisLabs
{

	class RWLock
	{
	 public:
		void Lock();
		void Unlock();

		void LockShared();
		void UnlockShared();

	 private:
		extra::atomic<bool> Reader_{ false };
		extra::atomic<bool> Exclusive_{ false };
		extra::atomic<size_t> ReaderCounter_{ 0 };

	};

}

