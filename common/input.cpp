#include "input.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Input::Input() : m_handleInput(false)
{
}

Input::~Input()
{
}

bool Input::initialize(HWND hwnd)
{
	m_hwnd = hwnd;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	bool result = ImGui_ImplWin32_Init(&m_hwnd);
	if(result)
		m_handleInput = true;

	return result;
}

void Input::handle(UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(m_hwnd, msg, wParam, lParam);
}


bool Input::isKeyIn(Key key)
{
	if (ImGui::IsKeyPressed(key))
		return true;

	return false;
}

bool Input::anyKeyPressed()
{
	for (uint16_t k = ImGuiKey_Tab; k <= ImGuiKey_Pause; k++)
		if (ImGui::IsKeyPressed((ImGuiKey)k))
			return true;

	return false;
}
