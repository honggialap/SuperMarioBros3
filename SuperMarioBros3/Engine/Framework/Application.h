#pragma once
#include <Windows.h>
#include <string>

/// <summary>
/// Reponsible for Win32 API's windows creation/ destruction calls and Win32 Message loop.
/// </summary>
struct CApplication
{
	HINSTANCE _hInstance;
	HWND _hWnd;

	bool Create(
		HINSTANCE hInstance,
		const std::wstring& title, const std::wstring& iconPath, 
		unsigned int width, unsigned int height, bool isFullscreen
	);
	void Exit();
	bool HandleMessage();
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
typedef	CApplication* pApplication;