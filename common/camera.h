#pragma once
#include "constants.h"

class Graphics;

class Camera
{
public:

	Camera();
	~Camera();
	void update(V3 lookTo);

private:

	Graphics* m_pGraphics;
};

