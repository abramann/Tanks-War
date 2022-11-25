#include "image.h"

Image::Image()
{
	ZeroMemory(&spriteData, sizeof(SpriteData));
	animate = NULL;
	frameDelay = 0.0f;	updateDelay = 0.0f;
	startFrame = 0;		endFrame = 0;	currentFrame = 0;
}

Image::~Image()
{
}

void Image::initialize(int width, int height, int extraWidth, int extraHeight, bool _animate, int _currentFrame, int _startFrame, int _endFrame, float _updateDelay, TextureManger* _textureManger, Graphics* _graphics)
{
	setGraphics(_graphics);
	setTexture(_textureManger);
	setCurrentFrame(_currentFrame);
	setStartFrame(_startFrame);
	setEndFrame(_endFrame);
	setFrameDelay(_updateDelay);
	setScalling(1.0f);
	setAnimate(_animate);
	if (width == 0 || height == 0)
	{
		width = getTextureWidth();
		height = getTextureHeight();
	}

	setImageWidth(width);
	setImageHeight(height);
	setExtraImageWidth(extraWidth);
	setExtraImageHeight(extraHeight);
	setFilterColor(COLOR_WHITE);
	RECT rect;
	rect.right = currentFrame * width + extraWidth;
	rect.left = rect.right - width;
	rect.top = extraHeight;
	rect.bottom = height + extraHeight;
	setRect(rect);
	V2 center = V2(width / 2, height / 2);
	setCenter(center);
}

void Image::draw()
{
	graphics->spriteBegin();
	graphics->spriteDraw(spriteData);
	graphics->spriteEnd();
}

void Image::update(float frameTime)
{
	frameDelay += frameTime;
	if (animate && frameDelay >= updateDelay)
	{
		frameDelay = 0;
		if (currentFrame == endFrame)
			currentFrame = startFrame;
		else
			currentFrame++;

		int rightRect = currentFrame * getWidth() + getExtraImageWidth();
		setRightRect(rightRect);
		int leftRect = rightRect - getWidth() + getExtraImageWidth();
		setLeftRect(leftRect);
	}
}

void Image::setScalling(float scalling)	
{
	spriteData.scalling = scalling;
	spriteData.center = V2(spriteData.width*scalling / 2, spriteData.height*scalling / 2);
}

void Image::setFrameRect(int frame, float time)
{
	spriteData.rect.right = frame*spriteData.width;
	spriteData.rect.left = spriteData.rect.right - spriteData.width;
}

void Image::setTexture(TextureManger* tm)
{
	textureManger = tm;
	spriteData.texture = textureManger->getTexture();
	spriteData.textureWidth = tm->getWidth();
	spriteData.textureHeight = tm->getHeight();
}
