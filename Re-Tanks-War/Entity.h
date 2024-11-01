#pragma once

#include "SystemComponent.h"

class IEntity : ISystemComponent
{
public:
	virtual std::string getName() const { return "Entity"; }

	virtual bool isBumpable() = 0;
	virtual bool isMoveable() = 0;
	
private:
};