#include "Client.h"
#include "GameError.h"
#include "GameSystem.h"
#include "Values.h"
#include "ImGui\imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace std;

static CClient client;
IGame* g_pGame = &client;


LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

CClient::CClient()
{
}

CClient::~CClient()
{

}

void CClient::initialize()
{
	readInitializeSettings();
	createWindow();
	g_pGameSystem->startup();
}

void CClient::run()
{
	g_pGameSystem->run();
}

void CClient::shutdown()
{
	g_pGameSystem->shutdown();
}

bool CClient::pollMessages()
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

LRESULT CClient::messageHandler(HWindow hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool imGuiInitialized = false;

	if (imGuiInitialized)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
	}
	else
	{
		imGuiInitialized = ImGui::GetCurrentContext() ? true : false;
	}

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int CClient::getWindowWidth() const
{
	return m_settings.windWidth;
}

int CClient::getWindowHeight() const
{
	return m_settings.windHeight;
}

HWindow CClient::getWindowHandle() const
{
	return m_hwnd;
}

bool CClient::isWindowed() const
{
	return m_settings.windowed;
}

void CClient::createWindow()
{
	const wchar_t CLASS_NAME[] = L"Tanks War";
	HINSTANCE instance = GetModuleHandle(NULL);
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	//wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = WinProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	//wc.hCursor = LoadCursorFromFileA("Assets\\cursor.cur");
	wc.lpszClassName = CLASS_NAME;
	RegisterClassEx(&wc);

	RECT rect;
	setNull(rect);
	rect.right = m_settings.windWidth;
	rect.bottom = m_settings.windHeight;
	DWORD style = WS_POPUP | WS_VISIBLE | WS_EX_LAYERED;
	if (m_settings.windowed)
		style |= WS_BORDER | WS_EX_LAYOUTRTL;
	else
		style |= WS_EX_TOPMOST;

	AdjustWindowRect(&rect, style, false);
	m_hwnd = CreateWindowEx(NULL, CLASS_NAME,CLASS_NAME, style,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, instance, NULL);
	
	CHECK_ERROR(m_hwnd, "Failed to create window");
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);
}

void CClient::readInitializeSettings()
{
	// TODO: read Tanks-War.ini
	m_settings.windWidth = values::MIN_WINDOW_WIDTH;
	m_settings.windHeight = values::MIN_WINDOW_HEIGHT;
	m_settings.windowed = true;
}

LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_pGame->messageHandler(hwnd, msg, wParam, lParam);
}