#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "always.h"
#include <d3d9.h>
#include <d3dx9.h>

#define V2 D3DXVECTOR2
#define COLOR D3DCOLOR
#define COLOR_ARGB D3DCOLOR_ARGB
#define COLOR_XRGB D3DCOLOR_XRGB

constexpr auto COLOR_WHITE = COLOR_XRGB(255, 255, 255);

struct SpriteData
{
	LPDIRECT3DTEXTURE9 texture;
	int width, height;
	int textureWidth, textureHeight;
	int x, y;
	int columns, rows;
	float angle;
	float scalling;
	RECT rect;
	COLOR filterColor;
	V2 center;
};
class Graphics
{
public:

	Graphics();
	~Graphics();
	bool initialize(HWND _hWnd, bool _fullscreen);
	void spriteBegin();
	void spriteEnd();
	void spriteDraw(SpriteData sd);
	void release();
	HRESULT reset();
	bool loadTexture(const char* textureFile, int& width, int& height, COLOR _transpanceyColor, LPDIRECT3DTEXTURE9& texture);
	HRESULT getDeviceState();
	HRESULT begin();
	HRESULT end();
	HRESULT showBackbuffer();
	LPDIRECT3DDEVICE9 getDevice()		{ return device3d; }
	LPD3DXSPRITE getSprite()		{ return sprite; }

private:

	bool isAdaptereCompatility();
	DWORD getBehaviorCompatility();

	LPDIRECT3D9 direct3d;
	LPDIRECT3DDEVICE9 device3d;
	LPD3DXSPRITE sprite;
	D3DPRESENT_PARAMETERS pPresentParameter;
	HWND hWnd;
	bool fullscreen;
	DWORD deviceState;
};


#endif