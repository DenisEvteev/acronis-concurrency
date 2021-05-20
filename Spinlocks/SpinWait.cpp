#include "SpinWait.h"

namespace AcronisLabs {

SpinWait::SpinWait() {
  curNIter = std::min((size_t)kMaxIterations, estimate * 2 + 10);
}

void SpinWait::operator()() {
  ExponentialSmoothingSpin();
}

void SpinWait::ExponentialSmoothingSpin() {
  if (kIters == curNIter) {
	std::this_thread::yield();
	estimate += (curNIter - estimate) / ReverseSmoothingFactor;
	curNIter = std::min((size_t)kMaxIterations, estimate * 2 + 10);
	kIters = 0;
	return;
  }
  SpinLockPause();
  ++kIters;
}
}