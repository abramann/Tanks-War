#ifndef _PLAYER_H
#define _PLAYER_H
#include "always.h"
#include "tank.h"
#include <vector>

enum PLAYER_STATE
{
	PLAYER_IDLE,
	PLAYER_MOVE,
	PLAYER_NITROMOVE,
	PLAYER_ATTACK,
	PLAYER_DIE
};

class Player : public Tank
{

public:

	Player();
	~Player();
	virtual void initialize(Map* map, TextureManger* textureManger, Input* input, Graphics* graphics);
	virtual void update(float frameTime, PlayerState p0State);
	virtual void draw();
	virtual PlayerToServer getPlayerToServer() const { return m_toServer; }
	void setPlayerState(PlayerState playerState);

private:

	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);
	virtual void executeAttack(float frameTime);

	PlayerToServer m_toServer;

	
};


#endif