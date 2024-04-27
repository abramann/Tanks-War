#pragma once

#include "VecMath.h"
#include <string>
#include <map>




namespace nsGameConfig
{
	const int MIN_WINDOW_HEIGHT = 600;
	const int MIN_WINDOW_WIDTH = 800;
	const int FRAME_RATE = 60;
	const float FRAME_TIME = 1000.0f / FRAME_RATE;
}

namespace nsGameLogic
{
	const float UPDATE_DELAY_IMAGE = 30.0f;
}

namespace nsTextureData
{
	inline Vertex getBodyCenter(const wchar_t* texFileName);
}