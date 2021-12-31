#pragma once
#include <Windows.h>
#include <string>


struct CApplication
{
	HINSTANCE _hInstance;
	HWND _hWnd;

	bool Create(HINSTANCE hInstance, std::wstring title, unsigned int width, unsigned int height);
	void Exit();
	bool HandleMessage();
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
typedef	CApplication* pApplication;