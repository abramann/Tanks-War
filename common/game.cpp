#include "game.h"
#include "imgui\imgui_impl_win32.h"
#include <timeapi.h>

Game::Game() : m_timeDeltaMs(0)
{
	m_pGraphics = new Graphics;
	m_pInput = new Input;
	m_pAudio = new Audio;
	m_pTextureManger = new TextureManger;
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
}

LRESULT Game::messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pInput != nullptr && m_pInput->m_handleInput)
		m_pInput->handle(msg, wParam, lParam);

	return DefWindowProcA(hWnd, msg, wParam, lParam);
}
constexpr auto LOGO_WIDTH = 800;
constexpr auto LOGO_HEIGHT = 600;

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

	m_pMap->initialize(this);
	m_logo.initialize(LOGO_WIDTH, LOGO_HEIGHT, 1, 1, 0, 0, m_pTextureManger->getLogoTexture(), m_pGraphics);
	m_logo.cover();	
	m_freq = getFreq();
	m_startTime = getTime();
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
	m_endTime = getTime();
	m_timeDeltaMs = (float)(m_endTime - m_startTime);// / (float)m_freq;
	m_startTime = m_endTime;
	if (m_timeDeltaMs < MIN_FRAME_TIME)
	{
		Sleep(MIN_FRAME_TIME - m_timeDeltaMs);
		m_timeDeltaMs = MIN_FRAME_TIME;
	}
	else if (m_timeDeltaMs > MAX_FRAME_TIME)
		m_timeDeltaMs = MAX_FRAME_TIME;

	static float fpsUpdateDelay = 0;
	fpsUpdateDelay += m_timeDeltaMs;
	if (fpsUpdateDelay >= FPS_UPDATE_DELAY)
	{
		m_fps = 1 / m_timeDeltaMs;
		fpsUpdateDelay = 0;
	}

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
