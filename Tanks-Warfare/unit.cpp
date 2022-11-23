#include "unit.h"

Unit::Unit()
{
	audio = new Audio;
	health = UNIT_HEALTH;
	speed = UNIT_SPEED;
	sinA = 0; cosA = 0;
	updateInput = false;
	Image::Image();
}

Unit::~Unit()
{
	Image::~Image();
}

void Unit::initialize(int _width, int _height, int extraWidth, int extraHeight, int _currentFrame, int _startFrame, int _endFrame, float _health, float _speed, TextureManger* _textureManger, Graphics* _graphics)
{
	Image::initialize(_width, _height, extraWidth, extraHeight, false, _currentFrame, _startFrame, _endFrame, NULL, textureManger, _graphics);
	setHealth(_health);
	setSpeed(_speed);
}

void Unit::inputInitialize(Input* _input, KEY forward_key, KEY back_key, KEY right_key, KEY left_key)
{
	updateInput = true;
	input = _input;
	key[FORWARD] = forward_key;
	key[BACK] = back_key;
	key[RIGHT] = right_key;
	key[LEFT] = left_key;
}

void Unit::update(float frameTime)
{
	mathUpdate();
	if (updateInput)
		inputUpdate(frameTime);

	Image::update(frameTime);
}

void Unit::inputUpdate(float frameTime)
{
	if (input->isKeyIn(key[FORWARD]))
		executeForward(frameTime);
	if (input->isKeyIn(key[BACK]))
		executeBack(frameTime);
	if (input->isKeyIn(key[RIGHT]))
		executeBack(frameTime);
	if (input->isKeyIn(key[LEFT]))
		executeLeft(frameTime);
}

void Unit::executeForward(float frameTime)
{
	xAdd(speed * frameTime);
}

void Unit::executeBack(float frameTime)
{
	xAdd(-(speed * frameTime));
}

void Unit::executeRight(float frameTime)
{
	yAdd(speed * frameTime);
}

void Unit::executeLeft(float frameTime)
{
	yAdd(-(speed * frameTime));
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

