#pragma once

#include "KeyTable.h"
#include <memory>

class CInput
{
public:
	CInput();
	~CInput();

	void initialize();
	bool isKeyDown(Key key) const;
	bool isKeyPressed(Key key) const;
	bool isKeyReleased(Key key) const;
};

extern CInput* g_pInput;