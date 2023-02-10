#include "input.h"

Input::Input()
{
	directInput = NULL;
	keyboard = NULL; mouse = NULL;
	reset();
}

Input::~Input()
{
	release();
}

bool Input::initialize(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(hr))
		return false;

	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(hr))
		return false;

	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return false;

	hr = keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return false;

	hr = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(hr))
		return false;

	hr = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return false;

	hr = mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return false;

	reset();
	return true;
}

void Input::scan()
{
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(BYTE) * 256, (LPVOID)keyboardState);
}

void Input::keyDown(BYTE k)
{
	keyboardState[k] = true;
}

void Input::release()
{
	SAFE_RELEASE(keyboard);
	SAFE_RELEASE(mouse);
	SAFE_RELEASE(directInput);
}

void Input::reset()
{
	ZeroMemory(keyboardState, sizeof(BYTE) * 256);
}

bool Input::isKeyIn(BYTE k)
{
	if (keyboardState[k])
		return true;
	return false;
}

bool Input::anyKeyPressed()
{
	for (int i = 0; i < 256; i++)
		if (keyboardState[i])
			return true;
	return false;
}
