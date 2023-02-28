#ifndef _PLAYER_H
#define _PLAYER_H
#include "always.h"
#include "remoteplayer.h"
#include <vector>

class Player : public RemotePlayer
{

public:

	Player();
	~Player();
	virtual void initialize(Map* map, TextureManger* textureManger, Input* input, Graphics* graphics);
	virtual void update();
	virtual void update(PlayerState playerState);
	virtual void update(ServerToPlayer serverToPlayer);
	PlayerToServer getPlayerToServer() const { return m_toServer; }
//	void setPlayerInfo(PlayerInfo playerInfo) { m_playerInfo = playerInfo; }
	bool m_event;

private:

	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);
	virtual void executeAttack(float frameTime);

	PlayerToServer m_toServer;
	
};


#endif