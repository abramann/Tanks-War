#pragma once
#include "constants.h"
#include "tank.h"

class RemotePlayer : public Tank
{
public:

	RemotePlayer();
	~RemotePlayer();

	virtual void initialize(Map* map, TextureManger* textureManger, Graphics* graphics);
	virtual void update(float frameTime);
	void applyPlayerToServer(PlayerToServer pts);
	PlayerState getPlayerState() const;
	void applyPlayerToServer(PlayerToServer pts, float frameTime);
	void setPlayerState(PlayerState playerState);

};

