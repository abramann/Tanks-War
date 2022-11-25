#include "tank.h"

Tank::Tank()
{
}

Tank::~Tank()
{
}

void Tank::initialize(int _width, int _height, float _health, float _speed, TextureManger* _textureManger, Graphics* _graphics)
{
	Unit::initialize(_width, _height, 0, 0, TANK_CURRENTFRAME, TANK_STARTFRAME, TANK_ENDFRAME, _health, _speed, _textureManger, _graphics);
}
