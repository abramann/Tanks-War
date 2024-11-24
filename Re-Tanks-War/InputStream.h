#pragma once
#include "SystemComponent.h"

class IInputStream : public ISystemComponent
{
public:
	std::string getName() const { return "InputStream"; }

};

