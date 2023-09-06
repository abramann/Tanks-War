// bullet.h
// Author: abamann

#pragma once

#include "types.h"
#include "image.h"

class Game;
class Tank;
class Map;
class Audio;
class TextureManger;

class Bullet : public Image
{
public:

	Bullet(const Game* pGame, const Tank * pTank);
	Bullet();
	~Bullet();
	virtual void update();
	void executeLaunch();
	void executeHit();
	bool isFinished() const { return m_finish; }

protected:

	virtual void executeAnimateRepeat();
	V3 Bullet::getBulletLaunchPosition();
	void executeLaunching();

	const Game* m_pGame;
	Map* m_pMap;
	Audio* m_pAudio;
	TextureManger* m_pTextureManger;
	const Tank* m_pTank;
	float m_speed, m_damage;
	bool m_hit, m_finish;
};