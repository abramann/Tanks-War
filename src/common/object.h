// object.h
// Author: abramann

#pragma once

#include "image.h"
#include "audio.h"

class Map;

class Object : public Image
{
public:

	Object();
	~Object();
	virtual void initialize(std::string texture, const Game * pGame);
	virtual void update();
	virtual void executeBack();
	virtual void executeDie();
	virtual void executeForward();
	virtual void executeLeft();
	virtual void executeRight();
	virtual void damage(float dmg);
	void addVeclocity(float addV) { m_velocity += addV; }
	float getHealth() const { return m_health; }
	float getVelocity() const { return m_velocity; }
	void setHealth(float health) { m_health = health; }

protected:

	virtual void playSoundForward() = 0;
	virtual void playSoundBack() = 0;
	virtual void playSoundRight() = 0;
	virtual void playSoundLeft() = 0;
	virtual void playSoundDie() = 0;

	const Game* m_pGame;
	Audio* m_pAudio;
	Map* m_pMap;
	float m_health, m_velocity;
	Sound m_soundForward, m_soundBack, m_soundRight, m_soundLeft;
};