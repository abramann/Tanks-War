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
	m_pGraphics = graphics;
	m_file = file;
	HRESULT hr = m_pGraphics->loadTexture(m_file.c_str(), m_width, m_height, COLOR_ALPHA, m_lpTexture);
	if (FAILED(hr))
		return false;
	return true;
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

std::string Texture::getTextureName() const
{
	const char* qw = m_file.c_str();
	return getFileNameFromPath(m_file);
}

void Texture::read()
{
}
