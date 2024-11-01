// camera.cpp
// Author: abramann


#include "Camera.h"
#include "Map.h"
#include "Input.h"
#include "Values.h"
#include "keys.h"
#include "Renderer.h"

std::shared_ptr<CCamera> g_pCamera;

CCamera::CCamera() :m_z(cameraNS::CAMERA_DEFAULT_HEIGHT), m_nearPlane(cameraNS::CAMERA_NEARPLANE), m_farPlane(cameraNS::CAMERA_FARPLANE),
m_fov(cameraNS::CAMERA_FOV)
{
	startup();
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
	if (g_pInput->isKeyDown(inputNS::F1_KEY))
		m_z += 5;
	else if (g_pInput->isKeyDown(inputNS::F2_KEY))
		m_z -= 5;
	else if (g_pInput->isKeyDown(inputNS::F3_KEY))
		m_z = cameraNS::CAMERA_DEFAULT_HEIGHT;

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
	if (g_pInput->isKeyDown(inputNS::R_KEY))
		m_aspectRatio += 0.01f;
	static Vertex lookTo;
	if (g_pInput->isKeyDown(inputNS::A_KEY))
	{
		lookTo.x -= 10;
	}
	if (g_pInput->isKeyDown(inputNS::D_KEY))
	{
		lookTo.x += 10;
	}
	if (g_pInput->isKeyDown(inputNS::W_KEY))
	{
		lookTo.y += 10;
	}
	if (g_pInput->isKeyDown(inputNS::S_KEY))
	{
		lookTo.y -= 10;
	}
	// zoom control
	if (g_pInput->isKeyDown(inputNS::F_KEY))
		m_z += 5;
	else if (g_pInput->isKeyDown(inputNS::G_KEY))
		m_z -= 5;
	else if (g_pInput->isKeyDown(inputNS::H_KEY))
		m_z = cameraNS::CAMERA_DEFAULT_HEIGHT;
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
	if (g_pInput->isKeyDown(inputNS::Q_KEY))
	{
		pxp += 5;
		pyp += 5;
	}
	else if (g_pInput->isKeyDown(inputNS::E_KEY))
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
