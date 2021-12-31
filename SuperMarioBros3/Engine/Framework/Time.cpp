#include "Time.h"
#include "Debug.h"


void CTime::Start()
{
	_elapsedMs = 0;
	_totalElapsedMs = 0;
	_startTimeStamp = _clock.now();
	_currentTimeStamp = _startTimeStamp;

	DebugOut(L"[Time] Start clock.\n");
}


void CTime::Tick()
{
	_previousTimeStamp = _currentTimeStamp;
	_currentTimeStamp = _clock.now();

	_elapsedMs = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>
		(_currentTimeStamp - _previousTimeStamp).count();

	_totalElapsedMs = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>
		(_currentTimeStamp - _startTimeStamp).count();
}
