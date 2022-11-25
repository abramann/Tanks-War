#include "unit.h"

Unit::Unit()
{
	audio = new Audio;
	health = 0;
	speed = 0;
	sinA = 0; cosA = 0;
	updateInput = false; playAudio = false;
	ZeroMemory(&key, sizeof(key));
	ZeroMemory(&effect, sizeof(effect));
}

Unit::~Unit()
{
	release();
}

void Unit::initialize(int _width, int _height, int extraWidth, int extraHeight, int _currentFrame, int _startFrame, int _endFrame, float _health, float _speed, TextureManger* _textureManger, Graphics* _graphics)
{
	Image::initialize(_width, _height, extraWidth, extraHeight, false, _currentFrame, _startFrame, _endFrame, NULL, _textureManger, _graphics);
	audio->initialize();
	setHealth(_health);
	setSpeed(_speed);
}

void Unit::inputInitialize(Input* _input, Key forward_key, Key back_key, Key right_key, Key left_key)
{
	key = new Key[4];
	updateInput = true;
	input = _input;
	key[FORWARD] = forward_key;
	key[BACK] = back_key;
	key[RIGHT] = right_key;
	key[LEFT] = left_key;
}

void Unit::audioInitialize(Effect forward_eff, Effect back_eff, Effect right_eff, Effect left_eff)
{
	effect = new Effect[4];
	playAudio = true;
	effect[FORWARD] = forward_eff;
	effect[BACK] = back_eff;
	effect[RIGHT] = right_eff;
	effect[LEFT] = left_eff;
}

void Unit::update(float frameTime)
{
	mathUpdate();
	if (updateInput)
		inputUpdate(frameTime);
	if(playAudio)
		audio->run();

	Image::update(frameTime);
}

void Unit::inputUpdate(float frameTime)
{
	if (input->isKeyIn(key[FORWARD]))
		executeForward(frameTime);
	if (input->isKeyIn(key[BACK]))
		executeBack(frameTime);
	if (input->isKeyIn(key[RIGHT]))
		executeRight(frameTime);
	if (input->isKeyIn(key[LEFT]))
		executeLeft(frameTime);
}

void Unit::executeForward(float frameTime)
{
	yAdd(speed * frameTime);
	audio->playCue(effect[FORWARD]);
}

void Unit::executeBack(float frameTime)
{
	yAdd(-(speed * frameTime));
	audio->playCue(effect[BACK]);
}

void Unit::executeRight(float frameTime)
{
	xAdd(speed * frameTime);
	audio->playCue(effect[RIGHT]);
}

void Unit::executeLeft(float frameTime)
{
	xAdd(-(speed * frameTime));
	audio->playCue(effect[LEFT]);
}

void Unit::release()
{
	SAFE_DELETE(audio);
	SAFE_DELETE_ARRAY(key);
	SAFE_DELETE_ARRAY(effect);
}

void Unit::mathUpdate()
{
	sinA = sin(getAngle());
	if (sinA > 1)
		sinA = 1;
	else if (sinA < -1)
		sinA = -1;
	else if (abs(sinA) < 0.0001)
		sinA = 0;
	cosA = cos(getAngle());
	if (cosA > 1)
		cosA = 1;
	else if (cosA < -1)
		cosA = -1;
	else if (abs(cosA) < 0.0001)
		cosA = 0;
}

