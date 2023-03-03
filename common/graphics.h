#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "constants.h"
#include <d3d9.h>

class Graphics
{
public:

	Graphics();
	~Graphics();
	bool initialize(HWND hWnd);
	HRESULT createVertexBuffer(UINT length);
	void spriteBegin();
	void spriteEnd();
	void spriteDraw(SpriteData sd);
	void release();
	HRESULT reset();
	bool loadTexture(const char* file, UINT& width, UINT& height, Color transparency, LPDIRECT3DTEXTURE9& texture);
	void setTexture(LPDIRECT3DTEXTURE9 texture) { m_lpDevice3d->SetTexture(0, texture); }
	void setVertexBuffer(void* pData, size_t size);
	HRESULT createLine(ID3DXLine** line);
	HRESULT begin();
	HRESULT end();
	HRESULT showBackbuffer();
	HRESULT drawPrimitive(UINT startVertex, UINT count);
	LPDIRECT3DDEVICE9 getDevice()		{ return m_lpDevice3d; }
	LPD3DXSPRITE getSprite()		{ return m_sprite; }
	HRESULT getDeviceState();

private:

	bool isAdaptereCompatility();
	DWORD getBehaviorCompatility();

	LPDIRECT3D9 m_lpDirect3d;
	LPDIRECT3DDEVICE9 m_lpDevice3d;
	LPDIRECT3DVERTEXBUFFER9 m_lpVB;
	LPD3DXSPRITE m_sprite;
	D3DPRESENT_PARAMETERS m_PresentParameter;
	DWORD m_deviceState;
	uint32_t m_vertices;
};


#endif