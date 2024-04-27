#include "DxRenderer.h"
#include "GameError.h"
#include "IGame.h"
#include "GameData.h"
#include "String.h"
#include "HLSLVertexShader.h"
#include "HLSLPixelShader.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui_impl_dx11.h"
#include "DxBuffer.h"
#include "DxTexture.h"
#include "IImage.h"
#include "CTankModel.h"
#include "DirectXTK\WICTextureLoader.h"
#include <map>


using namespace DirectX;

CDxRenderer* g_pDxRenderer = nullptr;

using namespace Microsoft::WRL;

std::vector<AdapterMode_t> dxgiGetAdapterModes();

void initSwapChainWindowed(DXGI_SWAP_CHAIN_DESC& swapChainDesc);

void initSwapChain(DXGI_SWAP_CHAIN_DESC& swapChainDesc, HWindow hwnd);

void initDepthStencil(D3D11_TEXTURE2D_DESC& depthStencilDesc);

void initViewport(D3D11_VIEWPORT& viewport);

void initSampleState(D3D11_SAMPLER_DESC& sampDesc);

void initBlend(D3D11_BLEND_DESC& blendDesc);

void initWCBuffer(D3D11_BUFFER_DESC& desc, uint32_t size);

CDxRenderer::CDxRenderer() : m_initialized(false)
{
	g_pDxRenderer = this;
}


CDxRenderer::~CDxRenderer()
{
	m_pSwapChain->SetFullscreenState(false, nullptr);
	if (m_initialized)
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
		m_initialized = false;
	}
}

void CDxRenderer::initialize(HWindow hwnd)
{
	// Initializing swapchain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	initSwapChain(swapChainDesc, hwnd);
	uint32_t flags = NULL;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	CHECK_ERROR(SUCCEEDED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, m_pSwapChain.GetAddressOf(),
		m_pDevice.GetAddressOf(), NULL, m_pDeviceContext.GetAddressOf())), "D3D11CreateDeviceAndSwapChain failed");
	
	m_backbufferWidth = swapChainDesc.BufferDesc.Width;
	m_backbufferHeight = swapChainDesc.BufferDesc.Height;

	// Initializing render target view
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);
	pBackBuffer->Release();

	// Initializing depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	initDepthStencil(depthStencilDesc);
	m_pDevice->CreateTexture2D(&depthStencilDesc, NULL, m_pDepthBuffer.GetAddressOf());
	CHECK_ERROR(SUCCEEDED(m_pDevice->CreateDepthStencilView(m_pDepthBuffer.Get(), NULL, &m_pDepthStencilView)), "CreateDepthStencilView failed");
	
	// Initializing viewport
	D3D11_VIEWPORT viewport;
	initViewport(viewport);
	m_pDeviceContext->RSSetViewports(1, &viewport);

	// Initializing vertex shader
	m_pDevice->CreateVertexShader(g_pHLSLVertexShader, ARRAYSIZE(g_pHLSLVertexShader), nullptr, m_pVertexShader.GetAddressOf());
	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);

	// Initializing input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), g_pHLSLVertexShader, ARRAYSIZE(g_pHLSLVertexShader), m_pInputLayout.GetAddressOf());
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Initializing pixel shader
	m_pDevice->CreatePixelShader(g_pHLSLPixelShader, ARRAYSIZE(g_pHLSLPixelShader), 0, m_pPixelShader.GetAddressOf());
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	// Initializing sampler and binding it to pixel shader
	D3D11_SAMPLER_DESC sampDesc;
	initSampleState(sampDesc);
	m_pDevice->CreateSamplerState(&sampDesc, m_pSampleState.GetAddressOf());
	m_pDeviceContext->PSSetSamplers(0, 1, m_pSampleState.GetAddressOf());

	// Initializing blend
	D3D11_BLEND_DESC blendDesc;
	initBlend(blendDesc);
	m_pDevice->CreateBlendState(&blendDesc, m_pBlendState.GetAddressOf());

	float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), blendFactor, 0xffffffff);

	// Initializing ImGui
	ImGui::CreateContext();
	CHECK_ERROR(ImGui_ImplWin32_Init(hwnd), "ImGui_ImplWin32_Init failed");
	CHECK_ERROR(ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get()), "ImGui_ImplDX11_Init failed");
	ImGui::GetIO().IniFilename = 0;

	// Initializing buffer to hold object info
	D3D11_BUFFER_DESC bufDesc;
	initWCBuffer(bufDesc, sizeof(CBPerDraw));
	m_pVSInSSOBuffer = createBuffer(bufDesc, nullptr);

	// Initializing color buffer
	initWCBuffer(bufDesc, 16);
	m_pPSColorBuffer = createBuffer(bufDesc, nullptr);

	m_initialized = true;
}

