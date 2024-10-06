#pragma once

#include "GameData.h"
#include "Math.h"
#include <iostream>




class ITexture;

class CSprite
{
public:
	CSprite(const wchar_t* texFileName);
	virtual ~CSprite();

public:
	void setPosition(Vertex position);
	Vertex getPosition() const;
	virtual void setScaling(Vertex newScaling);
	virtual Vertex getScaling() const;
	virtual void makeRotate(float newRotate);
	virtual void setRotate(float newRotate);
	virtual float getRotate() const;
	virtual void setColor(Color newColor);
	virtual Color getColor() const;
	virtual ITexture* getTexture() const;
	virtual IBuffer* getVertexBuffer() const;
	virtual IBuffer* getIndexBuffer() const;

protected:
	void createVertexBuffer();
	void createIndexBuffer();
	void updateVertexBuffer();
	Rect makeVertexBufferData();

private:
	Vertex m_position;
	Vertex m_scaling;
	float m_rotateDegree;
	Vertex m_rotateCenter;
	Color m_color;
	ITexture* m_pTexture;
	IBuffer* m_pVertexBuffer, * m_pIndexBuffer;
};