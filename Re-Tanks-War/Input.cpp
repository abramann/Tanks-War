#include "Input.h"
#include "ImGui\imgui_impl_win32.h"
#include <Windows.h>


CInput::CInput()
{
}


CInput::~CInput()
{
}

void CInput::startUp()
{
	ImGui::GetIO().KeyRepeatDelay = 0.0f;
	ImGui::GetIO().KeyRepeatRate = 0.05f;
}

void CInput::handle()
{
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
