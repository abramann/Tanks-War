#pragma once
#include "player.h"

class ServerPlayer : public Player
{
public:

	ServerPlayer();
	~ServerPlayer();
	virtual void initialize(PlayerID id, const char* name, Game* game);
	
};

