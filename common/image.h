#ifndef _IMAGE_H
#define _IMAGE_H
#include "constants.h"
#include "texturemanger.h"

class Image
{
public:

	Image();
	~Image();
	virtual void initialize(
		uint16_t width, uint16_t height, uint8_t  columns, uint8_t rows, bool animate,
		float updateDelay, Texture* texture, Graphics* graphics);
	virtual void initialize(Texture * texture, TextureManger* textureManger, Graphics * graphics);
	virtual void update(float frameTime);

	float getCos() const { return m_cosA; }
	float getSin() const { return m_sinA; }
	Texture* getTexture() { return m_pTexture; }
	virtual bool drawRapidly();
	virtual bool isAnimated() const { return m_animate; }
	virtual Color getFilterColor() const { return m_spriteData.filterColor; }
	virtual double getAngle() const { return m_spriteData.angle; }
	virtual float getCenterX() const { return m_spriteData.x + m_spriteData.width / 2; }
	virtual float getCenterY() const { return m_spriteData.y + m_spriteData.height / 2; }
	virtual float getFrameDelay() const { return m_frameDelay; }
	virtual float getScallingX() const { return m_spriteData.scalling.x; }
	virtual float getScallingY() const { return m_spriteData.scalling.y; }
	virtual float getX() const { return m_spriteData.x; }
	virtual float getY() const { return m_spriteData.y; }
	virtual float geUpdateDelay() const { return m_updateDelay; }
	virtual Image& setX(float x) { m_spriteData.x = x; return *this; }
	virtual Image& setY(float y) { m_spriteData.y = y; return *this; }
	virtual RECT getRect() const { return m_spriteData.rect; }
	virtual Space getAllocatedSpace() const;
	virtual SpriteData getSpriteData() const { return m_spriteData; }
	virtual uint16_t getHeight() const { return m_spriteData.height; }
	virtual uint16_t getTextureHeight() const { return m_pTexture->getHeight(); }
	virtual uint16_t getTextureWidth() const { return m_pTexture->getWidth(); }
	virtual uint16_t getWidth() const { return m_spriteData.width; }
	virtual uint8_t getColumn() const { return m_column; }
	virtual uint8_t	getRow() const { return m_row; }
	virtual V2 getCenter() const { return V2(getCenterX(), getCenterY()); }
	virtual V2 getFocusSite() const;
	virtual V2 getRotationCenter() const { return m_spriteData.center; }
	virtual V2 getXY() const { return V2(getX(), getY()); }
	virtual void cover();
	virtual void draw();
	virtual void endFrame();
	virtual void follow(float x, float y, float speed);
	virtual void release();
	virtual void setAngle(float angle) { m_spriteData.angle = angle; }
	virtual void setAnimate(bool animate) { m_animate = animate; }
	virtual void setBottomRect(int bottomRect) { m_spriteData.rect.bottom = bottomRect; }
	virtual void setCenter(V2 center) { m_spriteData.center = center; }
	virtual void setColumn(uint8_t column) { m_column = column; }
	virtual void setColumns(uint8_t columns) { m_spriteData.columns = columns; }
	virtual void setDefaultAnimate() { setAnimate(m_pImageInfo->animate); }
	virtual void setDefaultColumnRow() { m_column = 1; m_row = 1; }
	virtual void setDefaultColumns() { setColumns(m_pImageInfo->columns); }
	virtual void setDefaultImageHeight() { setHeight(m_pImageInfo->height); }
	virtual void setDefaultImageInfo();
	virtual void setDefaultImageWidth() { setWidth(m_pImageInfo->width); }
	virtual void setDefaultRows() { setRows(m_pImageInfo->rows); }
	virtual void setDefaultScalling() { setScalling(m_pImageInfo->scalling); }
	virtual void setDefaultUpdateDelay() { setUpdateDelay(m_pImageInfo->animateSpeed); }
	virtual void setFilterColor(Color filterColor) { m_spriteData.filterColor = filterColor; }
	virtual void setFrameDelay(float newm_frameDelay) { m_frameDelay = newm_frameDelay; }
	virtual void setFrameRect(uint8_t column, uint8_t row, uint16_t frames);
	virtual void setHeight(uint16_t height) { m_spriteData.height = height; }
	virtual void setLeftRect(int leftRect) { m_spriteData.rect.left = leftRect; }
	virtual void setLookTo(const Image& image);
	virtual void setNullSpriteData() { ZeroMemory(&m_spriteData, sizeof(SpriteData)); }
	virtual void setRect(RECT rect) { m_spriteData.rect = rect; }
	virtual void setRightRect(int rightRect) { m_spriteData.rect.right = rightRect; }
	virtual void setRotationCenter(V2 center) { m_spriteData.center = center; }
	virtual void setRow(uint8_t row) { m_row = row; }
	virtual void setRows(uint8_t rows) { m_spriteData.rows = rows; }
	virtual void setScalling(float scalling);
	virtual void setScallingX(float scallingX);
	virtual void setScallingY(float scallingY);
	virtual void setTexture(Texture* texture);
	virtual void setTopRect(int topRect) { m_spriteData.rect.top = topRect; }
	virtual void setUpdateDelay(float newm_updateDelay) { m_updateDelay = newm_updateDelay; }
	virtual void setWidth(uint16_t width) { m_spriteData.width = width; }
	virtual void setXY(V2 xy) { setX(xy.x).setY(xy.y); }
	virtual void xAdd(float value) { m_spriteData.x += value; }
	virtual void xDec(float value) { m_spriteData.x -= value; }
	virtual void yAdd(float value) { m_spriteData.y += value; }
	void mathUpdate();

protected:

	Texture* m_pTexture;
	Graphics* m_pGraphics;
	TextureManger* m_pTextureManger;
	SpriteData m_spriteData;
	float m_updateDelay, m_frameDelay;
	uint8_t m_column, m_row;
	bool m_animate;
	float m_sinA, m_cosA;
	ImageInfo* m_pImageInfo;

private:

	HANDLE m_threadHandle;
	int m_threadPara;
};

#endif