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
			prev->next_ = &per_cpu_;
			while (per_cpu_.locked_)
			{
				spinWait();
			}
		}
		//cs

	}
	MCSLock::Guard::~Guard()
	{
		SpinWait spinWait;
		if (per_cpu_.next_ == nullptr)
		{
			MCSNode* thisThread = &per_cpu_;
			if (spinlock_.tail_.compare_exchange_strong(thisThread, nullptr,))
				return;
		}

		while (per_cpu_.next_ == nullptr)
		{
			spinWait();
		}
		per_cpu_.next_->locked_ = false;

	}
}