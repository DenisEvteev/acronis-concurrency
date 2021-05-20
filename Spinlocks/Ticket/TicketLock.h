#pragma once
#include <atomic>
#include <thread>

#include "../SpinWait.h"

namespace AcronisLabs {
#define NEW_THREAD 1
#define RELEASE 1

class TicketLock {
 public:
  void Lock() {
	auto ticket = ticket_.fetch_add(NEW_THREAD, std::memory_order_relaxed);
	SpinWait spinWait;
	while (owner_.load(std::memory_order_acquire) != ticket) {
	  spinWait();
	}
  }

  void Unlock() {
	owner_.fetch_add(RELEASE, std::memory_order_release);
  }

 private:
  std::atomic<std::size_t> owner_{0};
  std::atomic<std::size_t> ticket_{0};
};

}