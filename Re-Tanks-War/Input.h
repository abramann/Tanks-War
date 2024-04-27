#pragma once

#include "keys.h"




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
