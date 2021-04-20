#include "SpinWait.h"

namespace AcronisLabs
{

	SpinWait::SpinWait()
	{
		CurNIter = std::min(kMaxIterations, estimate * 2 + 10);
	}

	void SpinWait::operator()()
	{
		ExponentialSmoothingSpin();
	}

	void SpinWait::ExponentialSmoothingSpin()
	{
		if (SpinIp == CurNIter)
		{
			std::this_thread::yield();
			estimate += (CurNIter - estimate) / ReverseSmoothingFactor;
			CurNIter = std::min(kMaxIterations, estimate * 2 + 10);
			SpinIp = 0;
			return;
		}
		SpinLockPause();
		++SpinIp;
	}
}