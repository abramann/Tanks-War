#include "camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::update(V3 lookTo)
{
	Matrix proj, cam;
	D3DXMatrixPerspectiveFovLH(
		&proj, PI * 0.5f, // 90 - degree
		(float)1.333F,
		1.0f,
		1000.0f);

	
	V3 position = lookTo;
	position.z = m_z;
	V3 target = lookTo;
	V3 up = V3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cam, &position, &target, &up);
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	world = world *cam*proj;
	
}
