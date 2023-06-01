// input.h
// Author: abramann

#include "input.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Input::Input() : m_handleInput(false), m_hwnd(0)
{
}

Input::~Input()
{
	if (m_hwnd)
	{
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

bool Input::initialize(HWND hwnd)
{
	m_hwnd = hwnd;
	ImGui::CreateContext();
	bool result = ImGui_ImplWin32_Init(m_hwnd);
	if(result)
		m_handleInput = true;

/*	// ImGui::GetIO().KeyRepeatDelay = 0.257f;
	ImGui::GetIO().KeyRepeatRate = 0.05f;*/
	
	ImGui::GetIO().KeyRepeatDelay = 0.0f;
	ImGui::GetIO().KeyRepeatRate = 0.05f; 
	ImGui::GetIO().IniFilename = 0;
	return result;
}

void Input::handle(UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(m_hwnd, msg, wParam, lParam);
}

bool Input::isKeyDown(Key key)
{
	return (ImGui::IsKeyDown(key)) ? true : false;
}

bool Input::isKeyPressed(Key key)
{
	return (ImGui::IsKeyPressed(key)) ? true : false;
}

bool Input::anyKeyPressed()
{
	for (uint16_t k = ImGuiKey_Tab; k <= ImGuiKey_Pause; k++)
		if (ImGui::IsKeyPressed((ImGuiKey)k))
			return true;

	return false;
}
