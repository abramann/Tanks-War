// aiplayer.h
// Author: abramann

#pragma once

#include "player.h"

class Game;
class Object;
class Map;

class AIPlayer : public Player
{
public:

	AIPlayer();
	~AIPlayer();
	virtual void initialize(Game* pGame, PlayerID id, const std::string& name);
	virtual void update();
	std::vector<V3> getPath() const { return m_path; }
	virtual void executeAnimateRepeat();
	
private:

	void lookForEnemy();
	void moveTowardsObject(Object* pObject);
	void attackOjbect(Object* pObject);
	void rotateToward(const V3& vertex);
	bool moveToward(const V3& vertex);

	Object* m_pTargetObject;
	std::vector<V3> m_path;
	bool m_onMoving, m_onAttack;
	int64 m_lastSeachEnemyTime, m_lastShotTime;
};