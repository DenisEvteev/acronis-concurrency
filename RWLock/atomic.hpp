#pragma once
#include <atomic>
#include <climits>

#include "futex.h"

namespace extra
{
/*The extended version of atomic is written to support
 * wait method for efficient waiting on a futex word
 * */

	template<typename T>
	class atomic : public std::atomic<T>
	{
	 public:
		atomic() = default;
		explicit atomic(T arg) : std::atomic<T>(arg)
		{
		}

		void wait(T old, std::memory_order order = std::memory_order_seq_cst)
		{
			while (this->load(order) == old)
			{
				FutexWait(old);
			}
		}

		void notify_one()
		{
			FutexWakeOne();
		}

		void notify_all()
		{
			FutexWakeAll();
		}

	 private:
		uint32_t* FutexAddr();
		void FutexWait(T expected);
		void FutexWakeOne();
		void FutexWakeAll();

	};

	template<typename T>
	uint32_t* atomic<T>::FutexAddr()
	{
		return reinterpret_cast<uint32_t*>(this);
	}

	template<typename T>
	void atomic<T>::FutexWait(T old)
	{
		block::FutexWait(FutexAddr(), old);
	}

	template<typename T>
	void atomic<T>::FutexWakeOne()
	{
		block::FutexWake(FutexAddr(), 1);
	}

	template<typename T>
	void atomic<T>::FutexWakeAll()
	{
		block::FutexWake(FutexAddr(), INT_MAX);
	}

} //namespace extra


