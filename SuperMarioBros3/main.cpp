#include "Engine/Framework/Application.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	pApplication app = new CApplication();
	if (app->Create(hInstance, L"Application class test", L"Data/brick.ico", 1920, 1080, false))
	{
		int counter = 0;	
		while (!app->HandleMessage())
		{
			if (counter < 100000)
			{
				counter++;
			}
			else
			{
				//app->Exit();
			}
		}
	}
	delete app;
	app = nullptr;

	return 0;
}