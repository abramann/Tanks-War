#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "constants.h"
#include <d3d9.h>

class Image2;

struct Resolution
{
	uint16_t width, height;
};

class Graphics
{
public:

	Graphics();
	~Graphics();
	bool initialize(HWND hWnd);
	bool isFullScreen();
	bool loadTexture(const char* file, UINT& width, UINT& height, Color transparency, LPDIRECT3DTEXTURE9& texture);
	Matrix V3ToMatrix(const V3 v3, MatrixType type);
	Result begin();
	void drawImage(const Image2* image);
	Result drawPrimitive(UINT startVertex, UINT count);
	Result end();
	Result getDeviceState();
	Result reset();
	Result showBackbuffer();
	LPD3DXSPRITE getSprite()		{ return m_sprite; }
	LPDIRECT3DDEVICE9 getDevice()		{ return m_lpDevice3d; }
	LPVertexBuffer createVertexBuffer(uint32 vertices, int usage);
	Resolution getResolution();
	std::vector<Resolution> getSupportedResolutions();
	void release();
	void resize(uint16_t width, uint16_t height);
	void setDrawProperties(V3 position = V3(0,0,0), V3 scall = V3(1,1,1), V3 rotate = V3(0,0,0));
	void setFullScreen(const bool& fullscreen);
	void setResolution(const Resolution& resolution);
	void setTexture(LPDIRECT3DTEXTURE9 texture) { m_lpDevice3d->SetTexture(0, texture); }
	void setVertexBuffer(LPVertexBuffer, Vertex* vertez, int32_t numberOfVertices);
	void setVertexBufferUV(LPVertexBuffer vb, Vertex* vertez, int8_t len);

	void spriteBegin();
	void spriteDraw(SpriteData sd);
	void spriteEnd();
	void streamVertexBuffer(LPVertexBuffer vb);
	
private:

	bool isAdaptereCompatility();
	DWORD getBehaviorCompatility();

	LPDIRECT3D9 m_lpDirect3d;
	LPDIRECT3DDEVICE9 m_lpDevice3d;
	LPD3DXSPRITE m_sprite;
	D3DPRESENT_PARAMETERS m_PresentParameter;
	DWORD m_deviceState;
	HWND m_hwnd;
};


#endif