#pragma once

#include "IGame.h"

typedef struct GameSettings_s
{
	int windWidth;
	int windHeight;
	bool windowed;
} GameSettings_t;

class CClient : public IGame
{
public:
	CClient();
	~CClient();

	void initialize() override;
	void run() override;
	void shutdown() override;
	bool pollMessages() override;
	LRESULT messageHandler(HWindow hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	int getWindowWidth() const override;
	int getWindowHeight() const  override;
	HWindow getWindowHandle() const override;
	bool isWindowed() const override;

private:
	void createWindow() override;
	void readInitializeSettings() override;

	GameSettings_t m_settings;
	HWindow m_hwnd;
};