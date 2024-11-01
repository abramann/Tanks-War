#pragma once

#include "VecMath.h"
#include <string>
#include <map>




namespace nsGameConfig
{
	constexpr int MIN_WINDOW_HEIGHT = 600;
	constexpr int MIN_WINDOW_WIDTH = 800;
	constexpr int FRAME_RATE = 60;
	constexpr float FRAME_TIME = 1000.0f / FRAME_RATE;
}

namespace nsGameLogic
{

}

namespace mapNS
{
	constexpr  unsigned int UNDEFINED_POSITION = 0xFFFF;
	constexpr int TILED_DIMENSION = 100;
	constexpr unsigned int CELL_VERTICES_NUM = 4;
}

namespace fileNS
{
	constexpr auto MAP_INITIALIZE = "Assets\\Map.ini";
	constexpr auto MAP_DIR = "Assets\\Maps\\";
	constexpr auto MODEL_DIR = "Assets\\Models\\";
	constexpr auto FONT_PATH = "Assets\\Fonts\\Tahoma.ttf";
}