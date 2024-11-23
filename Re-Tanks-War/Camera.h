// camera.h
// Author: abramann

#pragma once

#include "MatMath.h"
#include <memory>

class CCamera
{
public:
	CCamera();
	void startup();
	void update(Vertex lookTo);
	void freeCamera();
	void updatePerspectiveMatrix();
	
private:
	Matrix m_proj, m_camera;
	float m_fov, m_aspectRatio,
		m_nearPlane, m_farPlane,
		m_z;
};

extern CCamera* g_pCamera;