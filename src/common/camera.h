// camera.h
// Author: abramann

#pragma once

#include "types.h"

class Game;
class Graphics;
class Map;
class Input;

class Camera
{
public:

	Camera();
	~Camera();
	void initialize(const Game* pGame);
	void update(V3 lookTo);
	void updatePerspectiveMatrix();

private:

	Graphics* m_pGraphics;
	Input* m_pInput;
	Map* m_pMap;
	Matrix m_proj, m_camera;
	float m_fov, m_aspectRatio,
		m_nearPlane, m_farPlane,
		m_z;
};