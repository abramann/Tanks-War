// texturemanger.cpp
// Author: abramann

#include "texturemanger.h"
#include "fileio.h"
#include "graphics.h"
#include "texture.h"
#include "inlined.inl"

TextureManger::TextureManger()
{
}

TextureManger::~TextureManger()
{
}

void TextureManger::initialize(Graphics* graphics)
{
	m_pGraphics = graphics;
	load();
}

void TextureManger::load()
{
	auto textures = FileIO::getDirFileList(fileNS::TEXTURE_DIR, 0, ".png", false);
	int8 notLoaded = 0;
	for (auto texture : textures)
	{
		std::string texFile = strFormat("%s%s%s", fileNS::TEXTURE_DIR, texture.c_str(), ".png");
		bool result = m_pTexture[texture].initialize(texFile, m_pGraphics);
		if (!result)
		{
			std::string err = strFormat("Failed to load texture %s", texFile.c_str());
			messageBoxOk(err, "WARNING");
			notLoaded++;
		}
	}

	if (notLoaded)
	{
		std::string msg = strFormat("%d textures have not been loaded!", notLoaded);
		messageBoxOk(msg, "WARNING");
	}
}