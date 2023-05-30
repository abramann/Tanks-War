#pragma once
#include "constants.h"

class Game;
class Graphics;
class Map2;

constexpr auto CAMERA_NEARPLANE = 1.0f;
constexpr auto CAMERA_FARPLANE = 1000.0f;
constexpr auto CAMERA_FOV = 1.5707963f;
constexpr auto CAMERA_HEIGHT = -300;

class Camera
{
public:

	Camera();
	~Camera();
	void initialize(const Game* game);
	void update(V3 lookTo);

private:

	Graphics* m_pGraphics;
	Map2* m_pMap;
	Matrix m_proj, m_camera;
	float m_fov, m_aspectRation,
		m_nearPlane, m_farPlane,
		m_z;

};

