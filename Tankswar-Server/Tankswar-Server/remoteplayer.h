#pragma once
#include "player.h"

class RemotePlayer : public Tank
{
public:

	RemotePlayer();
	~RemotePlayer();

	virtual void initialize(Map* map, TextureManger* textureManger, Graphics* graphics);
	virtual void update(float frameTime, PlayerState p1State);
	void setPlayerState(PlayerState playerState);

};

