#include "remoteplayer.h"
/*
RemotePlayer::RemotePlayer()
{
}

RemotePlayer::~RemotePlayer()
{
}

void RemotePlayer::initialize(Map* map, TextureManger* textureManger, Graphics* graphics)
{
	Tank::initialize(map, textureManger, textureManger->getTexture(TEXTURES::PLAYER_TANK),
		graphics);
}

void RemotePlayer::update(float frameTime)
{
	Tank::update(frameTime);
}

void RemotePlayer::applyPlayerToServer(PlayerToServer pts)
{
	
}

PlayerState RemotePlayer::getPlayerState() const
{
	PlayerState state;
	state.angle = getAngle();
	state.health = getHealth();
	state.x = getX();
	state.y = getY();
	return state;
}

void RemotePlayer::applyPlayerToServer(PlayerToServer pts, float frameTime)
{
	if (pts.forward)
		executeForward(frameTime);
	if (pts.back)
		executeBack(frameTime);
	if (pts.right)
		executeRight(frameTime);
	if (pts.left)
		executeLeft(frameTime);
	if (pts.attack)
		executeAttack(frameTime);
}

void RemotePlayer::setPlayerState(PlayerState playerState)
{
	setX(playerState.x).setY(playerState.y).setAngle(playerState.angle);
	setHealth(playerState.health);
	//	setFireX();
}
*/