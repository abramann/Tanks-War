#include "game.h"

Game::Game()
{
	graphics = new Graphics;
	input = new Input;
	audio = new Audio;
	hWnd = NULL;
	fullscreen = NULL;
	timeDelta = 0;
}
Game::~Game()
{
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	SAFE_DELETE(audio);
}
LRESULT Game::messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		input->scan();
		if (input->isKeyIn(ESCAPE_KEY))
			PostQuitMessage(0);
		break;
	}
	return DefWindowProcA(hWnd, msg, wParam, lParam);
}
void Game::initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen)
{
	hWnd = _hWnd;
	fullscreen = _fullscreen;

	if(FAILED(graphics->initialize(hWnd, fullscreen)))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize graphics");
	if(FAILED(input->initialize(hInstance, hWnd)))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize input");
	input->scan();
	if (FAILED(audio->initialize()))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize audio");
}
void Game::run()
{
	updateGame();
	collision();
	renderGame();
	input->reset();
	audio->run();
}
void Game::renderGame()
{
	graphics->begin();
	render();
	graphics->end();
	handleLostGraphicsDevice();
	graphics->showBackbuffer();
}
void Game::updateGame()
{
	timeDelta = timeGetTime() - timeDelta;
	if (timeDelta < MIN_FRAME_TIME)
	{
		Sleep(MIN_FRAME_TIME - timeDelta);
		timeDelta = MIN_FRAME_TIME;
	}
	else if (timeDelta > MAX_FRAME_TIME)
	{
		timeDelta = MAX_FRAME_TIME;
	}

	update();
	timeDelta = timeGetTime();
}
void Game::handleLostGraphicsDevice()
{
	HRESULT hr = graphics->getDeviceState();
	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);
			return;
		}
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			HRESULT hr = graphics->reset();
			if (FAILED(hr))
				return;
			resetAll();
		}
		else
			throw GameError(gameErrorNS::FATAL_ERROR, "Unknown error occurred !");
	}
}
void Game::releaseAll()
{
	
}
void Game::resetAll()
{
}