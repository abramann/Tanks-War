// camera.h
// Author: abramann

#pragma once
#include "types.h"

class Game;
class Graphics;
class Map;

class Camera
{
public:

	Camera();
	~Camera();
	void initialize(const Game* game);
	void update(V3 lookTo);

private:

	Graphics* m_pGraphics;
	Map* m_pMap;
	Matrix m_proj, m_camera;
	float m_fov, m_aspectRation,
		m_nearPlane, m_farPlane,
		m_z;
};