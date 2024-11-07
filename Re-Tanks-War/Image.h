#pragma once

#include "Widget.h"

class CImage : public IWidget
{
	virtual void draw() = 0;
	virtual void reset() {}

};

