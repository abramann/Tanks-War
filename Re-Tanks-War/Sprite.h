#pragma once

#include "Math.h"
#include "SystemComponent.h"
#include "ColorTable.h"
#include <iostream>
#include "Model.h"

class ITexture;
class IBuffer;

class CSprite : public ISystemComponent
{
public:
	std::string getName() const override { return "Sprite"; };
	
	CSprite(const wchar_t* texFileName);
	virtual ~CSprite();

	// ISystemComponent
	void update() override {};
	void reset() override {};

	virtual void setPosition(Vertex position);
	virtual Vertex getPosition() const;
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