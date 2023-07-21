// texturemanger.h
// Author: abramann

#pragma once

#include "types.h"
#include "texture.h"
#include <memory>

class Graphics;

constexpr char* gameTexturePath[] =
{
	"Assets\\texture\\logo.png",
	"Assets\\texture\\player-tank.png",
	"Assets\\texture\\enemy-tank.png",
	"Assets\\texture\\tank-destroy.png",
	"Assets\\texture\\bullet.png",
	"Assets\\texture\\bullet-destroy.png",
	"Assets\\texture\\tiled-0.png",
	"Assets\\texture\\tiled-1.png",
	"Assets\\texture\\tiled-2.png",
};

class TextureManger
{
public:

	TextureManger();
	~TextureManger();
	bool initialize(Graphics* graphics);
	Texture* getTexture(uint8_t texture) { return &m_pTexture[texture]; }

private:

	bool load();

	Graphics* m_pGraphics;
	Texture m_pTexture[TEXTURES];
};