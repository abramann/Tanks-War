#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "always.h"
#include <d3d9.h>
#include <d3dx9.h>

#define V2 D3DXVECTOR2
#define Color D3DCOLOR
#define COLOR_WHITE D3DCOLOR_XRGB(255,255,255)

struct SpriteData
{
	LPDIRECT3DTEXTURE9 texture;
	int width, height;
	int extraWidth, extraHeight;
	INT textureWidth, textureHeight;
	int x, y;
	double angle;
	float scalling;
	RECT rect;
	Color filterColor;
	V2 center;
};
class Graphics
{
public:

	Graphics();
	~Graphics();
	HRESULT initialize(HWND _hWnd, bool _fullscreen);
	void spriteBegin();
	void spriteEnd();
	void spriteDraw(SpriteData sd);
	void release();
	HRESULT reset();
	bool loadTexture(char* textureFile, int& width, int& height, Color transpanceyC, LPDIRECT3DTEXTURE9& texture);
	HRESULT getDeviceState();
	HRESULT begin();
	HRESULT end();
	HRESULT showBackbuffer();

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