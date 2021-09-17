#pragma once
#include <Windows.h>

/// <summary>
/// Reponsible for Win32 API's windows creation/ destruction calls and Win32 Message loop.
/// </summary>
struct CApplication
{
	HINSTANCE _hInstance;
	HWND _hWnd;
	WNDCLASSEX _wcex;
	LPCWSTR _title;
	bool _isFullscreen;
	unsigned int _width;
	unsigned int _height;

	void Create(HINSTANCE hInstance, LPCWSTR title, unsigned int width, unsigned int height, bool isFullscreen);
	void Exit();
	bool HandleMessage();
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
typedef	CApplication* pApplication;