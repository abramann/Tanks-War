#include "image.h"

Image::Image()
{
	ZeroMemory(&spriteData, sizeof(SpriteData));
	animate = NULL;
	frameDelay = 0.0f;	updateDelay = 0.0f;
}

Image::~Image()
{
	release();
}

void Image::initialize(int width, int height, int extraWidth, int extraHeight, bool _animate, int currentFrame, int startFrame, int endFrame, float _updateDelay, TextureManger* _textureManger, Graphics* _graphics)
{
	frame = new int[FRAMES];
	graphics = _graphics;
	textureManger = _textureManger;
	spriteData.texture = textureManger->getTexture();
	spriteData.textureWidth = textureManger->getWidth();
	spriteData.textureHeight = textureManger->getHeight();
	frame[FRAMECURRENT] = currentFrame;
	frame[FRAMESTART] = startFrame;
	frame[FRAMEEND] = endFrame;
	updateDelay = _updateDelay;
	animate = _animate;
	spriteData.scalling = 1.0f;
	if (width <= 0)
		width = textureManger->getWidth();
	if (height <= 0)
		height = textureManger->getHeight();
	
	spriteData.width = width;
	spriteData.height = height;
	spriteData.extraWidth = extraWidth;
	spriteData.extraHeight = extraHeight;
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
	SAFE_DELETE(frame);
}

void Image::update(float frameTime)
{
	frameDelay += frameTime;
	if (animate && frameDelay >= updateDelay)
	{
		frameDelay = 0;
		if (frame[FRAMECURRENT] == frame[FRAMEEND])
			frame[FRAMECURRENT] = frame[FRAMEEND];
		else
			frame[FRAMECURRENT]++;
	}
	setFrameRect(frame[FRAMECURRENT], 0);
}

void Image::setScalling(float scalling)	
{
	spriteData.scalling = scalling;
	spriteData.center = V2(spriteData.width*scalling / 2, spriteData.height*scalling / 2);
}

void Image::setFrameRect(int frame, int frames)
{
	if (WaitForSingleObject(threadHandle, 0) == 0x102)		// Is thread running ?
		return;		// Thread is running
	RECT rect;
	rect.right = frame * spriteData.width + getExtraImageWidth();
	rect.left = rect.right - spriteData.width;
	rect.top = 0;
	rect.bottom = spriteData.height;
	setRect(rect);
	if (frames != 0)
	{
		threadPara = frames;
		CreateThread(0, 0,(LPTHREAD_START_ROUTINE) &waitFrame, &threadPara, 0, 0);
	}
}

void Image::setTexture(TextureManger* _textureManger)
{
	textureManger = _textureManger;
	spriteData.texture = textureManger->getTexture();
	spriteData.textureWidth = _textureManger->getWidth();
	spriteData.textureHeight = _textureManger->getHeight();
}
