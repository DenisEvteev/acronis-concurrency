#include "MCSLock.h"

namespace AcronisLabs
{

	MCSLock::Guard::Guard(MCSLock& spinlock) : spinlock_(spinlock)
	{
		auto prev = spinlock_.tail_.exchange(&per_cpu_, std::memory_order_acq_rel);
		if (prev != nullptr)
		{
			SpinWait spinWait;
			per_cpu_.locked_ = true;
			prev->next_.store(&per_cpu_, std::memory_order_release);
			while (per_cpu_.locked_.load(std::memory_order_acquire))
			{
				spinWait();
			}
		}
		//cs

	}
	MCSLock::Guard::~Guard()
	{
		SpinWait spinWait;
		if (per_cpu_.next_.load(std::memory_order_acquire) == nullptr)
		{
			MCSNode* thisThread = &per_cpu_;
			if (spinlock_.tail_.compare_exchange_strong(thisThread, nullptr))
				return;
		}

		while (per_cpu_.next_.load(std::memory_order_acquire) == nullptr)
		{
			spinWait();
		}
		per_cpu_.next_.load()->locked_.store(false, std::memory_order_release);
	}
}