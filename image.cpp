#include "image.h"

Image::Image() : m_animate(0), m_frameDelay(0), m_updateDelay(0),
	m_column(1), m_row(1), m_threadHandle(0), m_threadPara(0),
	m_sinA(0), m_cosA(0)
{
	ZeroMemory(&m_spriteData, sizeof(SpriteData));
}

Image::~Image()
{
	release();
}

void Image::initialize(uint16_t width, uint16_t height, uint8_t  columns, uint8_t rows, bool animate, float updateDelay, TextureManger* textureManger, Graphics* graphics)
{
	m_pGraphics = graphics;
	m_pTextureManger = textureManger;
	if (width == 0)
	{
		if (animate)
			width = textureManger->getImageWidth();
		else
			width = textureManger->getWidth();	// Use full texture width
	}
	if (height == 0)
	{
		if (animate)
			height = textureManger->getImageHeight();
		else
			height = textureManger->getHeight();	// Use full texture height;
	}

	m_spriteData.width = width;
	m_spriteData.height = height;
	if (columns == 1 && rows == 1 && animate)
	{
		m_spriteData.rows = textureManger->getRows();
		m_spriteData.columns = textureManger->getColumns();
	}
	else 
	{
		m_spriteData.columns = columns;
		m_spriteData.rows = rows;
	}

	m_animate = animate;
	m_updateDelay = updateDelay;
	m_spriteData.lpTexture = textureManger->getTexture();
	m_spriteData.textureWidth = textureManger->getWidth();
	m_spriteData.textureHeight = textureManger->getHeight();



	m_spriteData.scalling = 1.0f;
	m_spriteData.center = V2(width / 2, height / 2);
	m_spriteData.filterColor = COLOR_WHITE;
}

void Image::draw()
{
	m_pGraphics->spriteBegin();
	m_pGraphics->spriteDraw(m_spriteData);
	m_pGraphics->spriteEnd();
}

void Image::release()
{
}

void Image::update(float frameTime)
{
	m_frameDelay += frameTime;
	if (m_animate && m_frameDelay >= m_updateDelay)
	{
		if (m_column == m_spriteData.columns)
			if (m_row == m_spriteData.rows)
				endFrame();

		if (m_column == m_spriteData.columns)
		{
			m_column = 1;
			(m_row == m_spriteData.rows) ? m_row = 1 : m_row++;
		}
		else
			m_column++;

		m_frameDelay = 0;
	}

	setFrameRect(m_column, m_row, 0);
}

void Image::setScalling(float scalling)
{
	m_spriteData.scalling = scalling;
	m_spriteData.center = V2(m_spriteData.width * scalling / 2, m_spriteData.height * scalling / 2);
}

void Image::setFrameRect(uint8_t column, uint8_t row, uint16_t frames)
{
	if (WaitForSingleObject(m_threadHandle, 0) != 0xffffffff)		// Is thread running
		return;		// Thread is running

	RECT rect;
	rect.right = column * m_spriteData.width;
	rect.left = rect.right - m_spriteData.width;
	rect.bottom = row * m_spriteData.height;
	rect.top = rect.bottom - m_spriteData.height;
	setRect(rect);
	if (frames != 0)
	{
		m_threadPara = frames;
		DWORD threadId = 0;
		m_threadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)& waitFrame, &m_threadPara, 0, 0);
	}
}

void Image::setTexture(TextureManger* textureManger)
{
	m_pTextureManger = textureManger;
	m_spriteData.lpTexture = textureManger->getTexture();
}

void Image::setDefaultTextureInfo()
{
	setTextureWidth();
	setTextureHeight();
	setTextureImageWidth();
	setTextureImageHeight();
	setTextureColumns();
	setTextureRows();
}

Space Image::getAllocatedSpace() const
{
	Space space;
	space.x1 = m_spriteData.x;
	space.x2 = space.x1 + m_spriteData.width;
	space.y1 = m_spriteData.y;
	space.y2 = space.y1 + m_spriteData.height;
	return space;
}

void Image::endFrame()
{
}

void Image::follow(float x, float y, float speed)
{
	m_spriteData.x = m_spriteData.x + speed;
}

void Image::mathUpdate()
{
	m_sinA = _round(sin(m_spriteData.angle));
	m_cosA = _round(cos(m_spriteData.angle));
}