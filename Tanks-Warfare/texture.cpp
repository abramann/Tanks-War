#include "texture.h"

TextureManger::TextureManger()
{
	width = 0; height = 0;
}
TextureManger::~TextureManger()
{
	release();
}
bool TextureManger::initialize(const char* _textureFile, COLOR _transpanceyColor, Graphics* g)
{
	graphics = g;
	textureFile = _textureFile;
	transpanceyColor = _transpanceyColor;
	return graphics->loadTexture(textureFile, width, height, transpanceyColor, texture);
}
void TextureManger::onLostDevice()
{
	texture->Release();
}
void TextureManger::onResetDevice()
{
	graphics->loadTexture(textureFile, width, height, transpanceyColor, texture);
}
void TextureManger::release()
{
	if (texture != (LPDIRECT3DTEXTURE9)0xcdcdcdcd && texture != NULL)
		texture->Release();
}