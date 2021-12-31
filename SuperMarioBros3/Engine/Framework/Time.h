#pragma	once
#include <chrono>


struct CTime
{
	std::chrono::steady_clock _clock;
	std::chrono::steady_clock::time_point _startTimeStamp;
	std::chrono::steady_clock::time_point _previousTimeStamp;
	std::chrono::steady_clock::time_point _currentTimeStamp;
	float _elapsedMs = 0;
	float _totalElapsedMs = 0;

	void Start();
	void Tick();
};
typedef CTime* pTime;