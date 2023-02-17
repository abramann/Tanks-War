#ifndef _TEXTURE_H
#define _TEXTURE_H
#include "always.h"
#include "graphics.h"

struct TextureInfo
{
	uint8_t columns, rows;
	uint16_t width, height;
	Color transparency;
};

class TextureManger
{
public:

	TextureManger();
	~TextureManger();
	bool initialize(const char* file, Graphics* graphics);
	void onLostDevice();
	void onResetDevice();
	void release();
	LPDIRECT3DTEXTURE9 getTexture() const	{ return m_lpTexture; }
	UINT getWidth()	const	{ return m_width; }
	UINT getHeight() const	{ return m_height; }
	uint16_t getImageWidth() const	{ return m_textureInfo.width; }
	uint16_t getImageHeight() const	{ return m_textureInfo.height; }
	uint8_t getColumns() const	{ return m_textureInfo.columns; }
	uint8_t getRows() const	{ return m_textureInfo.rows; }
	

protected:

	void read();
	const char* m_file;
	Graphics* m_pGraphics;
	LPDIRECT3DTEXTURE9 m_lpTexture;
	UINT m_width, m_height;
	TextureInfo m_textureInfo;
};


#endif