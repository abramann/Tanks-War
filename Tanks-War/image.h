#ifndef _IMAGE_H
#define _IMAGE_H
#include "always.h"
#include "texturemanger.h"

struct Space
{
	int32_t x1, y1;
	int32_t x2, y2;
};

class Image
{

public:
	
	Image();
	~Image();
	virtual void initialize(uint16_t width, uint16_t height, uint8_t  columns, uint8_t rows, bool animate, float updateDelay, TextureManger* textureManger, Graphics* graphics);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void release();
	virtual Image& setXY(V2 xy) { setX(xy.x).setY(xy.y); return *this; }
	virtual Image& setX(float x) { m_spriteData.x = x; return *this; }
	virtual Image& setY(float y) { m_spriteData.y = y; return *this; }
	virtual void setImageWidth(int width) { m_spriteData.width = width; }
	virtual void setImageHeight(int height) { m_spriteData.height = height; }
	virtual void setFilterColor(Color filterColor) { m_spriteData.filterColor = filterColor; }
	virtual void setAngle(float angle) { m_spriteData.angle = angle; }
	virtual void setCenter(V2 center) { m_spriteData.center = center; }
	virtual void setRect(RECT rect) { m_spriteData.rect = rect; }
	virtual void setScalling(float scalling);
	virtual void setFrameDelay(float newm_frameDelay) { m_frameDelay = newm_frameDelay; }
	virtual void setUpdateDelay(float newm_updateDelay) { m_updateDelay = newm_updateDelay; }
	virtual void setAnimate(bool animate) { m_animate = animate; }
	virtual void setColumn(int newm_column) { m_column = newm_column; }
	virtual void setRow(int row) { m_row = row; }
	virtual void setFrameRect(uint8_t column, uint8_t row, uint16_t frames);
	virtual void setRightRect(int rightRect) { m_spriteData.rect.right = rightRect; }
	virtual void setLeftRect(int leftRect) { m_spriteData.rect.left = leftRect; }
	virtual void setTopRect(int topRect) { m_spriteData.rect.top = topRect; }
	virtual void setBottomRect(int bottomRect) { m_spriteData.rect.bottom = bottomRect; }
	virtual void setTexture(TextureManger* tm);
	virtual void setDefaultTextureInfo();
	void setTextureImageWidth() { m_spriteData.width = m_pTextureManger->getImageWidth(); }
	void setTextureImageHeight() { m_spriteData.height = m_pTextureManger->getImageHeight(); }
	void setTextureColumns() { m_spriteData.columns = m_pTextureManger->getColumns(); }
	void setTextureRows() { m_spriteData.rows = m_pTextureManger->getRows(); }
	void setTextureUpdateDelay() { m_updateDelay = m_pTextureManger->getAnimateSpeed(); }
	void setDefaultColumnRow() { m_column = 1; m_row = 1; }
	void setNullSpriteData() { ZeroMemory(&m_spriteData, sizeof(SpriteData)); }
	void setRotationCenter(V2 center) { m_spriteData.center = center; }
	virtual float getX() const { return m_spriteData.x; }
	virtual float getY() const { return m_spriteData.y; }
	virtual V2 getXY() const { return V2(getX(), getY()); }
	virtual int getWidth() const { return m_spriteData.width; }
	virtual int getHeight() const { return m_spriteData.height; }
	virtual int getTextureWidth() const { return m_pTextureManger->getWidth(); }
	virtual int getTextureHeight() const { return m_pTextureManger->getHeight(); }
	virtual double getAngle() const { return m_spriteData.angle; }
	virtual float getCenterX() const { return m_spriteData.x + m_spriteData.width / 2; }
	virtual float getCenterY() const { return m_spriteData.y + m_spriteData.height / 2; }
	virtual V2 getCenter() const { return V2(getCenterX(), getCenterY()); }
	virtual V2 getRotationCenter() const { return m_spriteData.center; }
	virtual RECT getRect() const { return m_spriteData.rect; }
	virtual float getScalling() const { return m_spriteData.scalling; }
	virtual float getFrameDelay() const { return m_frameDelay; }
	virtual float geUupdateDelay() const { return m_updateDelay; }
	virtual int	getRow() const { return m_row; }
	virtual int getColumn() const { return m_column; }
	float getSin() const  { return m_sinA; }
	float getCos() const { return m_cosA; }
	V2 getFocusSite() const;
	virtual void setLookTo(const Image& image);
	virtual Color getFilterColor() const { return m_spriteData.filterColor; }
	virtual SpriteData getSpriteData() const { return m_spriteData; }
	virtual bool isAnimated() const { return m_animate; }
	virtual Space getAllocatedSpace() const;

	virtual void endFrame();
	virtual void xAdd(float value) { m_spriteData.x += value; }
	virtual void yAdd(float value) { m_spriteData.y += value; }
	virtual void xDec(float value) { m_spriteData.x -= value; }
	virtual void yDec(float value) { m_spriteData.y -= value; }
	virtual void follow(float x, float y, float speed);
	void mathUpdate();

protected:

	TextureManger* m_pTextureManger;
	Graphics* m_pGraphics;

	SpriteData m_spriteData;
	float m_updateDelay, m_frameDelay;
	int m_column, m_row; // current column and row seted
	bool m_animate;
	float m_sinA, m_cosA;

private:

	HANDLE m_threadHandle;
	int m_threadPara;
};


#endif