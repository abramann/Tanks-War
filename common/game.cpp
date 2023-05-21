// Note parts of code are licensed under CC BY 3.0

// Programming 2D Games Copyright (c) 2011 by: Charles Kelly 
// game.cpp

#include "game.h"
#include "imgui\imgui_impl_win32.h"
#include "texturemanger.h"

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
	safeDelete(m_pGraphics);
	safeDelete(m_pInput);
	safeDelete(m_pAudio);
	safeDelete(m_pInterface);
	safeDelete(m_pTextureManger);
	safeDelete(m_pMap);
	safeDelete(m_pTimer);
}

LRESULT Game::messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pInput != nullptr && m_pInput->m_handleInput)
		m_pInput->handle(msg, wParam, lParam);

	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void Game::initialize(HINSTANCE hInstance, HWND hwnd)
{
	m_hwnd = hwnd;
	if (!m_pInput->initialize(m_hwnd))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Input");
	if(!m_pGraphics->initialize(this))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Graphics");
	if (!m_pAudio->initialize())
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Audio");
	if (!m_pTextureManger->initialize(m_pGraphics))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize TextureManger");

	m_pMap->initialize(this);
//	m_logo.initialize(m_pTextureManger->getTexture(TEXTURE_LOGO), this);
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
	m_pGraphics->showBackbuffer();
}

void Game::updateGame()
{
	m_pTimer->update();
	m_timeDeltaMillsec = m_pTimer->getTimeDelta();
	update();
}