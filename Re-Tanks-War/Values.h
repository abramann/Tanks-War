#pragma once

constexpr float PI = 3.141592653589f;


// Which renderer use(Add Vulkan in future)
#define RENDER_CLASS CDxRenderer

namespace values
{
	// General values
	constexpr auto GAME_NAME = "Tanks-War";
	constexpr int MIN_WINDOW_HEIGHT = 600;
	constexpr int MIN_WINDOW_WIDTH = 800;
	constexpr int FRAME_RATE = 60;
	constexpr float FRAME_TIME = 1000.0f / FRAME_RATE;
	
	// Camera values
	constexpr auto CAMERA_FARPLANE = 10000.0f;
	constexpr auto CAMERA_FOV = 1.5707963f;
	constexpr auto CAMERA_NEARPLANE = 1.0f;
	constexpr auto PROJECT_FAR_PLANE = 1000.0f;
	constexpr auto PROJECT_FOV = PI / 2;
	constexpr auto PROJECT_NEAR_PLANE = 1.0f;
	constexpr auto FADE_INC_RATE = 0.01f;
	constexpr auto CAMERA_DEFAULT_HEIGHT = -300.0f;

	// Map values
	constexpr unsigned int UNDEFINED_POSITION = 0xFFFF;
	constexpr int32_t TILED_DIMENSION = 100;
	constexpr uint32_t CELL_VERTICES_NUM = 4;

	// Directories values
	constexpr auto MAP_INITIALIZE = "Assets\\Map.ini";
	constexpr auto MAP_DIR = "Assets\\Maps\\";
	constexpr auto MODEL_DIR = "Assets\\Models\\";
	constexpr auto FONT_PATH = "Assets\\Fonts\\Tahoma.ttf";
}
