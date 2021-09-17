#pragma	once
#include <chrono>

/// <summary>
/// Reponsible for timing with high-resolution clock.
/// </summary>
struct CTime
{
	std::chrono::steady_clock _clock;
	std::chrono::steady_clock::time_point _startTimeStamp;
	std::chrono::steady_clock::time_point _previousTimeStamp;
	std::chrono::steady_clock::time_point _currentTimeStamp;
	float _elapsedMs;
	float _totalElapsedMs;

	void Start();
	void Tick();
};
typedef CTime* pTime;