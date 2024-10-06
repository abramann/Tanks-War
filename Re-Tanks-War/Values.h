#pragma once

constexpr float PI = 3.141592653589f;

#define RENDER_CLASS CDxRenderer

namespace logicNS
{
}

namespace cameraNS
{
	constexpr auto CAMERA_FARPLANE = 10000.0f;
	constexpr auto CAMERA_FOV = 1.5707963f;
	constexpr auto CAMERA_NEARPLANE = 1.0f;
	constexpr auto PROJECT_FAR_PLANE = 1000.0f;
	constexpr auto PROJECT_FOV = PI / 2;
	constexpr auto PROJECT_NEAR_PLANE = 1.0f;
	constexpr auto FADE_INC_RATE = 0.01f;
	constexpr auto CAMERA_DEFAULT_HEIGHT = -300.0f;

}