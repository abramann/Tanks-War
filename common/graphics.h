#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include "constants.h"

class Image2;
class Camera;

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
	bool loadTexture(const char* file, UINT& width, UINT& height, Color transparency, LPTextureD3D& texture);
	Matrix V3ToMatrix(const V3 v3, MatrixType type);
	Result begin();
	void drawImage(const Image2* image);
	Result drawPrimitive(UINT startVertex, UINT count);
	Result end();
	Result getDeviceState();
	Result reset();
	Result showBackbuffer();
//	LPD3DXSPRITE getSprite()		{ return  m_sprite; }
//	LPDIRECT3DDEVICE9 getDevice()		{ return m_lpDevice3d; }
	LPVertexBuffer createVertexBuffer(uint32 vertices, VB_USAGE usage, Vertex* data = 0);
	Resolution getResolution();
	std::vector<Resolution> getSupportedResolutions();
	void release();
	void resize(uint16_t width, uint16_t height);
	void setDrawProperties(V3 position = V3(0, 0, 0), V3 scall = V3(1, 1, 1), V3 rotate = V3(0, 0, 0), V3 rotateCenter = V3(0, 0, 0));
	void setFullScreen(const bool& fullscreen);
	void setResolution(const Resolution& resolution);
	void setTexture(LPTextureD3D texture);
	void setVertexBuffer(LPVertexBuffer, Vertex* vertez, int32_t numberOfVertices);
	void setVertexBufferUV(LPVertexBuffer vb, Vertex* vertez, int8_t len);
	void setWorldMatrix(Matrix worldMatrix);
	void spriteBegin();
	void spriteDraw(SpriteData sd);
	void spriteEnd();
	void streamVertexBuffer(LPVertexBuffer vb);
	Camera* getCamera() const { return m_pCamera; }

private:

	bool isAdaptereCompatility();
	DWORD getBehaviorCompatility();

#ifdef _BUILD_WITH_D3D9
	LPDIRECT3D9 m_lpDirect3d;
	LPDIRECT3DDEVICE9 m_lpDevice3d;
	LPD3DXSPRITE m_sprite;
	D3DPRESENT_PARAMETERS m_PresentParameter;
#else ifdef _BUILD_WITH_D3D11
	ID3D11Device* m_lpDevice3d;
	ID3D11DeviceContext* m_lpDeviceContext;
	IDXGISwapChain* m_lpSwapChain;
	ID3D11RenderTargetView* m_lpRenderTargetView;
	ID3D11DepthStencilView* m_lpDepthStencilView;
	ID3D11Texture2D* m_lpDepthBuffer;
	ID3D11Buffer* m_lpVSConstBuffer;
	ID3D11VertexShader* m_lpVShader;
	ID3D11PixelShader* m_lpPShader;
	ID3D11InputLayout* m_lpInputLayout;
	ID3D11SamplerState* m_lpSampleState;
	ID3D11BlendState* m_lpBlendState;
	D3DXMATRIX m_wvp;

	void setVSConstBuffer(const void* data);
#endif
	DWORD m_deviceState;
	HWND m_hwnd;
	Camera* m_pCamera;
};


#endif