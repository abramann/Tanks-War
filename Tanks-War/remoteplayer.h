#pragma once
#include "always.h"
#include "tank.h"

class RemotePlayer : public Tank
{
public:

	RemotePlayer();
	~RemotePlayer();

	virtual void initialize(Map* map, TextureManger* textureManger, Graphics* graphics);
	virtual void update(PlayerState playerState);

protected:

	void setState(PlayerState playerState);

	uint8_t id;

};

