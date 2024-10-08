#pragma once

#include "VecMath.h"
#include <stdint.h>



class IBuffer
{
public:
	virtual void readData(Vertex* pData, uint32_t size) = 0;
	virtual void writeData(Vertex* pData, uint32_t size) = 0;
	virtual void* getBufferObject() = 0;
};
