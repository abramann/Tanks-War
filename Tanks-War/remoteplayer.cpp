#include "remoteplayer.h"

RemotePlayer::RemotePlayer()
{
}

RemotePlayer::~RemotePlayer()
{
}

void RemotePlayer::initialize(PlayerInfo playerInfo, Map* map, TextureManger* textureManger, Graphics* graphics)
{
	setPlayerInfo(playerInfo);
	Tank::initialize(map, textureManger, textureManger->getTexture(TEXTURES::ENEMY_TANK),
		graphics);
}

void RemotePlayer::update()
{
	Tank::update(0);
}

void RemotePlayer::update(PlayerState playerState)
{
	setState(playerState);
	Tank::update(0);
}

void RemotePlayer::update(ServerToPlayer serverToPlayer)
{
	Tank::update(0);
}

void RemotePlayer::setState(PlayerState playerState)
{
	setX(playerState.x).setY(playerState.y).setAngle(playerState.angle);
	setHealth(playerState.health);
	if (playerState.attack)
		executeAttack(0);
}
