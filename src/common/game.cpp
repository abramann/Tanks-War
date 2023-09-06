// game.cpp
// Author: abramann
// Note this file is influenced by game.cpp from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0
// Note parts of this code are licensed under CC BY 3.0

// Programming 2D Games Copyright (c) 2011 by: Charles Kelly
// game.cpp

#include "game.h"
#include "imgui\imgui_impl_win32.h"
#include "texturemanger.h"
#include "graphics.h"
#include "input.h"
#include "timer.h"
#include "texture.h"
#include "image.h"
#include "audio.h"
#include "interface.h"
#include "map.h"
#include "dx11wrapper.h"
#include "sprite.h"
#include <fileio.h>
#include "imgui\imgui_impl_dx11.h"
#include "imgui\imgui.h"

Game::Game()
{
	m_pGraphics = std::make_shared<Graphics>();
	m_pInput = std::make_shared<Input>();
	m_pAudio = std::make_shared<Audio>();
	m_pTextureManger = std::make_shared<TextureManger>();
	m_pTimer = std::make_shared<Timer>();
	m_pInterface = std::make_shared<Interface>();
	m_pMap = std::make_shared<Map>();
	m_pDx11Wrapper = std::make_shared<Dx11Wrapper>();
}

Game::~Game()
{
	m_pGraphics.reset(); // for release ImGui_d3d implemet before imgui_win implement
}

LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pInput != nullptr && m_pInput->isInputHandled())
		m_pInput->handle(msg, wParam, lParam);
	if (msg == WM_DESTROY)
		PostQuitMessage(0);

	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void Game::initialize(HINSTANCE hInstance, HWND hwnd)
{
	m_hwnd = hwnd;
	if (!m_pInput->initialize(m_hwnd))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Input");
	if (!m_pDx11Wrapper->initialize(this))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Dx11Wrapper");
	if (!m_pGraphics->initialize(this))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Graphics");

	m_pAudio->initialize();
	m_pTextureManger->initialize(m_pGraphics.get());
	m_pMap->initialize(this);
}

void Game::run()
{
	if (isOnline())
	{
		updateGame();
		communicate();
	}

	m_pTimer->update();
	renderGame();
}

void Game::renderGame()
{
	m_pGraphics->beginRender();
	render();
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	m_pGraphics->endRender();
	m_pGraphics->showBackbuffer();
}

void Game::updateGame()
{
	update();
}

void Game::onUpdateDisplay() const
{
	m_pDx11Wrapper->setFullScreen(false);
	RECT rect = { 0 };
	rect.right = g_pGameSettings->width,
		rect.bottom = g_pGameSettings->height;
	DWORD style = WS_POPUP | WS_VISIBLE | WS_EX_LAYERED;
	if (g_pGameSettings->windowed)
		style |= WS_BORDER | WS_EX_LAYOUTRTL;
	else
		style |= WS_EX_TOPMOST;

	SetWindowLongPtr(m_hwnd, GWL_STYLE, style);
	AdjustWindowRect(&rect, style, false);
	SetWindowPos(m_hwnd, 0, 0, 0, 
		rect.right - rect.left,
		rect.bottom - rect.top, 0);
	m_pDx11Wrapper->onResize(g_pGameSettings->width, g_pGameSettings->height);
	m_pGraphics->onResize();
	updateGameSettings();
}

void Game::updateGameSettings() const
{
	FileIO::createGameSettings(g_pGameSettings);
}

bool Game::checkGameFiles() const
{
	for (auto file : gameNS::gameFiles)
	{
		auto existHash = FileIO::getCRC32(file.first);
		auto orgHash = gameNS::gameFiles.at(file.first);
		if (existHash != orgHash)
		{
			messageBoxOk(strFormat("File %s doesn't exist or is corrupted.", file.first.c_str()), "ERROR");
			return false;
		}
	}

	return true;
}

void Game::showLogo() const
{
	Sprite logo = Sprite(this, "logo");
	V2 scal = V2(g_pGameSettings->width*1.0f / logo.getWidth(),
		g_pGameSettings->height*1.0f / logo.getHeight());
	logo.setScaling(scal);
	bool end = false;
	do
	{
		m_pGraphics->beginRender();
		end = logo.fadeDraw(800);
		m_pGraphics->endRender();
		m_pGraphics->showBackbuffer();
	} while (!end);
}