#include "RWLock.h"

namespace AcronisLabs
{
//ticket lock -- memory_order_relaxed for
	void RWLock::LockShared()
	{
		uint32_t ReadType{ 0 };
		if ((ReadType = Reader_.exchange(true, std::memory_order_acquire)))
		{
			Reader_.wait(true, std::memory_order_acquire);
		}
		if (ReaderCounter_.fetch_add(1, std::memory_order_relaxed) == 0)
		{
			while (Exclusive_.exchange(true, std::memory_order_acquire))
			{
				Exclusive_.wait(true);
			}
		}
		if (ReadType == 0)
		{
			Reader_.store(false, std::memory_order_release);
			Reader_.notify_all();
		}
		//cs
	}

	void RWLock::UnlockShared()
	{
		Reader_.store(true);
		if (ReaderCounter_.fetch_sub(1) == 1)
		{
			Exclusive_.store(false, std::memory_order_release);
			Exclusive_.notify_one();
		}
		Reader_.store(false);
		Reader_.notify_one();
	}

	void RWLock::Lock()
	{
		while (Exclusive_.exchange(true))
		{
			Exclusive_.wait(true);
		}
	}

	void RWLock::Unlock()
	{
		Exclusive_.store(false, std::memory_order_release);
		Exclusive_.notify_one();
	}

}// namespace AcronisLabs