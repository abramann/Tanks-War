#pragma once

#include "keys.h"
#include <memory>



class CInput
{
public:
	CInput();
	~CInput();

	void startUp();
	void handle();
	bool isKeyDown(Key key) const;
	bool isKeyPressed(Key key) const;
	bool isKeyReleased(Key key) const;
};

extern std::shared_ptr<CInput> g_pInput;