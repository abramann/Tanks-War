// tank.h
// Author: abramann

#pragma once

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
	virtual void initialize(std::string texture, const Game * pGame);
	virtual void update();
	virtual void draw();
	virtual bool executeAttack();
	virtual void executeBack();
	virtual void executeDie();
	virtual void executeForward();
	virtual void executeLeft();
	virtual void executeRight();
	virtual void executeAnimateRepeat();
	float getBulletSpeed() const { return m_bulletSpeed; }
	float getBulletDamage() const { return m_bulletDamage; }
	float getInflictedDamage() const { return m_inflictedDamage; }
	bool isBulletLaunching() const { return (m_pBullet.size() > 0); }
	void addInflictedDamage(const float& infDmg) { m_inflictedDamage += infDmg; }
	int32 getMadeKills() const { return m_madeKills; }
	void addMadeKills(int32 newMadeKills) { m_madeKills += newMadeKills; }

protected:

	virtual void playSoundForward();
	virtual void playSoundBack();
	virtual void playSoundRight();
	virtual void playSoundLeft();
	virtual void playSoundDie();
	virtual void playSoundAttack();

	Sound m_soundAttack;
	float m_bulletSpeed, m_bulletDamage, m_rotateAmount;
	float m_inflictedDamage;
	std::string m_tankTexture;
	std::vector<std::shared_ptr<Bullet>> m_pBullet;
	Timer* m_pTimer;
	int32 m_madeKills;
};