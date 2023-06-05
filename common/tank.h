// tank.h
// Author: abramann

#ifndef _TANK_H
#define _TANK_H
#include "object.h"
#include <memory>

class Game;
class Bullet;
class Timer;

class Tank : public Object
{
public:

	Tank();
	~Tank();
	virtual void initialize(Texture* texture, const Game* game);
	virtual void update();
	virtual void draw();
	virtual void executeAttack();
	virtual void executeBack();
	virtual void executeDie();
	virtual void executeForward();
	virtual void executeLeft();
	virtual void executeRight();

	float getBulletSpeed() const { return m_bulletSpeed; }
	float getBulletDamage() const { return m_bulletDamage; }
	bool isBulletLaunching() const { return (m_pBullet.size() > 0); }

protected:

	virtual void playSoundForward();
	virtual void playSoundBack();
	virtual void playSoundRight();
	virtual void playSoundLeft();
	virtual void playSoundDie();
	virtual void playSoundAttack();

	Sound m_soundAttack;
	float m_bulletSpeed, m_bulletDamage;
	float m_rotateAmount;
	std::vector<std::shared_ptr<Bullet>> m_pBullet;
	TextureManger* m_pTextureManger;
	Timer* m_pTimer;
};

#endif