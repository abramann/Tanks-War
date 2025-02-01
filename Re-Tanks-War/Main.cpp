#include "Game.h"
#include "GameError.h"
#include "Values.h"
#include <Windows.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "winmm.lib") // Time(Begin/End)Period

#ifdef _DEBUG
#pragma comment(lib, "DirectXTKd.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#endif

#ifndef _WIN64	// Trouble with x64 Win10-11
#include "vld\vld.h" // For detecing memory leaks

#pragma comment(lib, "vld.lib")
#endif

void messageBoxOk(const std::string& msg);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		g_pGame->start();
	}
	catch (const CGameError& err)
	{
		messageBoxOk(err.getMessage());
	}
	catch (...)
	{
		messageBoxOk("Unknown exception occurded");
	}

	return 0;
}

void messageBoxOk(const std::string& msg)
{
	MessageBoxA(NULL, msg.c_str(), values::GAME_NAME, MB_OK);
}