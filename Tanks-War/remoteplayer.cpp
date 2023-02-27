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

void RemotePlayer::update(PlayerState playerState)
{
	setState(playerState);
	Tank::update(0);
}

void RemotePlayer::setState(PlayerState playerState)
{
	setX(playerState.x).setY(playerState.y).setAngle(playerState.angle);
	setHealth(playerState.health);
}
