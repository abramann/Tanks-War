// texture.cpp
// Author: abramann
// Note this file is influenced by texture.cpp from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#include "texture.h"
#include "fileio.h"
#include "inlined.inl"

Texture::Texture() : m_width(0), m_height(0), m_lpTexture(NULL)
{
}

Texture::~Texture()
{
	release();
}

bool Texture::initialize(const std::string& file, Graphics* pGraphics)
{
	m_pGraphics = pGraphics;
	m_file = file;
	return m_pGraphics->loadTexture(m_file.c_str(), m_width, m_height, m_lpTexture);
}

void Texture::release()
{
	safeRelease(m_lpTexture);
}