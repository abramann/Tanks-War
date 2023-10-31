// image.cpp
// Author: abramann

#include "image.h"
#include "game.h"
#include "timer.h"
#include "dx11wrapper.h"
#include "texturemanger.h"

Image::Image() : m_animate(false), m_column(1), m_row(1), m_columns(0), m_rows(0),
m_timeUntilLastUpdate(0), m_updateDelay(0), m_width(0), m_height(0), m_textureWidth(0),
m_textureHeight(0), m_pVertexBuffer(0), m_pStagingBuffer(0), m_position(0, 0, 0), m_rotate(0, 0, 0),
m_scalling(1, 1, 1), m_initialized(false)
{
}

Image::~Image()
{
}

void Image::initialize(const std::string& texture, const Game* pGame, int8 columns, int8 rows,
	int32 updateDelay)
{
	if (pGame)
	{
		m_pGraphics = pGame->getGraphics();
		m_pDx11Wrapper = pGame->getDx11Wrapper();
		m_pTimer = pGame->getTimer();
		m_pTextureManger = pGame->getTextureManger();
	}
	
	m_pTexture = m_pTextureManger->getTexture(texture);;
	m_columns = columns;
	m_rows = rows;
	m_textureWidth = m_pTexture->getWidth();
	m_textureHeight = m_pTexture->getHeight();
	if (m_columns != 1 && m_rows != 1)
	{
		m_animate = true;
		m_updateDelay = updateDelay;
		m_width = m_textureWidth / m_rows;
		m_height = m_textureHeight / m_columns;
	}
	else
	{
		m_animate = false;
		m_width = m_textureWidth;
		m_height = m_textureHeight;
	}

	if (!m_initialized)
	{
		createVertexBuffer();
		m_initialized = true;
	}

	updateLocalCoordinate();
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
	V3 rotateCenter = getRotateCenter();
	Space space;
	if (rotateCenter.x == 0 && rotateCenter.y == 0)
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
		float f2 = -s*(abs(f1) - 1);
		space.v1.x = x0, space.v1.y = y0;
		space.v2.x = space.v1.x + m_width*(f1),
			space.v2.y = space.v1.y + (m_height *f2);
		space.v3.x = space.v2.x + m_width*(-1 * f2);
		space.v3.y = space.v2.y + m_height*f1;
		space.v4.x = space.v1.x + m_width*(-1 * f2);
		space.v4.y = space.v1.y + m_height*f1;
		return space;
	}
	else
	{
		/*if (x0 == 0)
			x0 = m_position.x;
		if (y0 == 0)
			y0 = m_position.y;

		const float& angle = m_rotate.z;
		int8 s = (angle >= 0) ? 1 : -1;

		float f1 = 1 + (-0.636619772f*abs(angle));
		float f2 = -1 * s*(abs(f1) - 1);
		space.v1.x = x0, space.v1.y = y0;
		space.v2.x = space.v1.x + m_width*(f1),
			space.v2.y = space.v1.y + (m_height *f2);
		space.v3.x = space.v2.x + m_width*(-1 * f2);
		space.v3.y = space.v2.y + m_height*f1;
		space.v4.x = space.v1.x + m_width*(-1 * f2);
		space.v4.y = space.v1.y + m_height*f1;

		*/


		float f11 = 0, f33 = 0, f44 = 0;
		const float& x = m_rotate.z;
		
		if (IN_RANGE_OR_EQUAL(x, 0, -PI))
		{
			f11 = 1.2732395447351626861510701069801*x;// f(-PI/4)=-1
			static float inc1 = 0;
			static float inc2 = 0;
			if (IN_RANGE(x, 0, -PI / 4))
				inc1 = 30 * f11, inc2 = 15 * f11;

			space.v1 = m_position;
			space.v1.x += inc2;
			space.v1.y -= inc1;

			space.v2.x = m_width + m_position.x + inc1;
			space.v2.y = m_position.y + inc2;
			space.v3.x = m_position.x + m_width - inc2;
			space.v3.y = m_position.y + m_height + inc1;

			space.v4.x = m_position.x - inc1;
			space.v4.y = m_position.y + m_height - inc2;

			static float f22 = 0;
			if (IN_RANGE_OR_EQUAL(x, -PI / 4, -PI / 2))
				f22 = f11 + 1.000000;
				//f22 = 1.273239*x + 1.000000;

			space.v1.x += 15 * -f22;
			space.v1.y += 30 * -f22;
			space.v2.x -= 30 * -f22;
			space.v2.y += 15 * -f22;
			space.v3.x -= 15 * -f22;
			space.v3.y -= 30 * -f22;
			space.v4.x += 30 * -f22;
			space.v4.y -= 15 * -f22;
			if (IN_RANGE_OR_EQUAL(x, -PI / 2, -PI))
			{
				float shortX = x + PI / 2;
				f11 = 1.2732395447351626861510701069801*shortX;// f(-PI/4)=-1
				static float inc11 = 0;
				static float inc22 = 0;
				inc11 = 30 * f11, inc22 = 15 * f11;

				space.v2 = m_position;
				space.v2.x += inc22;
				space.v2.y -= inc11;

				space.v3.x = m_width + m_position.x + inc11;
				space.v3.y = m_position.y + inc22;
				space.v4.x -= inc22;
				space.v4.y = m_position.y + m_height + inc11;

				space.v1.x = m_position.x - inc11;
				space.v1.y = m_position.y + m_height - inc22;
				if (IN_RANGE_OR_EQUAL(shortX, 0, -PI / 4))
					return space;

				f22 = f11 + 1.000000;

				space.v1.x += 15 * -f22;
				space.v1.y += 30 * -f22;
				space.v2.x -= 30 * -f22;
				space.v2.y += 15 * -f22;
				space.v3.x -= 15 * -f22;
				space.v3.y -= 30 * -f22;
				space.v4.x += 30 * -f22;
				space.v4.y -= 15 * -f22;
			}
		}

		return space;
	}
}
/*space.v1 = m_position;

space.v1.x += 15 * f11;
space.v1.y -= 30 * f11;

space.v2.x = m_width + m_position.x;
space.v2.x -=30 * -f11;
space.v2.y = m_position.y;
space.v2.y -= 15 * -f11;
space.v3.x -= 15 * f11;
space.v3.y = m_position.y + m_height;
space.v3.y += 30 * f11;

space.v4.x = m_position.x + 30 * -f11;
space.v4.y = m_position.y + m_height + 15 * -f11;

space.v1 = m_position;
space.v2.x = space.v1.x + m_width;
space.v2.y = space.v1.y;
space.v3.x = space.v2.x;
space.v3.y = space.v1.y + m_height;
space.v4.x = space.v1.x;
space.v4.y = space.v3.y;
return space; */
void Image::createVertexBuffer()
{
	m_pVertexBuffer = m_pDx11Wrapper->createVertexBuffer(graphicsNS::VERTICES_IMAGE, D3D11_CPU_ACCESS_WRITE, 0);
	m_pStagingBuffer = m_pDx11Wrapper->createStagingBuffer(D3D11_CPU_ACCESS_READ, sizeof(Vertex), graphicsNS::VERTICES_IMAGE, 0);
}

