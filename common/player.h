#pragma once
#include "tank2.h"

class Player : public Tank2
{
public:

	Player();
	~Player();
	virtual void initialize(PlayerID id, const char* name, Texture* texture, const Game * game);
	void applyPlayerUpdate(const PlayerUpdate playerUpdate);
	PlayerUpdate getPlayerUpdate() const;

protected:

	PlayerID m_id;
	char m_name[MAX_NAME_LEN];
};

