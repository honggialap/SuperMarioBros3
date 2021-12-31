#include "Input.h"
#include "Debug.h"


void CInput::Initialize(HINSTANCE hInstance, HWND hWnd, pKeyHandler keyHandler)
{
	_keyHandler = keyHandler;

	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&_directInput, NULL);
	if (hr != DI_OK)
	{
		DebugOut(L"[Input] DirectInput8Create failed!\n");
		return;
	}

	hr = _directInput->CreateDevice(GUID_SysKeyboard, &_device, NULL);
	if (hr != DI_OK)
	{
		DebugOut(L"[Input] CreateDevice failed!\n");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = _device->SetDataFormat(&c_dfDIKeyboard);

	hr = _device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	hr = _device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = _device->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[Input] DINPUT8::Acquire failed!\n");
		return;
	}

	DebugOut(L"[Input] Keyboard has been initialized successfully\n");
}


void CInput::Shutdown()
{
	if (_device)
	{
		_device->Unacquire();
		_device->Release();
	}

	if (_directInput)
	{
		_directInput->Release();
	}
}


void CInput::Process()
{
	HRESULT hr;

	// Collect all key states first
	hr = _device->GetDeviceState(sizeof(_keyStates), _keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = _device->Acquire();
			if (h == DI_OK)
			{
				DebugOut(L"[Input] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	_keyHandler->KeyState();

	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = _device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		DebugOut(L"[Input] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = _keyEvents[i].dwOfs;
		int KeyState = _keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			_keyHandler->OnKeyDown(KeyCode);
		else
			_keyHandler->OnKeyUp(KeyCode);
	}
}
