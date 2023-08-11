// texturemanger.cpp
// Author: abramann

#include "texturemanger.h"
#include "fileio.h"
#include "graphics.h"
#include "texture.h"

TextureManger::TextureManger()
{
}

TextureManger::~TextureManger()
{
}

bool TextureManger::initialize(Graphics* graphics)
{
	m_pGraphics = graphics;
	bool result = load();
	return result;
}

bool TextureManger::load()
{
	for (int i = 0; i < TEXTURES; i++)
	{
		bool result = m_pTexture[i].initialize(gameTexturePath[i], m_pGraphics);
		if (!result)
			return false;
	}

	return true;
}