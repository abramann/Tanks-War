#pragma once

#include "Color.h"
#include "IRenderer.h"
#include "IObject2D.h"




class ITexture;

class IImage : public IObject2D
{
public:
	IImage();
	virtual ~IImage();

	virtual void setScaling(Vertex newScaling);
	virtual Vertex getScaling() const;
	virtual void makeRotate(float newRotate);
	virtual void setRotate(float newRotate);
	virtual float getRotate() const;
	virtual bool isRelativeToCamera() const;
	virtual void makeRelativeToCamera(bool newRelative);
	virtual void setColor(Color newColor);
	virtual Color getColor() const;
	virtual ITexture* getTexture() const;

protected:
	Vertex m_position;
	Vertex m_scaling;
	float m_rotateDegree;
	Vertex m_rotateCenter;
	bool m_relativeCamera;
	Color m_color;
	ITexture* m_pTexture;
};
