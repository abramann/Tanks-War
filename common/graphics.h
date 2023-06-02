// graphics.h
// Author: abramann
// Note this file is influenced by graphics.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0 

#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include "constants.h"
#include <memory>

class Game;
class Image;
class Camera;

#ifdef _BUILD_WITH_D3D9
typedef LPDIRECT3DDEVICE9 LPDevice;
#else ifdef _BUILD_WITH_D3D11
typedef ID3D11Device* LPDevice;
#endif
struct Resolution
{
	int16 width, height;
};

class Graphics
{
public:

	friend class Camera;
	
	Graphics();
	~Graphics();
	bool initialize(const Game* game);
	bool isWindowed() const;
	bool loadTexture(const char* file, UINT& width, UINT& height, Color transparency, LPTextureD3D& texture);
	static Matrix V3ToMatrix(const V3 v3, MatrixType type);
	Result begin();
	void drawImage(const Image* image);
	Result drawPrimitive(UINT startVertex, UINT count);
	Result end();
	Result showBackbuffer();
	LPDevice getDevice() const { return m_lpDevice3d; }
	LPVertexBuffer createVertexBuffer(uint32 vertices, VB_USAGE usage, Vertex* data = 0);
	Resolution getResolution();
	std::vector<Resolution> getSupportedResolutions() const;
	void release();
	void resize(uint16_t width, uint16_t height);
	void setDrawProperties(V3 position = V3(0, 0, 0), V3 scall = V3(1, 1, 1), V3 rotate = V3(0, 0, 0), V3 rotateCenter = V3(0, 0, 0));
	void setWindowed(const bool& windowed);
	void setResolution(const Resolution& resolution);
	void setTexture(LPTextureD3D texture);
	void setVertexBuffer(LPVertexBuffer, Vertex* vertez, int32_t numberOfVertices);
	void setVertexBufferUV(LPVertexBuffer vb, Vertex* vertez, int8_t len);
	void setWorldMatrix(Matrix worldMatrix);
	void streamVertexBuffer(LPVertexBuffer vb);
	bool checkFullscreenSupport() const;
	Camera* getCamera() const { return m_pCamera.get(); }

private:

	void setViewMatrix(Matrix wvp);
	LPDevice m_lpDevice3d;

#ifdef _BUILD_WITH_D3D9
	LPDIRECT3D9 m_lpDirect3d;
	D3DPRESENT_PARAMETERS m_presentParameter;
#else ifdef _BUILD_WITH_D3D11
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
	ID3D11Buffer* m_lpStagingBuffer;
	D3DXMATRIX m_wvp;

	void setVSConstBuffer(const void* data);
	Vertex* getVertexBufferData(LPVertexBuffer vb, uint32& size) const;
#endif

	DWORD m_deviceState;
	std::shared_ptr<Camera> m_pCamera;
};


#endif