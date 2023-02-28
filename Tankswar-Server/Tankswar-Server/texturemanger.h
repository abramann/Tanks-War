#pragma once
#include "always.h"
#include "graphics.h"
#include "texture.h"
#include "server.h"

class TextureManger
{
public:

	TextureManger();
	~TextureManger();
	bool initialize(Graphics* graphics);
	bool load();
	Texture* getTexture(uint8_t texture) { return &m_pTexture[texture]; }
	Texture* getTiledMapTexture() const { return m_pTiledTexture; }
	TextureInfo* getTextureInfo(uint8_t texture)  { return &m_TextureInfo[texture]; }

private:

	Server m_server;
	Graphics* m_pGraphics;
	Texture* m_pTexture;
	Texture *m_pTiledTexture;
	std::vector<std::string> m_textureList;  
	std::vector<TextureInfo> m_TextureInfo;

};
