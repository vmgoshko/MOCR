#include <crtdefs.h>
#include <time.h>
#include <chrono>

class Timer
{
public:
	void Start()
	{
		mStartTime = std::chrono::steady_clock::now();
	}

	double Stop()
	{
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - mStartTime);
		return elapsed.count();
	}

private:
	std::chrono::steady_clock::time_point mStartTime;
};