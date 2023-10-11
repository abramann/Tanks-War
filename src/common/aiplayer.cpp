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
	if (m_pTargetObject != nullptr)
	{
		if (GetAsyncKeyState(VK_F12))
		{
			V3 start = m_pMap->findSpaceByVertex(getSpace().getCenter()).getCenter(),
				end = m_pMap->findSpaceByVertex(m_pTargetObject->getSpace().getCenter()).getCenter();
			m_path = m_pMap->pathfind(start, end);
		}
		
		if (m_onMoving)
			moveTowardsObject(m_pTargetObject);
		if (m_onAttack)
			attackOjbect(m_pTargetObject);
	}
	else
		lookForEnemy();

	if (0)
	{
		m_path.clear();
		m_pTargetObject = nullptr;
	}

	Player::update();
}

void AIPlayer::lookForEnemy()
{
	m_pTargetObject = m_pMap->findClosestObject(getSpace().getCenter(), { this });
	if (m_pTargetObject)
		m_onMoving = true;
}

void AIPlayer::moveTowardsObject(Object * pObject)
{
	return;
	if (m_path.empty())
	{
		V3 start = m_pMap->findSpaceByVertex(getSpace().getCenter()).getCenter(),
			end = m_pMap->findSpaceByVertex(m_pTargetObject->getSpace().getCenter()).getCenter();
		m_path = m_pMap->pathfind(start, end);
		if (m_path.empty()) // The target object is not accessable
		{
			m_pTargetObject = nullptr;
			return;
		}
	}

	if (!m_onAttack)
	{
		auto bulletWidth = m_pTextureManger->getTexture("bullet")->getWidth();
		Vector3D toTargetVector = Vector3D(getSpace().getCenter(), m_pTargetObject->getSpace().getCenter(), bulletWidth);
		if (m_pMap->isVectorUnderFreespace(toTargetVector, { this, m_pTargetObject }))
			m_onMoving = false, m_onAttack = true;
	}

	if (moveToward(m_path.back()));
		//m_path.pop_back();

	if (m_path.empty())
		m_onMoving = false;
}

void AIPlayer::attackOjbect(Object * pObject)
{
}

void AIPlayer::rotateToward(const V3 & vertex)
{
	Vector3D toVertex = Vector3D(getSpace().getCenter(), vertex);
	float angle = gameMathNS::getAngle(toVertex);
	m_rotate.z = PI - angle;
}

bool AIPlayer::moveToward(const V3 & vertex)
{
	Space targetSpace = m_pMap->findSpaceByVertex(vertex);
	if (::areSpacesCollided(targetSpace, getSpace()))
		return true;

	rotateToward(vertex);
	//executeForward();
	return false;
}