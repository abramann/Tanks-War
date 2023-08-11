// camera.cpp
// Author: abramann

#include "camera.h"
#include "game.h"
#include "map.h"
#include "gamemath.h"

Camera::Camera() : m_pGraphics(0), m_z(graphicsNS::CAMERA_HEIGHT), m_nearPlane(graphicsNS::CAMERA_NEARPLANE), m_farPlane(graphicsNS::CAMERA_FARPLANE),
m_fov(graphicsNS::CAMERA_FOV)
{
}

Camera::~Camera()
{
}

void Camera::initialize(const Game* game)
{
	m_aspectRation = g_gameInfo.width*1.0f / g_gameInfo.height*1.0f;
	m_pGraphics = game->getGraphics();
	m_pMap = game->getMap();
	gameMathNS::matrixPerspectiveFovLH(&m_proj, m_fov, m_aspectRation, m_nearPlane,
		m_farPlane);
#ifdef _BUILD_WITH_D3D9
	LPDevice lpDevice3d = m_pGraphics->getDevice();
	lpDevice3d->SetTransform(D3DTS_PROJECTION, &m_proj);
#else ifdef _BUILD_WITH_D3D11
#endif
}

void Camera::update(V3 lookTo)
{
	V2 mapSize = m_pMap->getMapSize();
	if (lookTo.x / m_aspectRation <= abs(m_z))
		lookTo.x = abs(m_z)*m_aspectRation;
	else if (lookTo.x > (mapSize.x - abs(m_z)*m_aspectRation))
		lookTo.x = (mapSize.x - abs(m_z)*m_aspectRation);

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
#ifdef _BUILD_WITH_D3D9
	viewMatrix = eye;
#else ifdef _BUILD_WITH_D3D11
	viewMatrix = viewMatrix*eye*m_proj;
#endif
	m_pGraphics->setViewMatrix(&viewMatrix);
}