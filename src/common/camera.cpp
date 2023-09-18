// camera.cpp
// Author: abramann

#include "camera.h"
#include "game.h"
#include "map.h"
#include "gamemath.h"
#include "input.h"

Camera::Camera() : m_pGraphics(0), m_z(logicNS::CAMERA_HEIGHT), m_nearPlane(graphicsNS::CAMERA_NEARPLANE), m_farPlane(graphicsNS::CAMERA_FARPLANE),
m_fov(graphicsNS::CAMERA_FOV)
{
}

Camera::~Camera()
{
}

void Camera::initialize(const Game* pGame)
{
	m_pGraphics = pGame->getGraphics();
	m_pMap = pGame->getMap();
	m_pInput = pGame->getInput();
	updatePerspectiveMatrix();
}

void Camera::update(V3 lookTo)
{	
	V2 mapSize = m_pMap->getMapSize();
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
	V3 position(px, py, m_z);
	V3 target(lookTo.x, lookTo.y, lookTo.z);
	V3 up(0.0f, 1.0f, 0.0f);
	Matrix eye;
	gameMathNS::matrixLookAtLH(&eye, &position, &target, &up);
	Matrix viewMatrix;
	gameMathNS::matrixIdentity(&viewMatrix);
	
	// zoom control
	if (m_pInput->isKeyDown(inputNS::F1_KEY))
		m_z++;
	else if (m_pInput->isKeyDown(inputNS::F2_KEY))
		m_z--;
	else if (m_pInput->isKeyDown(inputNS::F3_KEY))
		m_z = logicNS::CAMERA_HEIGHT;

	float maxZ = max(mapSize.x, mapSize.y) / 2;
	if (-m_z *m_aspectRatio > maxZ)
		m_z = -maxZ / m_aspectRatio;

	viewMatrix = eye*m_proj;
	m_pGraphics->setWorldViewMatrix(&viewMatrix);
}

void Camera::updatePerspectiveMatrix()
{
	m_aspectRatio = g_pGameSettings->width*1.0f / g_pGameSettings->height*1.0f;
	gameMathNS::matrixPerspectiveFovLH(&m_proj, m_fov, m_aspectRatio, m_nearPlane,
		m_farPlane);
}