// winmain.cpp
// Author: abramann

// Note this file is influenced by winmain.cpp from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0
// Note parts of this code are licensed under CC BY 3.0

#include "types.h"
#ifdef _CLIENT_BUILD
#include "..\Client\tankswar.h"
typedef TanksWar GameBuildType;
#else ifdef _SERVER_BUILD
#include "..\Server\tankswarServer.h"
typedef TanksWarServer GameBuildType;
#endif
#include "fileio.h"
#include <memory>

#ifdef _DEBUG
#include "vld\vld.h" // For detect memory leaks

#pragma comment(lib,"sfml-audio-d.lib")
#else
#pragma comment(lib,"sfml-audio.lib")
#endif
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Ws2_32.lib")
#ifdef _BUILD_WITH_D3D9
#pragma comment(lib,"d3d9.lib")
#else ifdef _BUILD_WITH_D3D11
//#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"dxgi.lib")
#endif

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::shared_ptr<GameBuildType> game;

GameInfo g_gameInfo;

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->messageHandler(hWnd, msg, wParam, lParam);
}

bool createGameWindow(HWND& hwnd, HINSTANCE hInstance, int nCmdShow)
{
	const char CLASS_NAME[] = "GameClass";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInstance;
	//wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
	wc.hCursor = LoadCursorFromFileA("Assets\\cursor.cur");
	wc.lpszClassName = CLASS_NAME;
	RegisterClassEx(&wc);
	DWORD style = (g_gameInfo.windowed) ? WS_OVERLAPPED : WS_EX_TOPMOST | WS_POPUP;
	hwnd = CreateWindowEx(NULL,
		CLASS_NAME,
		"Tanks War",
		style,    // fullscreen values
		0, 0,    // the starting x and y positions should be 0
		g_gameInfo.width, g_gameInfo.height,    // set the window to 640 x 480
		NULL,
		NULL,
		hInstance,
		NULL);
	if (hwnd == 0)
		return false;

	RegisterClassEx(&wc);
	if (g_gameInfo.windowed)
	{
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			g_gameInfo.width + (g_gameInfo.width - clientRect.right),    // Right
			g_gameInfo.height + (g_gameInfo.height - clientRect.bottom), // Bottom
			TRUE);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	game = std::make_shared<GameBuildType>();
	bool filesExist = game->checkGameFiles();
	if (!filesExist)
		return 1;

	HWND hwnd = NULL;
	g_gameInfo = FileIO::readGameInfo();
	if (!createGameWindow(hwnd, hInstance, nCmdShow))
	{
		messageBoxOk("CreateGameWindow() failed !", "ERROR");
		return 1;
	}

	MSG msg;
	try
	{
		game->initialize(hInstance, hwnd);
#ifndef _DEBUG
		game->showLogo();
#endif
		while (true)
		{
			// PeekMessage,non-blocking method for checking for Windows messages.
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// look for quit message
				if (msg.message == WM_QUIT)
					break;

				// decode and pass messages on to WinProc
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				game->run();
		}
	}
	catch (const GameError& err)
	{
		messageBoxOk(err.getMessage(), "ERROR");
		DestroyWindow(hwnd);
	}
	catch (...)
	{
		messageBoxOk("Unknown error occurded !", "ERROR");
		DestroyWindow(hwnd);
	}

	return 0;
}