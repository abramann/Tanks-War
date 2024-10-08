#pragma once

class ISystem
{
public:
	virtual void startup() = 0;
	virtual void update() = 0;
	virtual void reset() = 0;
};