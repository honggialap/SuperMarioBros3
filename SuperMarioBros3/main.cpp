#include "Engine/Framework/Application.h"
#include "Engine/Framework/Time.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	pApplication app = new CApplication();
	pTime time = new CTime();
	if (app->Create(hInstance, L"Time class test", L"Data/brick.ico", 1920, 1080, false))
	{
		time->Start();
		float exitDuration = 0;
		float exitTime = 5000;

		while (!app->HandleMessage())
		{
			time->Tick();
			exitDuration += time->_elapsedMs;

			if (exitDuration > exitTime)
			{
				app->Exit();
			}
		}
	}
	delete time;
	time = nullptr;

	delete app;
	app = nullptr;

	return 0;
}