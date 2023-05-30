// Note parts of this code is licensed under CC BY 3.0

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// winmain.cpp

#include "constants.h"
#ifdef _CLIENT_BUILD
#include "..\Tanks-War\tankswar.h"
typedef TanksWar GameBuildType;
#else ifdef _SERVER_BUILD
#include "..\Tanks-War-Server\tankswarServer.h"
typedef TanksWarServer GameBuildType;
#endif
#include "fileio.h"
#include <memory>

#ifdef _DEBUG
 #include "vld\vld.h" // For detect memory leaks
#endif
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"d3dx9.lib")
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

int message(std::string msg, int type)
{
	return MessageBoxA(NULL, msg.c_str(), "Tanks War", type);
}

bool createGameWindow(HWND& hwnd, HINSTANCE hInstance, int nCmdShow)
{
	const char CLASS_NAME[] = "GameClass";

	//HWND hwnd;
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
	HWND hwnd = NULL;
	g_gameInfo = FileIO::readGameInfo();
	if (!createGameWindow(hwnd, hInstance, nCmdShow))
	{
		message("CreateGameWindow() failed !", MB_OK);
		return 1;
	}

	MSG msg;
	try
	{
		game->initialize(hInstance, hwnd);
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
		MessageBoxA(NULL, err.getMessage(), "EROR", MB_OK);
		DestroyWindow(hwnd);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unknown error occurded !", "ERROR", MB_OK);
		DestroyWindow(hwnd);
	}

	return 0;
}

