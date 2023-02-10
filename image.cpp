#include "image.h"

Image::Image()
{
	ZeroMemory(&spriteData, sizeof(SpriteData));
	animate = NULL;
	frameDelay = 0.0f;	updateDelay = 0.0f;
	column = 1;	row = 1;
}

Image::~Image()
{
	release();
}

void Image::initialize(int width, int height, int  columns, int rows, bool _animate, float _updateDelay, TextureManger* _textureManger, Graphics* _graphics)
{
	graphics = _graphics;
	textureManger = _textureManger;
	if (width <= 0)					
		width = textureManger->getWidth();	// Use full texture width
	if (height <= 0)
		height = textureManger->getHeight();	// Use full texture height;
		
	spriteData.width = width;
	spriteData.height = height;
	spriteData.columns = columns;
	spriteData.rows = rows;
	animate = _animate;
	updateDelay = _updateDelay;
	spriteData.texture = textureManger->getTexture();
	spriteData.textureWidth = textureManger->getWidth();
	spriteData.textureHeight = textureManger->getHeight();



	spriteData.scalling = 1.0f;
	spriteData.center = V2(width / 2, height / 2);
	spriteData.filterColor = COLOR_WHITE;
}

void Image::draw()
{
	graphics->spriteBegin();
	graphics->spriteDraw(spriteData);
	graphics->spriteEnd();
}

void Image::release()
{
}

void Image::update(float frameTime)
{
	frameDelay += frameTime;
	if (animate && frameDelay >= updateDelay)
	{
		if (column == spriteData.columns)
		{
			column = 1;
			if (row == spriteData.rows)
				row = 1;
			else if (row < spriteData.rows)
				row++;
		}
		else
			column++;

		frameDelay = 0;
	}

	setFrameRect(column,row,0);
}

void Image::setScalling(float scalling)	
{
	spriteData.scalling = scalling;
	spriteData.center = V2(spriteData.width * scalling / 2, spriteData.height * scalling / 2);
}

void Image::setFrameRect(int _column, int _row, int frames)
{
	if (WaitForSingleObject(threadHandle, 0) != 0xffffffff)		// Is thread running
		return;		// Thread is running

	RECT rect;
	rect.right = _column * spriteData.width;
	rect.left = rect.right - spriteData.width;
	rect.bottom = _row * spriteData.height;
	rect.top = rect.bottom - spriteData.height;
	setRect(rect);
	if (frames != 0)
	{
		threadPara = frames;
		DWORD threadId = 0;
		threadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)& waitFrame, &threadPara, 0, 0);
	}
}

void Image::setTexture(TextureManger* _textureManger)
{
	textureManger = _textureManger;
	spriteData.texture = textureManger->getTexture();
	spriteData.textureWidth = _textureManger->getWidth();
	spriteData.textureHeight = _textureManger->getHeight();
}