void CDxRenderer::beginRendering()
{
	float bgColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), bgColor);
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
	IRenderer::beginRendering();
}

void CDxRenderer::showBackbuffer()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	m_pSwapChain->Present(0, 0);
}

std::vector<AdapterMode_t> CDxRenderer::getAdapterModes()
{
	return dxgiGetAdapterModes();
}

IBuffer * CDxRenderer::createVertexBuffer(uint32_t numVertices, Vertex * pInitData, int access)
{
	D3D11_BUFFER_DESC desc;
	setNull(desc);

	desc.ByteWidth = numVertices * sizeof(Vertex) * 2;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.StructureByteStride = sizeof(Vertex) * 2;

	switch (access)
	{
	case BUFFER_IMMUTABLE:
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		break;
	case BUFFER_READ_ONLY:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		break;
	case BUFFER_WRITE_ONLY:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		break;
	case BUFFER_READ_WRITE:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		break;
	default:
		CHECK_ERROR(true, "Invalid access passed");
	}
	ID3D11Buffer * pNewDXOBuffer = createBuffer(desc, pInitData);
	CDxBuffer * pNewVertexBuffer = new CDxBuffer;
	pNewVertexBuffer->assign(pNewDXOBuffer);
	return pNewVertexBuffer;
}

IBuffer * CDxRenderer::createIndexBuffer(uint32_t length, int * pInitData, int access)
{
	D3D11_BUFFER_DESC desc;
	setNull(desc);

	desc.ByteWidth = length;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.StructureByteStride = sizeof(Vertex);

	switch (access)
	{
	case BUFFER_IMMUTABLE:
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		break;
	case BUFFER_READ_ONLY:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		break;
	case BUFFER_WRITE_ONLY:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		break;
	case BUFFER_READ_WRITE:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		break;
	default:
		CHECK_ERROR(true, "Invalid access passed");
	}
	ID3D11Buffer * pNewDXOBuffer = createBuffer(desc, pInitData);
	CDxBuffer * pNewIndexBuffer = new CDxBuffer;
	pNewIndexBuffer->assign(pNewDXOBuffer);
	return pNewIndexBuffer;
}

void CDxRenderer::releaseBuffer(IBuffer * pBuffer)
{
	if (pBuffer)
	{
		safeDelete(pBuffer);
	}
}

void CDxRenderer::drawImage(const IImage * pImage) const
{
	IBuffer* pVB = pImage->getVertexBuffer();
	ID3D11Buffer* pDXOVB = static_cast<ID3D11Buffer*>(pVB->getBufferObject());
	IBuffer* pIB = pImage->getIndexBuffer();
	ID3D11Buffer* pDXOIB = static_cast<ID3D11Buffer*>(pIB->getBufferObject());

	uint32_t color = pImage->getColor();
	uint32_t perVertexSize = sizeof(Vertex) * 2;
	uint32_t offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &pDXOVB, &perVertexSize, &offset);
	m_pDeviceContext->IASetIndexBuffer(pDXOIB, DXGI_FORMAT_R32_UINT, 0);

	CBPerDraw cbImg;
	setNull(cbImg);
	cbImg.relativeToCamera = pImage->isRelativeToCamera();
	cbImg.position = pImage->getPosition();
	cbImg.rotate = pImage->getRotate();
	cbImg.scaling = pImage->getScaling();

	copyToResource(m_pVSInSSOBuffer.Get(), &cbImg, sizeof(CBPerDraw));
	m_pDeviceContext->VSSetConstantBuffers(VSCBSLOT_PERDRAW, 1, m_pVSInSSOBuffer.GetAddressOf());

	copyToResource(m_pPSColorBuffer.Get(), &color, sizeof(Color));
	m_pDeviceContext->PSSetConstantBuffers(PSCBSLOT_COLOR, 1, m_pPSColorBuffer.GetAddressOf());
	ITexture* pTexture = pImage->getTexture();
	if (pTexture)
	{
		ID3D11ShaderResourceView* pSRV = static_cast<ID3D11ShaderResourceView*>(pTexture->getResourceObject());
		m_pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	}
	m_pDeviceContext->DrawIndexed(6, 0, 0);
}

