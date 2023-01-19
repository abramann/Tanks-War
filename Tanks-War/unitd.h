#ifndef _UNITD_H
#define _UNITD_H

#include "unit.h"

class Unitd :public Unit
{
public:

	Unitd();
	~Unitd();

private:

	Image death;
	int deathColumns, deathRows;

};


#endif