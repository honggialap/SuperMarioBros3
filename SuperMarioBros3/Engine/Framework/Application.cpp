#include "Application.h"
#include "Debug.h"


bool CApplication::Create(HINSTANCE hInstance, std::wstring title, unsigned int width, unsigned int height)
{
	_hInstance = hInstance;

	WNDCLASSEX wcex;
	wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = (WNDPROC)WinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"GameWindow";
	wcex.hIconSm = NULL;
	RegisterClassEx(&wcex);

	_hWnd = CreateWindowEx(
		0,
		L"GameWindow",
		title.c_str(),
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL, NULL,
		hInstance,
		NULL
	);

	if (!_hWnd)
	{
		DebugOut(L"[Application] Create game window failed.\n");
		return false;
	}
	else
	{
		ShowWindow(_hWnd, SW_SHOWNORMAL);
		UpdateWindow(_hWnd);

		DebugOut(L"[Application] Create game window success.\n");
		return true;
	}
}


void CApplication::Exit()
{
	DebugOut(L"[Application] Exit called.\n");
	SendMessage(_hWnd, WM_DESTROY, 0, 0);
}


bool CApplication::HandleMessage()
{
	MSG msg = {};
	bool isDone = false;

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			isDone = true;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return isDone;
}


LRESULT CApplication::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}