#include "TTASSpinlock.hpp"

namespace AcronisLabs {

void TTASSpinlock::Lock() {
  SpinWait spin_wait;
  while (lock_.exchange(true, std::memory_order_acquire)) {
	do {
	  spin_wait();
	} while (lock_.load(std::memory_order_acquire));
  }

}

void TTASSpinlock::Unlock() {
  lock_.store(false, std::memory_order_release);
}

} //namespace AcronisLabs
