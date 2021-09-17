#include "Application.h"

/// <summary>
/// Create window by settings. Created window assigned to _hWnd.
/// </summary>
/// <param name="hInstance">: Process instance handler.</param>
/// <param name="title">: Window title.</param>
/// <param name="width">: Window client zone width.</param>
/// <param name="height">: Window client zone height.</param>
/// <param name="isFullscreen">: Window fullscreen setting.</param>
void CApplication::Create(HINSTANCE hInstance, LPCWSTR title, unsigned int width, unsigned int height, bool isFullscreen)
{
}


/// <summary>
/// Send Exit message to Window Message Loop.
/// </summary>
void CApplication::Exit()
{
}


/// <summary>
/// Window Messagle Loop wrapper.
/// </summary>
/// <returns>true if Exit is called.</returns>
bool CApplication::HandleMessage()
{
	return false;
}


/// <summary>
/// Window Procedure, standard Win32 API callback function for handling Messagle Loop.
/// </summary>
LRESULT CApplication::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}