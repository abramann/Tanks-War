// texture.cpp
// Author: abramann
// Note this file is influnced by texture.cpp from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0 

#include "texture.h"
#include <fstream>
#include "fileio.h"

Texture::Texture() : m_width(0), m_height(0), m_lpTexture(NULL)
{
}

Texture::~Texture()
{
	release();
}

bool Texture::initialize(const char* file, Graphics* graphics)
{
	bool result = false;
	m_pGraphics = graphics;
	m_file = file;
	result = m_pGraphics->loadTexture(m_file.c_str(), m_width, m_height, COLOR_ALPHA, m_lpTexture);
	return result;
}

void Texture::onLostDevice()
{
	release();
}

void Texture::onResetDevice()
{
	m_pGraphics->loadTexture(m_file.c_str(), m_width, m_height, COLOR_BLACK, m_lpTexture);
}

void Texture::release()
{
	safeRelease(m_lpTexture);
}