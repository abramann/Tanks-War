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
	bool initialize(const Game* pGame);
	bool d3dInitialize();
	void d3dBegin();
	void d3dEnd();
	void d3dPresent();
	void d3dDraw(uint32 vertexCount, uint32 startVertex);
	void psSetSRV(ID3D11ShaderResourceView** ppSRV, uint32 numViews = 1, uint32 startSlot = 0);
	bool createSRVFromFile(const std::string& file, ID3D11ShaderResourceView ** ppSRV, int32& width, int32& height);
	Microsoft::WRL::ComPtr<ID3D11Buffer> createBuffer(D3D11_USAGE usage, D3D11_BIND_FLAG bindFlag, uint32 cpuAccess, uint32 byteWidth, uint32 stride = 0,
		void* initialData = 0, uint32 miscFlag = 0) const;
	Microsoft::WRL::ComPtr<ID3D11Buffer> createVertexBuffer(uint32 vertices, uint32 cpuAccess, Vertex* data = 0);
	Microsoft::WRL::ComPtr<ID3D11Buffer> createStructuredBuffer(uint32 elementSize, uint32 count, void* pInitData, int32 cpuAccess);
	Microsoft::WRL::ComPtr<ID3D11Buffer> createStagingBuffer(D3D11_CPU_ACCESS_FLAG cpuAccess, uint32 elementSize, uint32 count, void* pInitData);
	void setComputeShader(ID3D11ComputeShader* pComputeShader);
	void copyResourceToResource(ID3D11Resource* pDest, ID3D11Resource* pSource);
	void copyResource(void* pDest, ID3D11Resource* pSource, uint32 size);
	void copyToResource(ID3D11Resource* pDest, void* pSource, uint32 size);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> createBufferSRV(ID3D11Buffer* pBuffer);
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> createBufferUAV(ID3D11Buffer* pBuffer);
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> createComputeShader(const BYTE* pByteCode, size_t byteCodeSize);
	void runComputeShader(ID3D11ComputeShader* pComputeShader, uint32 SRVs, ID3D11ShaderResourceView** ppSRV,
		uint32 uavs, ID3D11UnorderedAccessView** ppUnorderedAccessView, uint32 x, uint32 y, uint32 z);
	void iaSetStreamBuffer(ID3D11Buffer * pVB, uint32 strides, uint32 offset, uint32 numBuffers = 1, uint32 startSlot = 0);
	void streamVertexBuffer(ID3D11Buffer* pVB);
	void vsSetConstBuffer(ID3D11Buffer* pVSConstBuffer, const void* data, uint32 slot);
	void onResize(int32 width, int32 height);
	std::vector<DXGI_MODE_DESC> enurmerateAdapterMode();
	void setFullScreen(bool fullscreen) const;

private:

	void initSwapChain(DXGI_SWAP_CHAIN_DESC& swapChainDesc);
	void initDepthStencil(D3D11_TEXTURE2D_DESC& depthStencilDesc);
	void initViewport(D3D11_VIEWPORT& viewport);
	void initSampleState(D3D11_SAMPLER_DESC& sampDesc);
	void initBlend(D3D11_RENDER_TARGET_BLEND_DESC& rtbd);

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampleState;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pStagingBuffer;

	HWND m_hwnd;
	bool m_initialzed;
};
