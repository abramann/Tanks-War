#include "IGame.h"
#include "GameError.h"
//#include "vld\vld.h" // For detecing  memory leaks
#include <memory>
#include <Windows.h>

#pragma comment(lib,"d3d11.lib") // Graphics libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "winmm.lib") // Time(Begin/End)Period
#ifdef _DEBUG
#include "vld/vld.h"
#pragma comment(lib, "vld.lib")

#pragma comment(lib, "DirectXTKd.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#endif


void messageBoxOk(const char* message, const char* title);

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
		messageBoxOk(err.getMessage(), "ERROR");
	}
	catch (...)
	{
		messageBoxOk("Unknown error occurded", "ERROR");
	}

	return 0;
}

void messageBoxOk(const char* msg, const char* title)
{
	MessageBoxA(NULL, msg, title, MB_OK);
}