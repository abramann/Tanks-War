// winmain.cpp
// Author: abramann
// Note this file is influenced by winmain.cpp from Chrles Kelly's Programming 2D Games
// Note parts of this code are licensed under CC BY 3.0

#include "inlined.inl"
#include "fileio.h"
#include <memory>
#ifdef _CLIENT_BUILD
#include "..\Client\tankswar.h"

typedef TanksWar GameBuildType;
#else ifdef _SERVER_BUILD
#include "..\Server\tankswarServer.h"

typedef TanksWarServer GameBuildType;
#endif

#ifdef _DEBUG
#include "vld\vld.h" // To detect memory leaks

#pragma comment(lib, "sfml-audio-d.lib")
#else
#pragma comment(lib,"sfml-audio.lib")
#endif
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"dxgi.lib")

std::unique_ptr<GameBuildType> pGame;
GameSettings* g_pGameSettings;

LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return pGame->messageHandler(hwnd, msg, wParam, lParam);
}

bool createGameWindow(HWND& hwnd, HINSTANCE hInstance, int nCmdShow)
{
	const char CLASS_NAME[] = "Game Class";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	//wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInstance;
	//wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
	wc.hCursor = LoadCursorFromFileA("Assets\\cursor.cur");
	wc.lpszClassName = CLASS_NAME;
	RegisterClassEx(&wc);
	RECT rect = { 0 };
	rect.right = g_pGameSettings->width, rect.bottom = g_pGameSettings->height;
	DWORD style = WS_POPUP | WS_VISIBLE | WS_EX_LAYERED;
	if (g_pGameSettings->windowed)
		style |= WS_BORDER | WS_EX_LAYOUTRTL;
	else
		style |= WS_EX_TOPMOST;

	AdjustWindowRect(&rect, style, false);
	hwnd = CreateWindowEx(NULL,
		CLASS_NAME,
		gameNS::WINDOW_TITLE,
		style,
		0, 0,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (hwnd == 0)
		return false;

	RegisterClassEx(&wc);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	pGame = std::make_unique<GameBuildType>();
	if (!pGame->checkGameFiles())
		return 1;

	GameSettings gameSettings = FileIO::readGameSettings();
	g_pGameSettings = &gameSettings;
	HWND hwnd = NULL;
	if (!createGameWindow(hwnd, hInstance, SW_SHOW))
	{
		messageBoxOk("CreateGameWindow() failed", "ERROR");
		return 1;
	}

	try
	{
		pGame->initialize(hInstance, hwnd);
#ifndef _DEBUG
		pGame->showLogo();
#endif
		while (pollMessages())
			pGame->run();
	}
	catch (const GameError& err)
	{
		messageBoxOk(err.getMessage(), "ERROR");
		DestroyWindow(hwnd);
	}
	catch (...)
	{
		messageBoxOk("Unknown error occurded", "ERROR");
		DestroyWindow(hwnd);
	}

	return 0;
}