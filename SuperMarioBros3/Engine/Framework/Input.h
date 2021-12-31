#pragma once
#include <dinput.h>


class CKeyHandler
{
public:
	virtual void KeyState() = 0;
	virtual void OnKeyDown(int keyCode) = 0;
	virtual void OnKeyUp(int keyCode) = 0;
};
typedef CKeyHandler* pKeyHandler;


struct CInput
{
	static const unsigned int KEYBOARD_BUFFER_SIZE = 1024;
	static const unsigned int KEYBOARD_STATE_SIZE = 256;
	LPDIRECTINPUT8 _directInput;
	LPDIRECTINPUTDEVICE8 _device;
	BYTE _keyStates[CInput::KEYBOARD_STATE_SIZE];
	DIDEVICEOBJECTDATA _keyEvents[CInput::KEYBOARD_BUFFER_SIZE];
	pKeyHandler _keyHandler;

	void Initialize(HINSTANCE hInstance, HWND hWnd, pKeyHandler keyHandler);
	void Shutdown();
	void Process();
};
typedef CInput* pInput;