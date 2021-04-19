#include "TTASSpinlock.hpp"

namespace AcronisLabs
{

	void TTASSpinlock::Lock()
	{
		SpinWait spinWait;
		while (lock_.exchange(true, std::memory_order_acquire))
		{
			do
			{
				spinWait();
			} while (lock_.load(std::memory_order_acquire));
		}

	}
	void TTASSpinlock::Unlock()
	{
		lock_.store(false, std::memory_order_release);
	}

} //namespace AcronisLabs
