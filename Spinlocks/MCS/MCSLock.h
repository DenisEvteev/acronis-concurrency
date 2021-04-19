#pragma once
#include <atomic>

#include "../SpinWait.h"

namespace AcronisLabs
{
	class MCSLock
	{
		struct MCSNode
		{
			MCSNode* next_{ nullptr };
			bool locked_{ false };
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