void CDxRenderer::drawTank(const CTankModel* pTankModel) const
{
	IBuffer* pVB = pTankModel->getVertexBuffer();
	ID3D11Buffer* pDXOVB = static_cast<ID3D11Buffer*>(pVB->getBufferObject());
	IBuffer* pIB = pTankModel->getIndexBuffer();
	ID3D11Buffer* pDXOIB = static_cast<ID3D11Buffer*>(pIB->getBufferObject());

	uint32_t perVertexSize = sizeof(Vertex) * 2;
	uint32_t offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &pDXOVB, &perVertexSize, &offset);
	m_pDeviceContext->IASetIndexBuffer(pDXOIB, DXGI_FORMAT_R32_UINT, 0);
	
	CBPerDraw cbTank;
	setNull(cbTank);
	cbTank.position = pTankModel->getPosition();
	copyToResource(m_pVSInSSOBuffer.Get(), &cbTank, sizeof(CBPerDraw));
	m_pDeviceContext->VSSetConstantBuffers(VSCBSLOT_PERDRAW, 1, m_pVSInSSOBuffer.GetAddressOf());

	uint32_t color = pTankModel->getColor();
	copyToResource(m_pPSColorBuffer.Get(), &color,sizeof(Color));
	m_pDeviceContext->PSSetConstantBuffers(PSCBSLOT_COLOR, 1, m_pPSColorBuffer.GetAddressOf());

	int baseIndex = 0;
	ID3D11ShaderResourceView* pSRV;
	int numChains;
	auto ppChains = pTankModel->getChainsTextures(numChains);
	for (int i = 0; i < numChains; i++)
	{
		pSRV = static_cast<ID3D11ShaderResourceView*>(ppChains[i]->getResourceObject());
		m_pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
		m_pDeviceContext->DrawIndexed(6, baseIndex += 6, 0);
		baseIndex += 6;
	}
	cbTank.rotate = pTankModel->getBodyRotate();
	cbTank.rotateCenter = pTankModel->getBodyRotateCenter();
	copyToResource(m_pVSInSSOBuffer.Get(), &cbTank, sizeof(CBPerDraw));
	m_pDeviceContext->VSSetConstantBuffers(VSCBSLOT_PERDRAW, 1, m_pVSInSSOBuffer.GetAddressOf());
	auto pBody = pTankModel->getBodyTexture();
	pSRV = static_cast<ID3D11ShaderResourceView*>(pBody->getResourceObject());
	m_pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	m_pDeviceContext->DrawIndexed(6, baseIndex += 6, 0);
	
	cbTank.rotate = pTankModel->getGunRotate();
	cbTank.rotateCenter = pTankModel->getGunRotateCenter();
	copyToResource(m_pVSInSSOBuffer.Get(), &cbTank, sizeof(CBPerDraw));
	m_pDeviceContext->VSSetConstantBuffers(VSCBSLOT_PERDRAW, 1, m_pVSInSSOBuffer.GetAddressOf());
	auto pGun = pTankModel->getBodyTexture();
	pSRV = static_cast<ID3D11ShaderResourceView*>(pGun->getResourceObject());
	m_pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	m_pDeviceContext->DrawIndexed(6, baseIndex += 6, 0);
}

ITexture * CDxRenderer::loadTextureFromFile(const wchar_t * texFileName)
{
	wchar_t texPath[256] = { 0 };
	wsprintfW(texPath, L"Assets\\Textures\\%s", texFileName);
	static std::map <std::wstring, ITexture*> loaded;

	// Check if texture already loaded
	if (loaded[texFileName] != nullptr)
	{
		return loaded[texFileName];
	}

	// Load the texture
	ID3D11Resource* pResource;
	ID3D11ShaderResourceView* pSRV;
	CHECK_ERROR(SUCCEEDED(CreateWICTextureFromFile(m_pDevice.Get(), texPath, &pResource, &pSRV)), strFormat("Failed to load %s", texPath).c_str());
	CDxTexture* pNewTexture = new CDxTexture;
	pNewTexture->assign(pResource, pSRV);
	loaded[texFileName] = pNewTexture;
	return pNewTexture;
}

void CDxRenderer::releaseTexture(ITexture * pTexture)
{
	if (pTexture)
	{
		safeDelete(pTexture);
	}
}

void CDxRenderer::copyResourceToResource(ID3D11Resource * pDest, ID3D11Resource * pSource) const
{
	m_pDeviceContext->CopyResource(pDest, pSource);
}

void CDxRenderer::copyResource(void * pDest, ID3D11Resource * pSource, uint32_t size) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	setNull(mappedResource);

	m_pDeviceContext->Map(pSource, 0, D3D11_MAP_READ, 0, &mappedResource);
	memcpy(pDest, mappedResource.pData, size);
	m_pDeviceContext->Unmap(pSource, 0);
}

void CDxRenderer::copyToResource(ID3D11Resource * pDest, void * pSource, uint32_t size) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	setNull(mappedResource);

	m_pDeviceContext->Map(pDest, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, pSource, size);
	m_pDeviceContext->Unmap(pDest, 0);
}

