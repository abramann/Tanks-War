// camera.cpp
// Author: abramann


#include "Camera.h"
#include "Map.h"
#include "Input.h"
#include "Values.h"
#include "KeyTable.h"
#include "Renderer.h"

static CCamera camera;
CCamera* g_pCamera = &camera;

CCamera::CCamera() :
	m_z(values::CAMERA_DEFAULT_HEIGHT),
	m_nearPlane(values::CAMERA_NEARPLANE),
	m_farPlane(values::CAMERA_FARPLANE),
	m_fov(values::CAMERA_FOV)
{
}

CCamera::~CCamera()
{
}

void CCamera::startup()
{
	updatePerspectiveMatrix();
}

void CCamera::update(Vertex lookTo)
{
	Vertex2D mapSize = g_pMap->getMapSizeInPixels();
	if (lookTo.x / m_aspectRatio <= abs(m_z))
		lookTo.x = abs(m_z)*m_aspectRatio;
	else if (lookTo.x > (mapSize.x - abs(m_z)*m_aspectRatio))
		lookTo.x = (mapSize.x - abs(m_z)*m_aspectRatio);

	if (lookTo.y <= abs(m_z))
		lookTo.y = abs(m_z);
	else if (lookTo.y > mapSize.y - abs(m_z))
		lookTo.y = mapSize.y - abs(m_z);

	auto px = round(lookTo.x * 10) / 10,
		py = round(lookTo.y * 10) / 10;
	Vertex position(px, py, m_z);
	Vertex target(lookTo.x, lookTo.y, lookTo.z);
	Vertex up(0.0f, 1.0f, 0.0f);
	Matrix eye;
	gameMathNS::matrixLookAtLH(&eye, &position, &target, &up);
	Matrix viewMatrix;
	gameMathNS::matrixIdentity(&viewMatrix);

	// zoom control
	if (g_pInput->isKeyDown(keyTable::F1_KEY))
		m_z += 5;
	else if (g_pInput->isKeyDown(keyTable::F2_KEY))
		m_z -= 5;
	else if (g_pInput->isKeyDown(keyTable::F3_KEY))
		m_z = values::CAMERA_DEFAULT_HEIGHT;

	if (m_z == 0)
		m_z = -1;

	// since the camera projection on the surface result Isosceles right triangle
	float maxZ = std::fmax(mapSize.x, mapSize.y) / 2;
	if (-m_z *m_aspectRatio > maxZ)
		m_z = -maxZ / m_aspectRatio;

	viewMatrix = eye * m_proj;
	g_pRenderer->setWorldViewMatrix(&viewMatrix);
}

void CCamera::freeCamera()
{
	//m_aspectRatio = 0;
	if (g_pInput->isKeyDown(keyTable::R_KEY))
		m_aspectRatio += 0.01f;
	static Vertex lookTo;
	if (g_pInput->isKeyDown(keyTable::A_KEY))
	{
		lookTo.x -= 10;
	}
	if (g_pInput->isKeyDown(keyTable::D_KEY))
	{
		lookTo.x += 10;
	}
	if (g_pInput->isKeyDown(keyTable::W_KEY))
	{
		lookTo.y += 10;
	}
	if (g_pInput->isKeyDown(keyTable::S_KEY))
	{
		lookTo.y -= 10;
	}

	// zoom control
	if (g_pInput->isKeyDown(keyTable::F_KEY))
		m_z += 5;
	else if (g_pInput->isKeyDown(keyTable::G_KEY))
		m_z -= 5;
	else if (g_pInput->isKeyDown(keyTable::H_KEY))
		m_z = values::CAMERA_DEFAULT_HEIGHT;
	if (m_z == 0)
		m_z = -1;

	Vertex2D mapSize = g_pMap->getMapSizeInPixels();
	if (lookTo.x / m_aspectRatio <= abs(m_z))
		lookTo.x = abs(m_z) * m_aspectRatio;
	else if (lookTo.x > (mapSize.x - abs(m_z) * m_aspectRatio))
		lookTo.x = (mapSize.x - abs(m_z) * m_aspectRatio);

	if (lookTo.y <= abs(m_z))
		lookTo.y = abs(m_z);
	else if (lookTo.y > mapSize.y - abs(m_z))
		lookTo.y = mapSize.y - abs(m_z);
		
	//auto px = round(lookTo.x * 10) / 10,
	//	py = round(lookTo.y * 10) / 10;
	static float pxp = 0, pyp = 0;

	float px = pxp + round(lookTo.x * 10) / 10,
			py = pyp + round(lookTo.y * 10) / 10;
	if (g_pInput->isKeyDown(keyTable::Q_KEY))
	{
		pxp += 5;
		pyp += 5;
	}
	else if (g_pInput->isKeyDown(keyTable::E_KEY))
	{
		pxp -= 5;
		pyp -= 5;
	}
	Vertex position(px, py, m_z);
	Vertex target(lookTo.x, lookTo.y, lookTo.z);
	Vertex up(0.0f, 1.0f, 0.0f);
	Matrix eye;
	gameMathNS::matrixLookAtLH(&eye, &position, &target, &up);
	Matrix viewMatrix;
	gameMathNS::matrixIdentity(&viewMatrix);

	viewMatrix = eye * m_proj;
	g_pRenderer->setWorldViewMatrix(&viewMatrix);
}

void CCamera::updatePerspectiveMatrix()
{
	float bbw = g_pRenderer->getBackbufferWidth();
	float bbh = g_pRenderer->getBackbufferHeight();
	m_aspectRatio = bbw / bbh;
	m_aspectRatio = 1;
	gameMathNS::matrixPerspectiveFovLH(&m_proj, m_fov, m_aspectRatio, m_nearPlane,
		m_farPlane);
}
