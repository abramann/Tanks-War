#pragma once

constexpr float PI = 3.141592653589f;


// Which renderer use(Vulkan in future)
#define RENDER_CLASS CDxRenderer

namespace values
{
	// General values
	constexpr auto GAME_NAME = "Tanks-War";
	constexpr auto MIN_WINDOW_HEIGHT = 600;
	constexpr auto MIN_WINDOW_WIDTH = 800;
	constexpr auto FRAME_RATE = 60;
	constexpr float FRAME_TIME = 1000.0f / FRAME_RATE;
	constexpr auto USED_FONT = "Perfect DOS VGA";

	// Camera values
	constexpr auto CAMERA_FARPLANE = 10000.0f;
	constexpr auto CAMERA_FOV = 1.5707963f;
	constexpr auto CAMERA_NEARPLANE = 1.0f;
	constexpr auto PROJECT_FAR_PLANE = 1000.0f;
	constexpr auto PROJECT_FOV = PI / 2;
	constexpr auto PROJECT_NEAR_PLANE = 1.0f;
	constexpr auto CAMERA_DEFAULT_HEIGHT = -300.0f;

	// Map values
	constexpr unsigned int UNDEFINED_POSITION = 0xFFFF;
	constexpr int32_t TILED_DIMENSION = 100;
	constexpr uint32_t CELL_VERTICES_NUM = 4;

	// Directories values
	constexpr auto MAP_INITIALIZE_FILE = "Assets\\Map.ini";
	constexpr auto MAPS_DIR = "Assets\\Maps\\";
	constexpr auto MODELS_DIR = "Assets\\Models\\";
	constexpr auto FONTS_DIR = "Assets\\Fonts\\";
}
