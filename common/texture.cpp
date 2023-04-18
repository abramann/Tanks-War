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
	SAFE_RELEASE(m_lpTexture);
}

void Texture::getTextureName(char* name) const
{
	getFileNameFromPath(m_file, name);
}

void Texture::read()
{
}
