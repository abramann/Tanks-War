#include "game.h"
#include "imgui\imgui_impl_win32.h"
#include <timeapi.h>

Game::Game() : m_timeDelta(0)
{
	m_pGraphics = new Graphics;
	m_pInput = new Input;
	m_pAudio = new Audio;
	m_pTextureManger = new TextureManger;
	m_pInterface = new Interface;
	m_pMap = new Map;
}

Game::~Game()
{
	SAFE_DELETE(m_pGraphics);
	SAFE_DELETE(m_pInput);
	SAFE_DELETE(m_pAudio);
}

LRESULT Game::messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pInput->m_handleInput)
		m_pInput->handle(msg, wParam, lParam);

	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void Game::initialize(HINSTANCE hInstance, HWND hWnd)
{
	if (!m_pInput->initialize(hWnd))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize m_pInput");
	if(!m_pGraphics->initialize(hWnd))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize m_pGraphics");
	if (!m_pAudio->initialize())
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize m_pAudio");
	if (!m_pTextureManger->initialize(m_pGraphics))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize m_pTextureManger");

	m_pMap->initialize(m_pTextureManger->getTiledMapTexture(), m_pGraphics);
	m_pInterface->initialize();
	m_logo.initialize(800, 400, 1, 1, 0, 0, m_pTextureManger->getLogoTexture(), m_pGraphics);
	m_logo.cover();	
}

void Game::run()
{
	if (m_pInput->isKeyIn(ESCAPE_KEY))
		PostQuitMessage(0);

	updateGame();
	collision();
	renderGame();
	m_pAudio->run();
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
	m_timeDelta = timeGetTime() - m_timeDelta;
 	if (m_timeDelta < MIN_FRAME_TIME)
	{
		Sleep(MIN_FRAME_TIME - m_timeDelta);
		m_timeDelta = MIN_FRAME_TIME;
	}
	else if (m_timeDelta > MAX_FRAME_TIME)
	{
		m_timeDelta = MAX_FRAME_TIME;
	}

	update();
	m_timeDelta = timeGetTime();
}

void Game::handleLostGraphicsDevice()
{
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
