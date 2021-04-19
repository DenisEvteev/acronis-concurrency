#include "SpinWait.h"

namespace AcronisLabs
{

	void SpinWait::operator()()
	{
		ExponentialSmoothingSpin();
	}

	void SpinWait::ExponentialSmoothingSpin()
	{
		int CurNIter = std::min(kMaxIterations, estimate * 2 + 10);
		int SpinIp{ 0 };
		/*Strange implementation of spinning ;( */
		while (true)
		{
			if (++SpinIp == CurNIter)
			{
				std::this_thread::yield();
				break;
			}
			SpinLockPause();
		}
		estimate += (CurNIter - estimate) / ReverseSmoothingFactor;
	}
}