#pragma once
#include "always.h"
#include "tank.h"

class RemotePlayer : public Tank
{
public:

	RemotePlayer();
	~RemotePlayer();

	virtual void initialize(Map* map, TextureManger* textureManger, Graphics* graphics);
	virtual void update();
	virtual void update(PlayerState playerState);
	virtual void update(ServerToPlayer serverToPlayer);
	void setPlayerId(uint8_t id) { m_id = id; }

	uint8_t m_id;

protected:

	void setState(PlayerState playerState);


};

