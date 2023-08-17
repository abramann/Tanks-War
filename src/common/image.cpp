// image.cpp
// Author: abramann

#include "image.h"
#include "game.h"
#include "timer.h"

Image::Image() : m_animate(false), m_column(1), m_row(1), m_columns(0), m_rows(0),
m_timeUntilLastUpdate(0), m_updateDelay(0), m_width(0), m_height(0), m_textureWidth(0),
m_textureHeight(0), m_lpVertexBuffer(0), m_position(0, 0, 0), m_rotate(0, 0, 0), m_scalling(1, 1, 1),
m_initialized(false)
{
	m_vertices = graphicsNS::VERTICES_IMAGE;
}

Image::~Image()
{
	safeRelease(m_lpVertexBuffer);
}

void Image::initialize(Texture * texture, const Game* game, int8 columns, int8 rows,
	int32 updateDelay)
{
	m_pTexture = texture;
	m_pGraphics = game->getGraphics();
	m_pTimer = game->getTimer();
	m_columns = columns;
	m_rows = rows;
	m_textureWidth = m_pTexture->getWidth();
	m_textureHeight = m_pTexture->getHeight();
	if (m_columns != 1 && m_rows != 1)
	{
		m_animate = true;
		m_width = m_textureWidth / m_rows;
		m_height = m_textureHeight / m_columns;
	}
	else
	{
		m_animate = false;
		m_width = m_textureWidth;
		m_height = m_textureHeight;
	}

	m_updateDelay = updateDelay;
	if (!m_initialized)
		createVertexBuffer();

	setLocalCoordinate();
	m_initialized = true;
}

void Image::update()
{
	if (m_animate)
	{
		auto frameTime = m_pTimer->getTimeDelta();
		updateTextureCoordinate(frameTime);
	}
}

void Image::draw() const
{
	m_pGraphics->drawImage(this);
}

V3 Image::getRotateCenter() const
{
	V3 center = V3(static_cast<float>(m_width / 2), static_cast<float>(m_height / 2), 0);
	return center;
}

Space Image::getSpace(float x0, float y0) const
{
	if (x0 == 0)
		x0 = m_position.x;
	if (y0 == 0)
		y0 = m_position.y;

	float angle = m_rotate.z;
	int8 s = 1;
	if (angle > 0.0001)
		s = +1;
	else if (angle < -0.0001)
		s = -1;

	float f1 = 1 + (-0.636619772f*abs(angle));
	float f2 = -1 * s*(abs(f1) - 1);
	Space is;
	is.v1.x = x0, is.v1.y = y0;
	is.v2.x = is.v1.x + m_width*(f1),
		is.v2.y = is.v1.y + (m_height *f2);
	is.v3.x = is.v2.x + m_width*(-1 * f2);
	is.v3.y = is.v2.y + m_height*f1;
	is.v4.x = is.v1.x + m_width*(-1 * f2);
	is.v4.y = is.v1.y + m_height*f1;
	return is;
}

void Image::createVertexBuffer()
{
	m_lpVertexBuffer = m_pGraphics->createVertexBuffer(m_vertices);
}

void Image::setLocalCoordinate()
{
	float width = m_width*1.0f, height = m_height*1.0f;
	float u = 1.0f / m_rows, v = 1.0f / m_columns;
	Vertex vx[graphicsNS::VERTICES_IMAGE];
	vx[0] = Vertex(width, 0, 0.0f, 0.0f, v);
	vx[1] = Vertex(0, 0, 0.0f, u, v);
	vx[2] = Vertex(0, height, 0.0f, u, 0.0f);
	vx[3] = Vertex(0, height, 0.0f, u, 0.0f);
	vx[4] = Vertex(width, height, 0.0f, 0.0f, 0.0f);
	vx[5] = Vertex(width, 0, 0.0f, 0.0f, v);
	m_pGraphics->setVertexBuffer(m_lpVertexBuffer, vx, m_vertices);
}

void Image::updateTextureCoordinate(int64 frameTime)
{
	m_timeUntilLastUpdate += frameTime;
	if (m_timeUntilLastUpdate >= m_updateDelay)
		setNextImageTextureCoordinate();
}

void Image::setNextImageTextureCoordinate()
{
	float u = 1.0f / (m_rows*1.0f),
		v = 1.0f / (m_columns*1.0f);
	if (m_row == m_rows + 1)
	{
		if (m_column == m_columns)
			executeAnimateRepeat();
		else
		{
			m_column++;
			m_row = 1;
		}
	}

	Vertex vx[6];
	vx[0].u = (m_row - 1)*u, vx[0].v = m_column*v;
	vx[1].u = (m_row)*u, vx[1].v = (m_column)*v;
	vx[2].u = m_row*u, vx[2].v = (m_column - 1)*v;
	vx[3].u = (m_row)*u, vx[3].v = (m_column - 1)*v;
	vx[4].u = (m_row - 1)*u, vx[4].v = (m_column - 1)*v;
	vx[5].u = (m_row - 1)*u, vx[5].v = m_column*v;

	m_pGraphics->setVertexBufferUV(m_lpVertexBuffer, vx, 6);
	m_row++, m_timeUntilLastUpdate = 0;
}