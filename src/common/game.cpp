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

Game::Game() : m_timeDeltaMillsec(0)
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

LRESULT Game::messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pInput != nullptr && m_pInput->m_handleInput)
		m_pInput->handle(msg, wParam, lParam);

	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void Game::initialize(HINSTANCE hInstance, HWND hwnd)
{
	m_onResize = false;
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
	if (m_pInput->isKeyDown(inputNS::ESCAPE_KEY))
		PostQuitMessage(0);

	updateGame();
	renderGame();
}

void Game::renderGame()
{
	m_pGraphics->beginRender();
	//	 if (m_logo.drawRapidly())
	render();
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	m_pGraphics->endRender();
	m_pGraphics->showBackbuffer();
}

void Game::updateGame()
{
	m_pTimer->update();
	m_timeDeltaMillsec = m_pTimer->getTimeDelta();
	update();
}

bool Game::checkGameFiles() const
{
	for (auto file : gameNS::gameFiles)
	{
		auto existHash = FileIO::getCRC32(file.first);
		auto orgHash = gameNS::gameFiles.at(file.first);
		if (existHash != orgHash)
		{
			std::string msg = strFormat("File %s doesn't exist or is corrupted.", file.first.c_str());
			messageBoxOk(msg, "ERROR");
			return false;
		}
	}

	return true;
}

void Game::setResolution(int16 width, int16 height) const
{
	GameInfo info = FileIO::readGameInfo();
	info.width = width;
	info.height = height;
	FileIO::createGameInfo(&info);
}

void Game::setWindowed(bool windowed) const
{
	GameInfo info = FileIO::readGameInfo();
	info.windowed = windowed;
	FileIO::createGameInfo(&info);
}

Resolution Game::getResolution() const
{
	auto gameInfo = FileIO::readGameInfo();
	Resolution resol;
	resol.width = gameInfo.width;
	resol.height = gameInfo.height;
	return resol;
}

bool Game::isWindowed() const
{
	auto gameInfo = FileIO::readGameInfo();
	return gameInfo.windowed;
}

void Game::showLogo() const
{
	Sprite logo = Sprite(this, "logo");
	V2 scal = V2(g_gameInfo.width*1.0f / logo.getWidth(),
		g_gameInfo.height*1.0f / logo.getHeight());
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