ID3D11Buffer * CDxRenderer::createBuffer(D3D11_BUFFER_DESC desc, void* pInitData)
{
	ID3D11Buffer * pNewBuffer = nullptr;
	D3D11_SUBRESOURCE_DATA srData;
	setNull(srData);

	if (!pInitData)
		m_pDevice->CreateBuffer(&desc, 0, &pNewBuffer);
	else
	{
		srData.pSysMem = pInitData;
		m_pDevice->CreateBuffer(&desc, &srData, &pNewBuffer);
	}

	CHECK_ERROR(pNewBuffer, "Failed to create buffer");
	return pNewBuffer;
}

std::vector<AdapterMode_t> dxgiGetAdapterModes()
{
	static std::vector<AdapterMode_t> adapterModes;
	if (adapterModes.empty())
	{
		ComPtr<IDXGIFactory> pFactory;
		CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
		ComPtr<IDXGIAdapter> pAdapter;
		ComPtr<IDXGIOutput> pAdapterOutput;

		pFactory->EnumAdapters(0, &pAdapter);
		pAdapter->EnumOutputs(0, &pAdapterOutput);

		uint32_t numModes;
		pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&numModes, NULL);

		DXGI_MODE_DESC* pDisplayModeList = new DXGI_MODE_DESC[numModes];
		pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&numModes, pDisplayModeList);

		for (uint32_t i = 0; i < numModes; i += 2)
		{
			if (pDisplayModeList[i].Width >= nsGameConfig::MIN_WINDOW_WIDTH && pDisplayModeList[i].Height >= nsGameConfig::MIN_WINDOW_HEIGHT)
			{
				AdapterMode_t adapterMode;
				adapterMode.width = pDisplayModeList[i].Width;
				adapterMode.height = pDisplayModeList[i].Height;
				adapterMode.format = pDisplayModeList[i].Format;
				adapterModes.push_back(adapterMode);
			};
		}
		safeDeleteArray(pDisplayModeList);
	}

	return adapterModes;
}

void initSwapChainWindowed(DXGI_SWAP_CHAIN_DESC& swapChainDesc)
{
	DXGI_MODE_DESC modeDesc = { 0 };
	modeDesc.Width = g_pGame->getWindowWidth();
	modeDesc.Height = g_pGame->getWindowHeight();
	modeDesc.RefreshRate.Numerator = 60;
	modeDesc.RefreshRate.Denominator = 1;
	modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; 
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferDesc = modeDesc;
}

void initSwapChain(DXGI_SWAP_CHAIN_DESC& swapChainDesc, HWindow hwnd)
{
	setNull(swapChainDesc);
	if (g_pGame->isWindowed())
	{
		initSwapChainWindowed(swapChainDesc);
	}
	else
	{
		auto modes = dxgiGetAdapterModes();
		int winWidth = g_pGame->getWindowWidth();
		int winHeight = g_pGame->getWindowHeight();

		for (uint32_t i = 0; i < modes.size(); i++)
		{
			AdapterMode_t mode = modes[i];

			if (mode.width == winWidth && mode.height == winHeight)
			{
				swapChainDesc.BufferDesc.Width = mode.width;
				swapChainDesc.BufferDesc.Height = mode.height;
				swapChainDesc.BufferDesc.Format = static_cast<DXGI_FORMAT>(mode.format);
				break;
			}
			else
			{
				CHECK_ERROR(i == modes.size() - 1, strFormat("Graphics adapter does not support fullscreen %dx%d mode. Restart to switch to windowed mode", winWidth, winHeight).c_str());
			}
		}
	}
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
}

void initDepthStencil(D3D11_TEXTURE2D_DESC& depthStencilDesc)
{
	setNull(depthStencilDesc);
	depthStencilDesc.Width = g_pGame->getWindowWidth();
	depthStencilDesc.Height = g_pGame->getWindowHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = NULL;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = NULL;
	depthStencilDesc.MiscFlags = NULL;
}

void initViewport(D3D11_VIEWPORT& viewport)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(g_pGame->getWindowWidth());
	viewport.Height = static_cast<float>(g_pGame->getWindowHeight());
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
}

void initSampleState(D3D11_SAMPLER_DESC& sampDesc)
{
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC; //D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = -FLT_MAX;
	sampDesc.MaxLOD = FLT_MAX;
}

void initBlend(D3D11_BLEND_DESC& blendDesc)
{
	setNull(blendDesc);
	D3D11_RENDER_TARGET_BLEND_DESC rtbDesc;
	setNull(rtbDesc);
	rtbDesc.BlendEnable = true;
	rtbDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtbDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
	rtbDesc.DestBlendAlpha = D3D11_BLEND_ONE;
	rtbDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbDesc;
}

void initWCBuffer(D3D11_BUFFER_DESC & desc, uint32_t size)
{
	setNull(desc);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//desc.StructureByteStride = 0;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
}
