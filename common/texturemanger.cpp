#include "texturemanger.h"
#include "fileio.h"
#include "graphics.h"

TextureManger::TextureManger()
{
}


TextureManger::~TextureManger()
{
}

bool TextureManger::initialize(Graphics* graphics)
{
	m_pGraphics = graphics;
	uint8_t textures = FileIO::getDirFiles(TEXTURE_DIR);
	m_pTexture = new Texture[textures];
//	m_TextureInfo = new TextureInfo*[textures];
	return load();
}

bool TextureManger::load()
{
	m_textureList = FileIO::getDirFileList(TEXTURE_DIR);
	Texture texture;

	uint8_t counter = 0;
	for (auto item : m_textureList)
	{
		ZeroMemory(&texture, sizeof(texture));
		item.insert(0, TEXTURE_DIR);
		if (!texture.initialize(item.c_str(), m_pGraphics))
			throw GameError(gameErrorNS::FATAL_ERROR, item.c_str());

		texture.setNumber(counter);
		m_pTexture[counter] = texture;
		if (strComp(texture.getTextureName(), "tiled-0") == 0)
			m_pTiled = &m_pTexture[counter];
		else if (strComp(texture.getTextureName(), "logo") == 0)
			m_pLogo = &m_pTexture[counter];
		
		TextureInfo textureInfo;
		textureInfo = *FileIO::readTextureInfo(texture.getTextureName());
		m_TextureInfo.push_back(textureInfo);
		counter++;
	}

	return true;
}
