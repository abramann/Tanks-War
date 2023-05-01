#include "game.h"
#include "imgui\imgui_impl_win32.h"
#include <timeapi.h>

Game::Game() : m_timeDeltaMillsec(0)
{
	m_pGraphics = new Graphics;
	m_pInput = new Input;
	m_pAudio = new Audio;
	m_pTextureManger = new TextureManger;
	m_pTimer = new Timer;
	m_pInterface = new Interface;
	m_pMap = new Map2;
}

Game::~Game()
{
	SAFE_DELETE(m_pGraphics);
	SAFE_DELETE(m_pInput);
	SAFE_DELETE(m_pAudio);
	SAFE_DELETE(m_pInterface);
	SAFE_DELETE(m_pTextureManger);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pTimer);
}

LRESULT Game::messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pInput != nullptr && m_pInput->m_handleInput)
		m_pInput->handle(msg, wParam, lParam);

	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void Game::initialize(HINSTANCE hInstance, HWND hWnd)
{
	if (!m_pInput->initialize(hWnd))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Input");
	if(!m_pGraphics->initialize(hWnd))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Graphics");
	if (!m_pAudio->initialize())
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Audio");
	if (!m_pTextureManger->initialize(m_pGraphics))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize TextureManger");

	m_pMap->initialize(this);
//	m_logo.initialize(m_pTextureManger->getTexture(TEXTURE_LOGO), this);
//	m_logo.cover();	
}

void Game::run()
{
	if (m_pInput->isKeyIn(ESCAPE_KEY))
		PostQuitMessage(0);

	updateGame();
	collision();
	renderGame();
}

void Game::renderGame()
{
	m_pGraphics->begin();
//	 if (m_logo.drawRapidly())
 		render();

	ImGui::SetMouseCursor(ImGuiMouseCursor_None);

	m_pGraphics->end();
	handleLostGraphicsDevice();
	m_pGraphics->showBackbuffer();
}

void Game::updateGame()
{
	m_pTimer->update();
	m_timeDeltaMillsec = m_pTimer->getTimeDelta();
	update();
}

void Game::handleLostGraphicsDevice()
{
//	return;
	HRESULT hr = m_pGraphics->getDeviceState();
	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);
			return;
		}
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			onLostDevice();
			HRESULT hr = m_pGraphics->reset();
			if (FAILED(hr))
				return;
			onResetDevice();
		}
		else
			throw GameError(gameErrorNS::FATAL_ERROR, "Unknown error occurred !");
	}
}

void Game::onLostDevice()
{
}

void Game::onResetDevice()
{
}
