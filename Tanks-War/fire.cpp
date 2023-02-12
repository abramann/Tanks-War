#include "fire.h"
int qq = 0;
Fire::Fire()
{
}

Fire::~Fire()
{
}

void Fire::initialize(Map* _map, TextureManger* _textureManger, Graphics* _graphics)
{
	Unit::initialize(0, 0, 0, 0, false, 1, 1, 1, 0, _map, _textureManger, _graphics);
}
