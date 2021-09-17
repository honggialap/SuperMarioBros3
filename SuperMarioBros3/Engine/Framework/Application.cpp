#include "Application.h"

/// <summary>
/// Create window by settings. Created window assigned to _hWnd.
/// </summary>
/// <param name="hInstance">: Process instance handler.</param>
/// <param name="title">: Window title.</param>
/// <param name="iconPath">: Window title bar icon path.</param>
/// <param name="width">: Window client zone width.</param>
/// <param name="height">: Window client zone height.</param>
/// <param name="isFullscreen">: Window fullscreen setting.</param>
/// <returns>true if creation successful.</returns>
bool CApplication::Create(
	HINSTANCE hInstance, 
	const std::wstring& title, const std::wstring& iconPath, 
	unsigned int width, unsigned int height, bool isFullscreen
)
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
	wcex.hIcon = (HICON)LoadImage(hInstance, iconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"GameWindow";
	wcex.hIconSm = NULL;
	RegisterClassEx(&wcex);

	DWORD windowStyle;
	if (isFullscreen)
	{
		windowStyle = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else
	{
		windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;;
	}

	_hWnd = CreateWindowEx(
		0,
		L"GameWindow",
		title.c_str(),
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL, NULL,
		hInstance,
		NULL
	);

	if (!_hWnd)
	{
		//DebugOut
		return false;
	}
	else
	{
		ShowWindow(_hWnd, SW_SHOWNORMAL);
		UpdateWindow(_hWnd);
		return true;
	}
}


/// <summary>
/// Send Exit message to Window Message Loop.
/// </summary>
void CApplication::Exit()
{
	SendMessage(_hWnd, WM_DESTROY, 0, 0);
}


/// <summary>
/// Window Messagle Loop wrapper.
/// </summary>
/// <returns>true if Exit is called.</returns>
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


/// <summary>
/// Window Procedure, standard Win32 API callback function for handling Messagle Loop.
/// </summary>
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