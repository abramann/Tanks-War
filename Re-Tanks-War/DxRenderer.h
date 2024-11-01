#pragma once

#include "IGame.h" // HWindow definition
#include "Renderer.h"
#include <set>
#include <stdint.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>




class CDxRenderer : public IRenderer
{
	const uint32_t DEFUSAGE_DATA_LIMIT = 128;

	enum PSCBSlot
	{
		PSCBSLOT_COLOR,
	};

	enum VSCBSlot
	{
		VSCBSLOT_PERFRAME,
		VSCBSLOT_PERDRAW,
	};

	struct VSCBPerFrame
	{
		Matrix wvm;
	};

	//__declspec(align(16)) 
	struct VSCBPerDraw
	{
		//uint32_t relativeToCamera;
		float position[4];
		float scaling[4];
		float rotate[4];
		float rotateCenter[4];
	};

	__declspec(align(16)) 
	struct PSCBPerDraw
	{
		uint color;
	};

public:
	CDxRenderer();
	~CDxRenderer();

	void initialize(HWindow hwnd) override;
	void beginRendering() override;
	void showBackbuffer() override;
	std::vector<AdapterMode> getAdapterModes() override;
	IBuffer* createVertexBuffer(uint32_t numVertices, Vertex * pInitData, int access) override;
	IBuffer* createIndexBuffer(uint32_t length, uint * pInitData, int access) override;
	void releaseBuffer(IBuffer* pBuffer) override;
	void setVSDrawProperties(Vertex position = Vertex(0, 0, 0), Vertex scaling = Vertex(1, 1, 1), Vertex rotate = Vertex(0, 0, 0), Vertex rotateCenter = Vertex(0, 0, 0)) const;
	void setPSDrawProperties(Color color = Color(255, 255, 255, 255)) const;
	void renderMap() const override;
	void drawSprite(const CSprite* pSprite) const override;
	void drawModel(const CModel* pModel) const override;
	void setWorldViewMatrix(Matrix* mat) override;
	ITexture* loadTextureFromFile(const wchar_t* texFileName) override;
	void releaseTexture(ITexture* pTexture) override;

	void copyResourceToResource(ID3D11Resource * pDest, ID3D11Resource * pSource) const;
	void copyResource(void * pDest, ID3D11Resource * pSource, uint32_t size) const;
	void copyToResource(ID3D11Resource * pDest, void * pSource, uint32_t size) const;
private:

	ID3D11Buffer * createBuffer(D3D11_BUFFER_DESC desc, void* pInitData);

	bool m_initialized;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	mutable Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampleState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pPSColorBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVSInSSOBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVBPerFrame;
};

extern CDxRenderer * g_pDxRenderer;