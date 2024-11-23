#include "IGame.h"
#include "GameError.h"
#include "Values.h"
#include "vld\vld.h" // For detecing  memory leaks
#include <memory>
#include <Windows.h>

#pragma comment(lib,"d3d11.lib") // Graphics libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "winmm.lib") // Time(Begin/End)Period

#ifdef _DEBUG
#pragma comment(lib, "DirectXTKd.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#endif

#ifdef _WIN64
#pragma comment(lib, "Lib/x64/vld.lib")
#else
#pragma comment(lib, "Lib/Win32/vld.lib")
#endif

void messageBoxOk(const std::string& msg);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		g_pGame->initialize();
		while (g_pGame->pollMessages())
			g_pGame->run();
	}
	catch (const CGameError& err)
	{
		messageBoxOk(ERROR + err.getMessage().c_str());
	}
	catch (...)
	{
		messageBoxOk("ERROR: Unknown error occurded");
	}

	return 0;
}

void messageBoxOk(const std::string& msg)
{
	MessageBoxA(NULL, msg.c_str(), values::GAME_NAME, MB_OK);
}