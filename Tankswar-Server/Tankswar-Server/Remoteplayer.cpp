#include "remoteplayer.h"

RemotePlayer::RemotePlayer()
{
}

RemotePlayer::~RemotePlayer()
{
}

void RemotePlayer::initialize(Map* map, TextureManger* textureManger, Graphics* graphics)
{
	Tank::initialize(map, textureManger, textureManger->getTexture(TEXTURES::ENEMY_TANK),
		graphics);
}

void RemotePlayer::update(float frameTime, PlayerState p1State)
{
	setPlayerState(p1State);
	Tank::update(frameTime);
}

void RemotePlayer::setPlayerState(PlayerState playerState)
{
	setX(playerState.x).setY(playerState.y).setAngle(playerState.angle);
	setHealth(playerState.health);
	//	setFireX();
}
