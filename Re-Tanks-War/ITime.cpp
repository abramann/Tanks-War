#include "ITime.h"
#include <chrono>
#include <thread>
#include <Windows.h>

void ITime::sleep(int64_t millsec) const
{
	int64_t start = getTime();
	int64_t now = 0;
	int64_t delta = 0;

	do
	{
		timeBeginPeriod(1);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		timeEndPeriod(1);

		now = getTime();

		delta = now - start;
	} while (delta < millsec);
}