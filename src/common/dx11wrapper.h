// dx11wrapper.h
// Author: abramann

#pragma once

#include "types.h"
#include <wrl\client.h>

class Game;

class Dx11Wrapper
{
public:

	Dx11Wrapper();
	~Dx11Wrapper();
	bool initialize(const Game* game);
	bool d3dInitialize();
	void d3dBegin();
	void d3dEnd();
	void d3dPresent();
	void d3dDraw(uint32 vertexCount, uint32 startVertex);
	void d3dSetSRV(ID3D11ShaderResourceView* pSrv, uint32 numViews = 1, uint32 startSlot = 0);
	bool loadSRVFromFile(const std::string& file, ID3D11ShaderResourceView *& srv, int32& width, int32& height);
	ID3D11Buffer * Dx11Wrapper::createBuffer(uint32 size, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlag, uint32 cpuAccess, void* initialData = 0,
		uint32 miscFlag = 0, uint32 stride = 0) const;
	ID3D11Buffer* createVertexBuffer(uint32 vertices, Vertex* data = 0);
	void d3dStreamVertexBuffer(ID3D11Buffer * vertexBuffer, uint32 strides, uint32 offset, uint32 numBuffers = 1, uint32 startSlot = 0);
	void setVertexBuffer(ID3D11Buffer* vb, Vertex * vertex, uint32 vertices);
	void setVertexBufferUV(LPVertexBuffer vb, Vertex * vertex, int8 len);
	void streamVertexBuffer(LPVertexBuffer vb);
	void setVSConstBuffer(const void* data);

private:

	void initSwapChain(DXGI_SWAP_CHAIN_DESC& swapChainDesc);
	void initDepthStencil(D3D11_TEXTURE2D_DESC& depthStencilDesc);
	void initViewport(D3D11_VIEWPORT& viewport);
	void initSampleState(D3D11_SAMPLER_DESC& sampDesc);
	void initBlend(D3D11_RENDER_TARGET_BLEND_DESC& rtbd);
	Vertex* getVertexBufferData(LPVertexBuffer vb, uint32& size) const;

	Microsoft::WRL::ComPtr<ID3D11Device> m_lpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_lpDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_lpSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_lpRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_lpDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_lpDepthBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_lpVSConstBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_lpVShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_lpPShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_lpInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_lpSampleState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_lpBlendState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_lpStagingBuffer;

	HWND m_hwnd;
	bool m_initialzed;
};
