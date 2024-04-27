#include "IImage.h"


IImage::IImage()
{
	m_position = Vertex(0, 0);
	m_scaling = Vertex(1, 1);
	m_rotateDegree = 0;
	m_relativeCamera = false;
	m_color = makeColor(255, 255, 255, 1);
	m_pTexture = nullptr;
}

IImage::~IImage()
{
	IObject2D::~IObject2D();
	g_pRenderer->releaseTexture(m_pTexture);
}

void IImage::setScaling(Vertex newScaling)
{
	m_scaling = newScaling;
}

Vertex IImage::getScaling() const
{
	return m_scaling;
}

void IImage::makeRotate(float degree)
{
	m_rotateDegree += degree;
}

void IImage::setRotate(float newRotate)
{
	m_rotateDegree = newRotate;
}

float IImage::getRotate() const
{
	return m_rotateDegree;
}

bool IImage::isRelativeToCamera() const
{
	return m_relativeCamera;
}

void IImage::makeRelativeToCamera(bool newRelative)
{
	m_relativeCamera = newRelative;
}

void IImage::setColor(Color newColor)
{
	m_color = newColor;
}

Color IImage::getColor() const
{
	return m_color;
}

ITexture * IImage::getTexture() const
{
	return m_pTexture;
}
