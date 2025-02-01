#include "Utility.h"
#include "Win32Window.h"
#include "ImGui/imgui_impl_win32.h"
#include "GameError.h"
#include <Windows.h>

static CWin32Window window;
IWindow* g_pWindow = &window;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWindow CWin32Window::createHandle(int w, int h, bool fullscreen, const std::string & title,
									bool borderless)
{
	HINSTANCE instance = GetModuleHandle(NULL);

	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	//wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = WinProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	//wc.hCursor = LoadCursorFromFileA("Assets\\cursor.cur");

	std::wstring wtitle = to_wstring(title);

	wc.lpszClassName = wtitle.c_str();
	
	RegisterClassEx(&wc);

	RECT rect;

	rect.right = w;
	rect.bottom = h;

	rect.top = rect.left = 0;

	DWORD style = WS_POPUP | WS_VISIBLE | WS_EX_LAYERED;

	if (fullscreen)
		style |= WS_EX_TOPMOST;
	else
		style |= WS_BORDER | WS_EX_LAYOUTRTL;

	AdjustWindowRect(&rect, style, false);

	HWindow handle = CreateWindowEx(NULL, wtitle.c_str(), wtitle.c_str(), style,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, instance, NULL);

	CHECK_ERROR(handle == nullptr, "Failed to create window");

	ShowWindow(handle, SW_SHOWDEFAULT);
	UpdateWindow(handle);

	return handle;
}

void CWin32Window::deleteHandle(HWindow handle)
{
	DestroyWindow(handle);
}

bool CWin32Window::pollMessages()
{
	static bool run = true;

	if (!run)
		return false;

	MSG msg;

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			run = false;
			break;
		}

		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return run;
}

LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool imGuiInitialized = false;

	if (imGuiInitialized)
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
	else
		imGuiInitialized = ImGui::GetCurrentContext() ? true : false;

	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
