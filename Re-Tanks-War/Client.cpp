#include "Client.h"
#include "GameSystem.h"
#include "Window.h"

static CClient client;
IGame* g_pGame = &client;

void CClient::initialize()
{
	g_pWindow->create();

	g_pGameSystem->startup();
}

void CClient::run()
{
	while (g_pWindow->pollMessages())
		g_pGameSystem->run();
}

void CClient::shutdown()
{
	g_pGameSystem->shutdown();

	g_pWindow->destroy();
}

