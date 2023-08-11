// texture.h
// Author: abramann
// Note this file is influenced by texture.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#pragma once

#include "types.h"
#include "graphics.h"

class Texture
{
public:

	Texture();
	~Texture();
	bool initialize(std::string file, Graphics* graphics);
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
	int32 m_width, m_height;
};