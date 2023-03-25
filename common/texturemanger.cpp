#include "texturemanger.h"
#include "fileio.h"
#include "graphics.h"

TextureManger::TextureManger()
{
}

const DWORD INVALID_PTR = 0xcdcdcdcd;

TextureManger::~TextureManger()
{
	if(m_pTexture!= (Texture*)INVALID_PTR)
	SAFE_DELETE_ARRAY(m_pTexture);
}

bool TextureManger::initialize(Graphics* graphics)
{
	m_pGraphics = graphics;
	m_textures = FileIO::getDirFiles(TEXTURE_DIR);
	m_pTexture = new Texture[m_textures];
	return load();

}
constexpr auto MAX_FILE_NAME_LEN = 30;

bool TextureManger::load()
{
	m_textureList = FileIO::getDirFileList(TEXTURE_DIR);
	uint8_t counter = 0;
	char name[MAX_FILE_NAME_LEN] = { 0 };
	for (auto item : m_textureList)
	{
		item.insert(0, TEXTURE_DIR);
	//	Texture texture;
		if (!m_pTexture[counter].initialize(item.c_str(), m_pGraphics))
			throw GameError(gameErrorNS::FATAL_ERROR, item.c_str());

		m_pTexture[counter].setNumber(counter);
	//	m_pTexture[counter] = texture;
	//	texture.release();
		m_pTexture[counter].getTextureName(name);
		if (strComp(name, "tiled-0") == 0)
			m_pTiled = &m_pTexture[counter];
		else if (strComp(name, "logo") == 0)
			m_pLogo = &m_pTexture[counter];
		
		TextureInfo& textureInfo = *FileIO::readTextureInfo(name);
		m_TextureInfo.push_back(textureInfo);
		counter++;
	}
	memset(name, 0, MAX_NAME_LEN);
	
	return true;
}
