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
	virtual void initialize(PlayerID id, const char* name, PlayerType playerType, const Game * pGame);
	void applyPlayerUpdate(const PlayerUpdate playerUpdate);
	PlayerUpdate getPlayerUpdate() const;
	PlayerID getID() const { return m_id; }
	void setID(PlayerID id) { m_id = id; }
	const char* getName() const { return m_name; }

protected:

	PlayerID m_id;
	char m_name[gameNS::MAX_NAME_LEN];
};