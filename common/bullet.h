//bullet.h
// Author: abamann

#ifndef _BULLET_H
#define _BULLET_H
#include "constants.h"
#include "image.h"

class Game;
class Tank;
class Map;
class Audio;
class TextureManger;

class Bullet : public Image
{
public:

	Bullet(const Game* game, const Tank * tank);
	Bullet();
	~Bullet();
	virtual void update(const float frameTime);
	void executeLaunch();
	void executeHit();
	bool isFinished() const { return m_finish; }

protected:

	virtual void executeAnimateRepeat();
	V3 Bullet::getBulletLaunchPosition();
	bool isCollided();

private:

	void executeLaunching(float frameTime);

	const Game* m_pGame;
	Map* m_pMap;
	Audio* m_pAudio;
	TextureManger* m_pTextureManger;
	const Tank* m_pTank;
	float m_speed, m_damage;
	bool m_hit, m_finish;
};


#endif