#ifndef _TEXTURE_H
#define _TEXTURE_H
#include "always.h"
#include "graphics.h"

class TextureManger
{
public:

	TextureManger();
	~TextureManger();
	bool initialize(char* tFile, COLOR transpanceyColor, Graphics* g);
	void onLostDevice();
	void onResetDevice();
	void release();

	Graphics* getGraphics()		{ return graphics; }
	LPDIRECT3DTEXTURE9 getTexture()		{ return texture; }
	int getWidth()		{ return width; }
	int getHeight()		{ return height; }

private:

	Graphics* graphics;
	char* textureFile;
	COLOR transpanceyColor;
	LPDIRECT3DTEXTURE9 texture;
	int width, height;
};


#endif