void Image::updateLocalCoordinate()
{
	float width = m_width, height = m_height;
	float u = 1.0f / m_rows, v = 1.0f / m_columns;
	Vertex vx[4];
	vx[0] = Vertex(width, 0, 0.0f, u, 0.0f);
	vx[1] = Vertex(0, 0, 0.0f, 0.0f, 0.0f);
	vx[2] = Vertex(width, height, 0.0f, u, -v);
	vx[3] = Vertex(0, height, 0.0f, 0.0f, -v);
	m_pDx11Wrapper->copyToResource(m_pVertexBuffer.Get(), vx, 4*sizeof(Vertex));
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
		{
			executeAnimateRepeat();
			return;
		}
		else
		{
			m_column++;
			m_row = 1;
		}
	}

	Vertex vx[graphicsNS::VERTICES_IMAGE];
	m_pDx11Wrapper->copyResourceToResource(m_pStagingBuffer.Get(), m_pVertexBuffer.Get());
	m_pDx11Wrapper->copyResource(vx, m_pStagingBuffer.Get(), sizeof(Vertex) * graphicsNS::VERTICES_IMAGE);

	vx[0].u = (m_row)*u, vx[0].v = (m_column - 1)*v;
	vx[1].u = (m_row - 1)*u, vx[1].v = (m_column - 1)*v;
	vx[2].u = (m_row)*u, vx[2].v = (m_column)*v;
	vx[3].u = (m_row - 1)*u, vx[3].v = (m_column)*v;

	m_row++, m_timeUntilLastUpdate = 0;
	m_pDx11Wrapper->copyToResource(m_pVertexBuffer.Get(), vx, graphicsNS::VERTICES_IMAGE*sizeof(Vertex));
}