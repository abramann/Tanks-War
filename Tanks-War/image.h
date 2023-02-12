#ifndef _IMAGE_H
#define _IMAGE_H
#include "always.h"
#include "texture.h"

class Image
{
public:

	Image();
	~Image();
	virtual void initialize(int width, int height, int  m_columns, int m_rows, bool _m_animate, float _m_updateDelay, TextureManger* _textureManger, Graphics* _graphics);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void release();
	//virtual Image& setXY(V2 xy) { setX(xy.x).setY(xy.y); return *this; }
	virtual Image& setX(float x) { m_spriteData.x = x; return *this; }
	virtual Image& setY(float y) { m_spriteData.y = y; return *this; }
	virtual void setImageWidth(int width) { m_spriteData.width = width; }
	virtual void setImageHeight(int height) { m_spriteData.height = height; }
	virtual void setFilterColor(COLOR filterColor) { m_spriteData.filterColor = filterColor; }
	virtual void setAngle(float angle) { m_spriteData.angle = angle; }
	virtual void setCenter(V2 center) { m_spriteData.center = center; }
	virtual void setRect(RECT rect) { m_spriteData.rect = rect; }
	virtual void setScalling(float scalling);
	virtual void setFrameDelay(float newm_frameDelay) { m_frameDelay = newm_frameDelay; }
	virtual void setUpdateDelay(float newm_updateDelay) { m_updateDelay = newm_updateDelay; }
	virtual void setColumn(int newm_column) { m_column = newm_column; }
	virtual void setRow(int newm_row) { m_row = newm_row; }
	virtual void setm_animate(bool newm_animate) { m_animate = newm_animate; }
	virtual void setFrameRect(int _m_column, int _m_row, int frames);
	virtual void setRightRect(int rightRect) { m_spriteData.rect.right = rightRect; }
	virtual void setLeftRect(int leftRect) { m_spriteData.rect.left = leftRect; }
	virtual void setTopRect(int topRect) { m_spriteData.rect.top = topRect; }
	virtual void setBottomRect(int bottomRect) { m_spriteData.rect.bottom = bottomRect; }
	virtual void setTexture(TextureManger* tm);

	virtual int getX() { return m_spriteData.x; }
	virtual int getY() { return m_spriteData.y; }
	virtual int getWidth() { return m_spriteData.width; }
	virtual int getHeight() { return m_spriteData.height; }
	virtual int getTextureWidth() { return m_pTextureManger->getWidth(); }
	virtual int getTextureHeight() { return m_pTextureManger->getHeight(); }
	virtual double getAngle() { return m_spriteData.angle; }
	virtual V2 getCenter() { return V2(getCenterX(), getCenterY()); }
	virtual int getCenterX() { return m_spriteData.x + m_spriteData.width / 2; }
	virtual int getCenterY() { return m_spriteData.y + m_spriteData.height / 2; }
	virtual RECT getRect() { return m_spriteData.rect; }
	virtual float getScalling() { return m_spriteData.scalling; }
	virtual float getFrameDelay() { return m_frameDelay; }
	virtual float geUupdateDelay() { return m_updateDelay; }
	virtual int	getRow() { return m_row; }
	virtual int getColumn() { return m_column; }
	virtual COLOR getFilterColor() { return m_spriteData.filterColor; }
	virtual SpriteData getSpriteData() { return m_spriteData; }
	virtual bool isAnimated() { return m_animate; }

	virtual void xAdd(int xAdd) { m_spriteData.x += xAdd; }
	virtual void yAdd(int yAdd) { m_spriteData.y += yAdd; }

protected:

	TextureManger* m_pTextureManger;
	Graphics* m_pGraphics;

	SpriteData m_spriteData;
	float m_updateDelay, m_frameDelay;
	int m_column, m_row; // current column and row seted
	bool m_animate;

private:

	HANDLE m_threadHandle;
	int m_threadPara;
};


#endif