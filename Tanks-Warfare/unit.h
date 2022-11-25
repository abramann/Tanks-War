#ifndef _UNIT_H
#define _UNIT_H

constexpr auto MAX_CONTROL = 10;
constexpr auto FORWARD = 0;
constexpr auto BACK = 1;
constexpr auto RIGHT = 2;
constexpr auto LEFT = 3;

const int UNIT_HEALTH = 100;
const int UNIT_SPEED = 10;

#include "always.h"
#include "image.h"
#include "audio.h"
#include "input.h"

class Unit : public Image
{
public:

	Unit();
	~Unit();
	virtual void initialize(int _width, int _height, int extraWidth, int extraHeight, int currentFrame, int startFrame, int endFrame, float health, float speed, TextureManger* _textureManger, Graphics* _graphics);
	virtual void inputInitialize(Input* _input, Key forward_key, Key back_key, Key right_key, Key left_key);
	virtual void audioInitialize(Effect _forward_eff, Effect back_eff, Effect right_eff, Effect left_eff);
	virtual void update(float frameTime);
	virtual void inputUpdate(float frameTime);
	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);
	virtual void release();

	virtual void setHealth(float _health) { health = _health; }
	virtual void setSpeed(float _speed) { speed = _speed; }
	virtual void setUpdateInput(bool _updateInput) { updateInput = _updateInput; }
	virtual void setPlayAudio(bool _playAudio) { playAudio = _playAudio; }

	float getAngleSin() { return sinA; }
	float getAngleCos() { return cosA; }
	virtual float getHealth() { return health; }
	virtual float getSpeed() { return speed; }
	virtual bool getUpdateInput() { return updateInput; }
	virtual bool getPlayAudio() { return playAudio; }

private:

	void mathUpdate();

	float sinA, cosA;

protected:
	
	Audio* audio;
	Input* input;

	Key key[MAX_CONTROL];	
	Effect effect[MAX_CONTROL];
	float health, speed;
	bool updateInput, playAudio;

};


#endif