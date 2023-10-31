// aiplayer.cpp
// Author: abramann

#include "aiplayer.h"
#include "map.h"
#include "game.h"
#include "gamemath.h"
#include "object.h"
#include "texturemanger.h"

AIPlayer::AIPlayer() : m_pTargetObject(nullptr)
{
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::initialize(Game * pGame, PlayerID id, const char * name)
{
	Player::initialize(id, name, PLAYER_ENEMY, pGame);
}

void AIPlayer::update()
{
	Player::update();
	if (m_health <= 0)
		return;

	if (m_pMap->isValidObject(m_pTargetObject))
	{
		moveTowardsObject(m_pTargetObject);
		if (m_onAttack)
			attackOjbect(m_pTargetObject);
	}
	else
	{
		lookForEnemy();
	}

	if (GetAsyncKeyState(VK_F12))
	{
		V3 start = m_pMap->findSpaceByVertex(getSpace().getCenter()).getCenter(),
			end = m_pMap->findSpaceByVertex(m_pTargetObject->getSpace().getCenter()).getCenter();
		m_path = m_pMap->pathfind(start, end);
	}
}

void AIPlayer::executeAnimateRepeat()
{
	m_path.clear();
	m_pTargetObject = nullptr;
	Player::executeAnimateRepeat();
}

void AIPlayer::lookForEnemy()
{
	m_pTargetObject = m_pMap->findClosestObject(getSpace().getCenter(), { this });
	if (!m_pTargetObject)
	{
		if (!m_path.empty())
			m_path.clear();

		return;
	}

	V3 start = m_pMap->findSpaceByVertex(getSpace().getCenter()).getCenter(),
		end = m_pMap->findSpaceByVertex(m_pTargetObject->getSpace().getCenter()).getCenter();
	m_path = m_pMap->pathfind(start, end);
	if (m_path.empty()) // The target object is not accessable
	{
		m_pTargetObject = nullptr;
		return;
	}

	m_onMoving = true;
}

void AIPlayer::moveTowardsObject(Object * pObject)
{
	if (m_path.empty())
	{
		if (!m_onAttack)
			m_pTargetObject = nullptr;

		return;
	}
	if (moveToward(m_path.back()))
	{
		if (!m_onAttack)
		{
			auto bulletWidth = m_pTextureManger->getTexture("bullet")->getWidth();
			Vector3D toTargetVector = Vector3D(getSpace().getCenter(), m_pTargetObject->getSpace().getCenter(), bulletWidth);
			if (m_pMap->isVectorUnderFreespace(toTargetVector, { this, m_pTargetObject }))
			{
				m_onMoving = false, m_onAttack = true;
				m_path.clear();
				return;
			}
		}
		if (m_path.empty())
			m_pTargetObject = nullptr;
		else
			m_path.pop_back();
	}
}

void AIPlayer::attackOjbect(Object * pObject)
{
	auto bulletWidth = m_pTextureManger->getTexture("bullet")->getWidth();
	Vector3D toTargetVector = Vector3D(getSpace().getCenter(), m_pTargetObject->getSpace().getCenter(), bulletWidth);
	if (!m_pMap->isVectorUnderFreespace(toTargetVector, { this, m_pTargetObject }))
	{
		m_onMoving = false, m_onAttack = false;
		m_path.clear();
		m_pTargetObject = nullptr;
	}
	else
	{
		rotateToward(pObject->getSpace().getCenter());
		executeAttack();
	}
}

void AIPlayer::rotateToward(const V3 & vertex)
{
	Vector3D toVertex = Vector3D(getSpace().getCenter(), vertex);
	float posiAngle = PI - gameMathNS::getAngle(toVertex);
	if (posiAngle > PI)
		m_rotate.z = -2 * PI + posiAngle;
	else
		m_rotate.z = posiAngle;
}

bool isSpaceInclude(const Space& space, const V3&  vertex)
{
	if (IN_RANGE(vertex.x, space.getMinX(), space.getMaxX()))
		if (IN_RANGE(vertex.y, space.getMinY(), space.getMaxY()))
			return true;

	return false;
}

bool AIPlayer::moveToward(const V3 & vertex)
{
	if (isSpaceInclude(getSpace(), vertex))
		return true;

	rotateToward(vertex);
	executeForward();
	return false;
}