#include "tank2.h"
#include "game.h"

constexpr auto HEALTH_TANK = 100;
constexpr auto VELOCITY_TANK = 10;
constexpr auto TEXTURE_TANK_ENEMY = 2;
const auto TANK_MAX_ANGLE = PI;

Tank2::Tank2()
{
	m_health = HEALTH_TANK;
	m_velocity = VELOCITY_TANK;
}

Tank2::~Tank2()
{
}

void Tank2::update(float frameTime)
{
	/*float width = m_width*1.0f, height = m_height*1.0f;
	Vertex v[VERTICES_IMAGE];
	v[0] = Vertex(-width, -height, 0.0f, 0.0f, -1.0f);
	v[1] = Vertex(-width, height, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(width, height, 0.0f, -1.0f, 0.0f);
	v[3] = Vertex(-width, -height, 0.0f, 0.0f, -1.0f);
	v[4] = Vertex(width, height, 0.0f, -1.0f, 0.0f);
	v[5] = Vertex(width, -height, 0.0f, -1.0f, -1.0f);

	m_pGraphics->setVertexBuffer(m_lpVertexBuffer, v, m_vertices);*/
	Object2::update(frameTime);
}

void Tank2::executeAttack()
{
}

void Tank2::executeBack(float frameTime)
{
	float x = m_position.x + (m_velocity * sin(m_rotate.z));
	m_position.x = m_pMap->passX(this, x);
	float y = m_position.y - m_velocity * cos(m_rotate.z);
	m_position.y = m_pMap->passY(this, y);
	Object2::executeForward();
}

void Tank2::executeDie()
{
	Object2::executeDie();
}

void Tank2::executeForward(float frameTime)
{
	m_position.x -= m_velocity * sin(m_rotate.z);
	m_position.y += m_velocity * cos(m_rotate.z);
	Object2::executeForward();
}

void Tank2::executeLeft(float frameTime)
{
	if (m_rotate.z >= (TANK_MAX_ANGLE - 0.01f))
		m_rotate.z *= -1;
	Sleep(200);

	m_rotate.z += TANK_ROTATE_AMOUNT;
	Object2::executeLeft();
}

void Tank2::executeRight(float frameTime)
{
	if (m_rotate.z <= -TANK_MAX_ANGLE)
		m_rotate.z *= -1;

	m_rotate.z -= TANK_ROTATE_AMOUNT;
	Sleep(200);
	Object2::executeRight();
}

void Tank2::playSoundForward()
{
}

void Tank2::playSoundBack()
{
}

void Tank2::playSoundRight()
{
}

void Tank2::playSoundLeft()
{
}

void Tank2::playSoundDie()
{
}