#include "input.h"

Input::Input() : m_lpDirectInput(NULL), m_lpKeyboard(NULL), m_lpMouse(NULL)
{
	reset();
}

Input::~Input()
{
	release();
}

bool Input::initialize(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpDirectInput, NULL);
	if (FAILED(hr))
		return false;

	hr = m_lpDirectInput->CreateDevice(GUID_SysKeyboard, &m_lpKeyboard, NULL);
	if (FAILED(hr))
		return false;

	hr = m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return false;

	hr = m_lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return false;

	hr = m_lpDirectInput->CreateDevice(GUID_SysMouse, &m_lpMouse, NULL);
	if (FAILED(hr))
		return false;

	hr = m_lpMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return false;

	hr = m_lpMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return false;

	reset();
	return true;
}

void Input::scan()
{
	m_lpKeyboard->Acquire();
	m_lpKeyboard->GetDeviceState(sizeof(BYTE) * 256, (LPVOID)m_keyboardState);
}

void Input::keyDown(BYTE k)
{
	m_keyboardState[k] = true;
}

void Input::release()
{
	SAFE_RELEASE(m_lpKeyboard);
	SAFE_RELEASE(m_lpMouse);
	SAFE_RELEASE(m_lpDirectInput);
}

void Input::reset()
{
	ZeroMemory(m_keyboardState, sizeof(BYTE) * 256);
}

bool Input::isKeyIn(BYTE k)
{
	if (m_keyboardState[k])
		return true;
	return false;
}

bool Input::anyKeyPressed()
{
	for (auto i = 0; i < 256; i++)
		if (m_keyboardState[i])
			return true;
	return false;
}
