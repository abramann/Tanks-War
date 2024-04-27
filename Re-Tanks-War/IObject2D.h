#pragma once

#include "VecMath.h"


class IBuffer;

class IObject2D
{
public:
	IObject2D();
	virtual ~IObject2D();

	virtual void setPosition(Vertex newPosition);
	virtual Vertex getPosition() const;
	virtual IBuffer* getVertexBuffer() const;
	virtual IBuffer* getIndexBuffer() const;

protected:
	virtual void createVertexBuffer() = 0;
	virtual void createIndexBuffer() = 0;
	virtual void updateVertexBuffer() = 0;

	Vertex m_position;
	IBuffer* m_pVertexBuffer;
	IBuffer* m_pIndexBuffer;
};