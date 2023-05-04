#pragma once

#include "image2.h"
#include "audio.h"

class Map2;

class Object2 : public Image2
{
public:

	Object2();
	~Object2();
	virtual void initialize(Texture* texture, const Game* game);
	virtual void update(const float frmaeTime);
	virtual void executeBack();
	virtual void executeDie();
	virtual void executeForward();
	virtual void executeLeft();
	virtual void executeRight();
	void addHealth(float addH) { m_health += addH; }
	void addVeclocity(float addV) { m_velocity += addV; }

protected:

	virtual void playSoundForward() = 0;
	virtual void playSoundBack() = 0;
	virtual void playSoundRight() = 0;
	virtual void playSoundLeft() = 0;
	virtual void playSoundDie() = 0;

	const Game* m_pGame;
	Audio* m_pAudio;
	Map2* m_pMap;
	float m_health, m_velocity;
	Sound m_soundForward, m_soundBack, m_soundRight, m_soundLeft;
	bool m_playSound;
};
