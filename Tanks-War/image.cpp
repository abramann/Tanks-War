#include "image.h"

Image::Image() : m_animate(0), m_frameDelay(0), m_updateDelay(0),
	m_column(1), m_row(1), m_threadHandle(0), m_threadPara(0)
{
	ZeroMemory(&m_spriteData, sizeof(SpriteData));
}

Image::~Image()
{
	release();
}

void Image::initialize(int width, int height, int  columns, int rows, bool animate, float updateDelay, TextureManger* textureManger, Graphics* graphics)
{
	m_pGraphics = graphics;
	m_pTextureManger = textureManger;
	if (width <= 0)
		width = textureManger->getWidth();	// Use full texture width
	if (height <= 0)
		height = textureManger->getHeight();	// Use full texture height;

	m_spriteData.width = width;
	m_spriteData.height = height;
	m_spriteData.columns = columns;
	m_spriteData.rows = rows;
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
	if (m_animate & m_frameDelay >= m_updateDelay)
	{
		if (m_column == m_spriteData.columns)
		{
			m_column = 1;
			// Testing code
			::DebugBreak();
			(m_row == m_spriteData.rows) ? m_row = 1 : m_row++;
			/*if (m_row == m_spriteData.rows)
				m_row = 1;
			else if (m_row < m_spriteData.rows)
				m_row++;*/
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

void Image::setFrameRect(int _m_column, int _m_row, int frames)
{
	if (WaitForSingleObject(m_threadHandle, 0) != 0xffffffff)		// Is thread running
		return;		// Thread is running

	RECT rect;
	rect.right = _m_column * m_spriteData.width;
	rect.left = rect.right - m_spriteData.width;
	rect.bottom = _m_row * m_spriteData.height;
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
	m_spriteData.textureWidth = textureManger->getWidth();
	m_spriteData.textureHeight = textureManger->getHeight();
}
