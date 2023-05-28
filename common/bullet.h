#pragma once
#include "constants.h"
#include "image2.h"

class Game;
class Tank2;
class Map2;
class Audio;
class TextureManger;

class Bullet : public Image2
{
public:
	
	Bullet(const Game* game, const Tank2 * tank);
	Bullet();
	~Bullet();
	virtual void update(float frameTime);
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
	Map2* m_pMap;
	Audio* m_pAudio;
	TextureManger* m_pTextureManger;
	const Tank2* m_pTank;
	float m_speed, m_damage;
	bool m_hit, m_finish;
};

