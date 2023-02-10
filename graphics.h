#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "always.h"
#include <d3d9.h>
#include <d3dx9.h>

struct Vertex 
{
	float x, y, z, rhw;
	float u, v;
};

struct TextureVertices
{
	Vertex v1, v2, v3;
	Vertex v4, v5, v6;
};

#define V2 D3DXVECTOR2
#define ZERO_V2(v2) (v2 == V2(0,0))
#define COLOR D3DCOLOR
#define COLOR_ARGB D3DCOLOR_ARGB
#define COLOR_XRGB D3DCOLOR_XRGB

constexpr auto  VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
constexpr auto COLOR_WHITE = COLOR_XRGB(255, 255, 255);

struct SpriteData
{
	LPDIRECT3DTEXTURE9 texture;
	int width, height;
	int textureWidth, textureHeight;
	float x, y;
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
	HRESULT createVertexBuffer(UINT length);
	void spriteBegin();
	void spriteEnd();
	void spriteDraw(SpriteData sd);
	void release();
	HRESULT reset();
	bool loadTexture(const char* textureFile, int& width, int& height, COLOR _transpanceyColor, LPDIRECT3DTEXTURE9& texture);
	void setTexture(LPDIRECT3DTEXTURE9 texture) { device3d->SetTexture(0, texture); }
	void setVertexBuffer(void* pData, UINT size);
	HRESULT createLine(ID3DXLine** line);
	HRESULT begin();
	HRESULT end();
	HRESULT showBackbuffer();
	HRESULT drawPrimitive(UINT startVertex, UINT count);
	LPDIRECT3DDEVICE9 getDevice()		{ return device3d; }
	LPD3DXSPRITE getSprite()		{ return sprite; }
	HRESULT getDeviceState();

private:

	bool isAdaptereCompatility();
	DWORD getBehaviorCompatility();

	LPDIRECT3D9 direct3d;
	LPDIRECT3DDEVICE9 device3d;
	LPDIRECT3DVERTEXBUFFER9 m_vb;
	LPD3DXSPRITE sprite;
	D3DPRESENT_PARAMETERS pPresentParameter;
	HWND hWnd;
	bool fullscreen;
	DWORD deviceState;
	UINT m_vertices;
};


#endif