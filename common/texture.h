#ifndef _TEXTURE_H
#define _TEXTURE_H
#include "constants.h"
#include "graphics.h"

class Texture
{
public:

	Texture();
	~Texture();
	bool initialize(const char* file, Graphics* graphics);
	void onLostDevice();
	void onResetDevice();
	void release();
	void Texture::read();
	LPTextureD3D getTexture() const { return m_lpTexture; }
	uint16_t getWidth()	const	{ return m_width; }
	uint16_t getHeight() const	{ return m_height; }
	std::string getFilePath() const { return m_file; };
	void getTextureName(char* name) const;
	uint8_t getNumber() const { return m_number; }

	/*
	uint16_t getImageWidth() const	{ return m_imageInfo.width; }
	uint16_t getImageHeight() const	{ return m_imageInfo.height; }
	uint8_t getColumns() const	{ return m_imageInfo.columns; }
	uint8_t getRows() const	{ return m_imageInfo.rows; }
	bool getAnimate()const { return m_imageInfo.animate; }
	float getAnimateSpeed() const { return m_imageInfo.animateSpeed; }
	float getTextureScalling() const { return m_imageInfo.scalling; }
	*/

	void setNumber(uint8_t number) { m_number = number; }

protected:

	std::string m_file;
	Graphics* m_pGraphics;
	LPTextureD3D m_lpTexture;
	uint32_t m_width, m_height;
	uint8_t m_number;

};


#endif