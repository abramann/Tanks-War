#include "Input.h"
#include "ImGui\imgui_impl_win32.h"
#include <Windows.h>


CInput input;
CInput* g_pInput = &input;

CInput::CInput()
{
}


CInput::~CInput()
{
}

void CInput::initialize()
{
	ImGui::GetIO().KeyRepeatDelay = 0.0f;
	ImGui::GetIO().KeyRepeatRate = 0.05f;
}

bool CInput::isKeyDown(Key key) const
{
	return ImGui::IsKeyDown(key);
}

bool CInput::isKeyPressed(Key key) const
{
	return false;
}

bool CInput::isKeyReleased(Key key) const
{
	return false;
}
