#ifndef _FIRE_H
#define _FIRE_H
#include "always.h"
#include "image.h"

class Fire : public Image
{
public:

	Fire();
	~Fire();
	virtual void initialize(TextureManger* _textureManger, Graphics* _graphics);

private:

};


#endif