// texture.h
// Author: abramann
// Note this file is influenced by texture.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

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
	LPTextureD3D getTexture() const { return m_lpTexture; }
	int32 getWidth()	const { return m_width; }
	int32 getHeight() const { return m_height; }

protected:

	std::string m_file;
	Graphics* m_pGraphics;
	LPTextureD3D m_lpTexture;
	uint32 m_width, m_height;
};

#endif