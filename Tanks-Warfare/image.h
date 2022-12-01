#ifndef _IMAGE_H
#define _IMAGE_H
#include "always.h"
#include "texture.h"

#define Frame int
#define FRAMES		3
#define FRAMECURRENT	0
#define FRAMESTART		1
#define FRAMEEND		2

class Image
{
public:

	Image();
	~Image();
	virtual void initialize(int width, int height, int extraWidth, int extraHeight, bool _animate, int currentFrame, int startFrame, int endFrame, float _updateDelay, TextureManger* _textureManger, Graphics* _graphics);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void release();
	virtual void setX(int x)		{ spriteData.x = x; }
	virtual void setY(int y)		{ spriteData.y = y; }
	virtual void setImageWidth(int width)		{ spriteData.width = width; }
	virtual void setImageHeight(int height)			{ spriteData.height = height; }
	virtual void setExtraImageWidth(int extraWidth)			{ spriteData.extraWidth = extraWidth; }
	virtual void setExtraImageHeight(int extraHeight)		{ spriteData.extraHeight = extraHeight; }
	virtual void setFilterColor(COLOR filterColor)		{ spriteData.filterColor = filterColor; }
	virtual void setAngle(float angle)		{ spriteData.angle = angle; }
	virtual void setCenter(V2 center)		{ spriteData.center = center; }
	virtual void setRect(RECT rect)		{ spriteData.rect = rect; }
	virtual void setScalling(float scalling);
	virtual void setFrameDelay(float newFrameDelay)		{ frameDelay = newFrameDelay; }
	virtual void setUpdateDelay(float newUpdateDelay)		{ updateDelay = newUpdateDelay; }
	virtual void setStartFrame(int newStartFrame)		{ frame[FRAMESTART] = newStartFrame; }
	virtual void setEndFrame(int newEndFrame)		{ frame[FRAMEEND] = newEndFrame; }
	virtual void setCurrentFrame(int newCurrentFrame)		{ frame[FRAMECURRENT] = newCurrentFrame; }
	virtual void setAnimate(bool newAnimate)		{ animate = newAnimate; }
	virtual void setFrameRect(int frame, int frames);
	virtual void setRightRect(int rightRect)		{ spriteData.rect.right = rightRect; }
	virtual void setLeftRect(int leftRect)		{ spriteData.rect.left = leftRect; }
	virtual void setTopRect(int topRect)		{ spriteData.rect.top = topRect; }
	virtual void setBottomRect(int bottomRect)		{ spriteData.rect.bottom = bottomRect; }
	virtual void setTexture(TextureManger* tm);
	
	virtual int getX()		{ return spriteData.x; }
	virtual int getY()		{ return spriteData.y; }
	virtual int getWidth()		{ return spriteData.width; }
	virtual int getHeight()		{ return spriteData.height; }
	virtual int getExtraImageWidth()		{ return spriteData.extraWidth; }
	virtual int getExtraImageHeight()		{ return spriteData.extraHeight; }
	virtual int getTextureWidth()		{ return textureManger->getWidth(); }
	virtual int getTextureHeight()		{ return textureManger->getWidth(); }
	virtual double getAngle()		{ return spriteData.angle; }
	virtual V2 getCenter()		{ return spriteData.center; }
	virtual RECT getRect()		{ return spriteData.rect; }
	virtual float getScalling()		{ return spriteData.scalling; }
	virtual float getFrameDelay()		{ return frameDelay; }
	virtual float getUpdateDelay()		{ return updateDelay; }
	virtual int	getStartFrame()		{ return frame[FRAMESTART]; }
	virtual int getEndFrame()		{ return frame[FRAMEEND]; }
	virtual COLOR getFilterColor()		{ return spriteData.filterColor; }
	virtual bool isAnimated() { return animate; }

	virtual void xAdd(int xAdd) { spriteData.x += xAdd; }
	virtual void yAdd(int yAdd) { spriteData.y += yAdd; }

protected:

	TextureManger* textureManger;
	Graphics* graphics;
	Frame* frame;

	SpriteData spriteData;
	float updateDelay, frameDelay;
	bool animate;
	
private:

	HANDLE threadHandle;
	int threadPara;

};


#endif