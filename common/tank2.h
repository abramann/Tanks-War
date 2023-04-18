#pragma once
#include "object2.h"

class Game;

class Tank2 : public Object2
{
public:

	Tank2();
	~Tank2();
	virtual void update(float frameTime);

	virtual void executeAttack();
	virtual void executeBack(float frameTime);
	virtual void executeDie();
	virtual void executeForward(float frameTime);
	virtual void executeLeft(float frameTime);
	virtual void executeRight(float frameTime);

protected:

	virtual void playSoundForward();
	virtual void playSoundBack();
	virtual void playSoundRight();
	virtual void playSoundLeft();
	virtual void playSoundDie();

	Sound m_soundAttack;
};