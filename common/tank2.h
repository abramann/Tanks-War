#pragma once
#include "object2.h"

class Game;
class Bullet;

constexpr auto BULLET_SPEED = 10;
constexpr auto BULLET_DAMAGE = 40;
class Tank2 : public Object2
{
public:

	Tank2();
	~Tank2();
	virtual void update(float frameTime);
	virtual void draw();

	virtual void executeAttack();
	virtual void executeBack(float frameTime);
	virtual void executeDie();
	virtual void executeForward(float frameTime);
	virtual void executeLeft(float frameTime);
	virtual void executeRight(float frameTime);

	float getBulletSpeed() const { return m_bulletSpeed; }
	float getBulletDamage() const { return m_bulletDamage; }

protected:

	virtual void playSoundForward();
	virtual void playSoundBack();
	virtual void playSoundRight();
	virtual void playSoundLeft();
	virtual void playSoundDie();

	Sound m_soundAttack;
	float m_bulletSpeed, m_bulletDamage;
	std::vector<Bullet*> m_pBullet;
	TextureManger* m_pTextureManger;
};