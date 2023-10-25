// image.cpp
// Author: abramann

#include "image.h"
#include "game.h"
#include "timer.h"
#include "dx11wrapper.h"
#include "texturemanger.h"
#include "inlined.inl"

Image::Image() : m_animate(false), m_column(1), m_row(1), m_columns(0), m_rows(0),
m_timeUntilLastUpdate(0), m_updateDelay(0), m_width(0), m_height(0), m_textureWidth(0),
m_textureHeight(0), m_pVertexBuffer(0), m_pStagingBuffer(0), m_position(0, 0, 0), m_rotate(0, 0, 0),
m_scalling(1, 1, 1), m_initialized(false)
{
	V3* edge[] = { &m_space.v1,&m_space.v2,&m_space.v3,&m_space.v4 };
	memcpy(m_edge, edge, sizeof(Space*) * 4);
	m_spaceAdd1 = 0, m_spaceAdd2 = 0;
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
	if (x0 == 0)
		x0 = m_position.x;
	if (y0 == 0)
		y0 = m_position.y;

	V3 rotateCenter = getRotateCenter();
	if (m_width == m_height && !(rotateCenter == V3(0, 0, 0)))
	{
		float x = getNegativeRotate(), f11 = 0,
			piOverTwo = x / (-PI / 2);
#ifdef _DEBUG
		if (x < -2 * PI || x > 0)
			debuggerBreak("Invalid angle!\n");
#endif 
		const int16& polygon = m_width;
		if (piOverTwo > 3)
		{
			m_edge[0] = &m_space.v4,
				m_edge[1] = &m_space.v1,
				m_edge[2] = &m_space.v2,
				m_edge[3] = &m_space.v3;
			x += 3 * PI / 2;
			m_spaceAdd1 = -polygon / 2, m_spaceAdd2 = -polygon / 4;
		}
		else if (piOverTwo > 2)
		{
			m_edge[0] = &m_space.v3,
				m_edge[1] = &m_space.v4,
				m_edge[2] = &m_space.v1;
			m_edge[3] = &m_space.v2;
			x += PI;
			m_spaceAdd1 = -polygon / 2, m_spaceAdd2 = -polygon / 4;
		}
		else if (piOverTwo > 1)
		{
			m_edge[3] = &m_space.v1,
				m_edge[0] = &m_space.v2,
				m_edge[1] = &m_space.v3,
				m_edge[2] = &m_space.v4;
			x += (PI / 2);
			m_spaceAdd1 = -polygon / 2, m_spaceAdd2 = -polygon / 4;
		}
		else if (m_edge[0] != &m_space.v1)
		{
			m_edge[0] = &m_space.v1,
				m_edge[1] = &m_space.v2,
				m_edge[2] = &m_space.v3,
				m_edge[3] = &m_space.v4;
			f11 = 1.2732395447351626861510701069801*-PI / 4;
			m_spaceAdd1 = f11 * m_width / 2, m_spaceAdd2 = f11*m_height / 4;
		}

		f11 = 1.2732395447351626861510701069801*x;
		if (IN_RANGE(x, 0, -PI / 4))
			m_spaceAdd1 = f11*polygon / 2, m_spaceAdd2 = f11*polygon / 4;

		m_edge[0]->x = x0 + m_spaceAdd2;
		m_edge[0]->y = y0 - m_spaceAdd1;
		m_edge[1]->x = polygon + x0 + m_spaceAdd1;
		m_edge[1]->y = y0 + m_spaceAdd2;
		m_edge[2]->x = x0 + polygon - m_spaceAdd2;
		m_edge[2]->y = y0 + polygon + m_spaceAdd1;
		m_edge[3]->x = x0 - m_spaceAdd1;
		m_edge[3]->y = y0 + polygon - m_spaceAdd2;

		if (IN_RANGE_OR_EQUAL(x, -PI / 4, -PI / 2))
		{
			float f22 = f11 + 1.000000;
			m_edge[0]->x += 15 * -f22;
			m_edge[0]->y += 30 * -f22;
			m_edge[1]->x -= 30 * -f22;
			m_edge[1]->y += 15 * -f22;
			m_edge[2]->x -= 15 * -f22;
			m_edge[2]->y -= 30 * -f22;
			m_edge[3]->x += 30 * -f22;
			m_edge[3]->y -= 15 * -f22;
		}
	}
	else
	{
		float f1 = 1 + (-0.636619772f*abs(m_rotate.z));
		float f2 = ((m_rotate.z < 0) ? -1 : 1) * -(abs(f1) - 1);
		m_space.v1.x = x0, m_space.v1.y = y0;
		m_space.v2.x = m_space.v1.x + m_width*(f1);
		m_space.v2.y = m_space.v1.y + (m_width *f2);
		m_space.v3.x = m_space.v2.x + m_height*(-1 * f2);
		m_space.v3.y = m_space.v2.y + m_height*f1;
		m_space.v4.x = m_space.v1.x + m_height*(-1 * f2);
		m_space.v4.y = m_space.v1.y + m_height*f1;
		return m_space;
	}

	m_space = Space(*m_edge[0], *m_edge[1], *m_edge[2], *m_edge[3]);
	return m_space;
}

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
	m_pDx11Wrapper->copyToResource(m_pVertexBuffer.Get(), vx, 4 * sizeof(Vertex));
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