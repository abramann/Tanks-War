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

	Bullet(const Game* pGame, Tank * pTank);
	~Bullet();
	virtual void update();
	void executeLaunch();
	bool isFinished() const { return m_finish; }
	virtual V3 getRotateCenter() const;

protected:

	virtual void executeAnimateRepeat();
	void executeHit();
	void executeLaunching();
	V3 getBulletLaunchPosition();

	const Game* m_pGame;
	Map* m_pMap;
	Audio* m_pAudio;
	TextureManger* m_pTextureManger;
	Tank* m_pTank;
	float m_speed, m_damage;
	bool m_hit, m_finish;
};