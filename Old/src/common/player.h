// player.h
// Author: abramann

#pragma once

#include "tank.h"

enum PlayerType
{
	PLAYER_SELF,
	PLAYER_ENEMY
};

class Player : public Tank
{
public:

	Player();
	~Player();
	virtual void initialize(PlayerID id, const std::string& name, PlayerType playerType, const Game * pGame);
	PlayerID getID() const { return m_id; }
	void setID(PlayerID id) { m_id = id; }
	const char* getName() const { return m_name; }
	virtual void damage(float dmg);
	virtual void executeDie();
	virtual void executeAnimateRepeat();
	virtual void reset();

#ifdef _CLIENT_BUILD
	void setClientGameStatus(const ClientGameStatus& clientGameStatus);
	void setClientGameAttribute(const ClientGameAttribute& clientGameAttribute);
#endif
protected:

	PlayerID m_id;
	char m_name[gameNS::MAX_NAME_LEN];
};