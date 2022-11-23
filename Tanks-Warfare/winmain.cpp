#include "always.h"
#include "tanksWarfare.h"
#define _CRTDBG_MAP_ALLOC // For detecting memory leaks
#include <stdlib.h> // For detecting memory leaks
#include <crtdbg.h> // For detecting memory leaks

bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow);
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool isFullScreen();

bool fullscreen;

TanksWarfare* game;

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->messageHandler(hWnd, msg, wParam, lParam);
}
int message(std::string msg, int type)
{
	return MessageBoxA(NULL, msg.c_str(), "Tanks Warfare", type);
}
bool AnotherInstance()
{
	HANDLE ourMutex;
	// Attempt to create a mutex using our unique string
	ourMutex = CreateMutexA(NULL, true, "Use_a_different_string_here_for_each_program_48161-XYZZY");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;
	// Another instance was found
	return false;
}
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow)
{
	if (AnotherInstance() == true)
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
	wcx.hIcon = LoadIconA(NULL,"Data//tankswarfare.ico");
	wcx.hCursor = LoadCursorFromFileA("Data//tankswarfare.cursor");   // predefined arrow 
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // black background 
	wcx.hIconSm = NULL;                 // small class icon 
	wcx.lpszMenuName = NULL;           // name of menu resource 
	wcx.lpszClassName = L"Game Class";     // name of window class 
	// Register the window class. 
	// RegisterClassEx returns 0 on error.
	if (RegisterClassEx(&wcx) == 0)    // if error
		return false;

	//set up the screen in windowed or fullscreen mode?
	DWORD style;
	if (fullscreen)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_EX_TOPMOST;
	// Create window
	hwnd = CreateWindowA(
		"Game Class",
		"Tanks Warfare",             // title bar text
		style,                  // window style
		CW_USEDEFAULT,          // default horizontal position of window
		CW_USEDEFAULT,          // default vertical position of window
		GAME_WIDTH,             // width of window
		GAME_HEIGHT,            // height of the window
		(HWND)NULL,            // no parent window
		(HMENU)NULL,           // no menu
		hInstance,              // handle to application instance
		(LPVOID)NULL);         // no window parameters

	// if there was an error creating the window
	if (!hwnd)
		return false;

	if (!fullscreen)             // if window
	{
		// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
			TRUE);                                       // Repaint the window
	}

	// Show the window
	ShowWindow(hwnd, nCmdShow);
	
	// Send a WM_PAINT message to the window procedure
	UpdateWindow(hwnd);
	return true;
}
bool isFullScreen()
{
	if (message("Run full screen ? ", MB_OKCANCEL) == 2)
		return false;
	else
		return true;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	game = new TanksWarfare;
	MSG msg;
	HWND hWnd = NULL;
	fullscreen = isFullScreen();
	if (!CreateMainWindow(hWnd, hInstance, nCmdShow))
	{
		message("CreateMainWindow() failed !", MB_OK);
		return 1;
	}

	try
	{
		game->initialize(hInstance, hWnd, fullscreen);

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

	safeDelete(game);
	return 0;
}

