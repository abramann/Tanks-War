#include "constants.h"
#include "tankswarserver.h"
#include "fileio.h"

#define _CRTDBG_MAP_ALLOC // For detecting memory leaks
#include <stdlib.h> // For detecting memory leaks
#include <crtdbg.h> // For detecting memory leaks
#include <vld.h>

#pragma comment(lib,"winmm.lib") // Includes timeGetTime
#pragma comment(lib,"d3d9.lib")	// Include dx9 apis
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")	// Include dinput8 apis
#pragma comment(lib,"dxguid.lib")

bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow);
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool isFullScreen();

// bool fullscreen;
GameInfo g_gameInfo;

TanksWarServer* game;

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->messageHandler(hWnd, msg, wParam, lParam);
}

int message(std::string msg, int type)
{
	return MessageBoxA(NULL, msg.c_str(), "Tankswar-Server", type);
}

bool alreadyOpen()
{
	return false;
	HANDLE mutex;
	mutex = CreateMutexA(NULL, true, "tankswarbyabramann");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;
	return false;
}

bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow)
{
	if (alreadyOpen())
		return false;

	WNDCLASSEX wcx;
	// Fill in the window class structure with parameters 
	// that describe the main window. 
	wcx.cbSize = sizeof(wcx);           // size of structure 
	wcx.style = CS_HREDRAW | CS_VREDRAW;    // redraw if size changes 
	wcx.lpfnWndProc = WinProc;          // points to window procedure 
	wcx.cbClsExtra = 0;                 // no extra class memory 
	wcx.cbWndExtra = 0;                 // no extra window memory 
	wcx.hInstance = hInstance;          // handle to instance 
	wcx.hIcon = LoadIcon(NULL,"Assets//tankswar.icon");
	wcx.hCursor = LoadCursorFromFile("Assets//tankswar.cursor");   // predefined arrow 
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // black background 
	wcx.hIconSm = NULL;                 // small class icon 
	wcx.lpszMenuName = NULL;           // name of menu resource 
	wcx.lpszClassName = "Game Class";     // name of window class 
	// Register the window class. 
	// RegisterClassEx returns 0 on error.
	if (RegisterClassEx(&wcx) == 0)    // if error
		return false;

	//set up the screen in windowed or fullscreen mode?
	DWORD style = (::g_gameInfo.fullScreen) ? WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP : WS_EX_TOPMOST;
	// Create window
	hwnd = CreateWindow(
		"Game Class",
		"Tankswar-Server",             // title bar text
		style,                  // window style
		CW_USEDEFAULT,          // default horizontal position of window
		CW_USEDEFAULT,          // default vertical position of window
		::g_gameInfo.width,           // width of window
		::g_gameInfo.height,            // height of the window
		(HWND)NULL,            // no parent window
		(HMENU)NULL,           // no menu
		hInstance,              // handle to application instance
		(LPVOID)NULL);         // no window parameters

	// if there was an error creating the window
	if (!hwnd)
		return false;

	if (!::g_gameInfo.fullScreen)             // if window
	{
		// Adjust window size so client area is GAME_WIDTH x pGameInfo->height
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			::g_gameInfo.width + (::g_gameInfo.width - clientRect.right),    // Right
			::g_gameInfo.height + (::g_gameInfo.height - clientRect.bottom), // Bottom
			TRUE);                                         // Repaint the window
	}

	// Show the window
	ShowWindow(hwnd, nCmdShow);
	
	// Send a WM_PAINT message to the window procedure
	UpdateWindow(hwnd);
	return true;
}

bool isFullScreen()
{
	return (message("Run full screen ? ", MB_OKCANCEL) == 2) ? false : true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	game = new TanksWarServer;

	HWND hWnd = NULL;
//	fullscreen = isFullScreen();
	::g_gameInfo = FileIO::readGameInfo();
	if (!CreateMainWindow(hWnd, hInstance, nCmdShow))
	{
		message("CreateMainWindow() failed !", MB_OK);
		return 1;
	}

	MSG msg;
	try
	{
		game->initialize(hInstance, hWnd);
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
	catch (const GameError &err)
	{
		MessageBoxA(NULL, err.getMessage(), "EROR", MB_OK);
		DestroyWindow(hWnd);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unknown error occurded !", "ERROR", MB_OK);
		DestroyWindow(hWnd);
	}

	SAFE_DELETE(game);
	return 0;
}

