#include "input.h"

Input::Input()
{
	directInput = NULL;
	keyboard = NULL; mouse = NULL;
	reset();
}
Input::~Input()
{
	keyboard->Release(); 
	mouse->Release();
	directInput->Release();
}
HRESULT Input::initialize(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(hr))
		return E_FAIL;

	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(hr))
		return E_FAIL;

	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return E_FAIL;

	hr = keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return E_FAIL;

	hr = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(hr))
		return E_FAIL;

	hr = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return E_FAIL;

	hr = mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return E_FAIL;

	reset();
	return S_OK;
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