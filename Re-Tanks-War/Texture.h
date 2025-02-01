#pragma once

class ITexture
{
public:
	virtual ~ITexture() {};

	virtual void* getResourceObject() const = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
};

