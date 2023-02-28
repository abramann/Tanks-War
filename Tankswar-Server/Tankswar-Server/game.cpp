#include "game.h"

Game::Game() : m_timeDelta(0)
{
	m_pGraphics = new Graphics;
	m_pInput = new Input;
	m_pAudio = new Audio;
}

Game::~Game()
{
	SAFE_DELETE(m_pGraphics);
	SAFE_DELETE(m_pInput);
	SAFE_DELETE(m_pAudio);
}

LRESULT Game::messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		m_pInput->scan();
		if (m_pInput->isKeyIn(ESCAPE_KEY))
			PostQuitMessage(0);
		break;
	}
	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void Game::initialize(HINSTANCE hInstance, HWND hWnd)
{
	if(!m_pGraphics->initialize(hWnd))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize m_pGraphics");
	if(!m_pInput->initialize(hInstance, hWnd))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize m_pInput");
	m_pInput->scan();
	if (!m_pAudio->initialize())
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize m_pAudio");
}

void Game::run()
{
	updateGame();
	collision();
	renderGame();
	m_pAudio->run();
	m_pInput->reset();
}

void Game::renderGame()
{
	m_pGraphics->begin();
	render();
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
