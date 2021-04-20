#pragma once
#include <atomic>

#include "../SpinWait.h"

namespace AcronisLabs
{
	class MCSLock
	{
		struct MCSNode
		{
			std::atomic<MCSNode*> next_{ nullptr };
			std::atomic<bool> locked_{ false };
		};
	 public:
		class Guard
		{
		 public:
			explicit Guard(MCSLock& spinlock);
			~Guard();
			Guard(const Guard&) = delete;
			Guard& operator=(const Guard&) = delete;

		 private:
			MCSLock& spinlock_;
			MCSNode per_cpu_;

		};

	 private:
		friend class Guard;
		std::atomic<MCSNode*> tail_{ nullptr };
	};
}
