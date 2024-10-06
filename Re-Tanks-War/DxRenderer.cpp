#include "DxRenderer.h"
#include "GameError.h"
#include "IGame.h"
#include "GameData.h"
#include "String.h"
#include "Model.h"
#include "Sprite.h"
#include "HLSLVertexShader.h"
#include "HLSLPixelShader.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui_impl_dx11.h"
#include "DxBuffer.h"
#include "Map.h"
#include "DxTexture.h"
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

void initWCBuffer(D3D11_BUFFER_DESC& desc, uint32_t size, uint stride = 0);

CDxRenderer::CDxRenderer() : m_initialized(false)
{
	g_pDxRenderer = this;
}


CDxRenderer::~CDxRenderer()
{
	if (m_pSwapChain)
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
	CHECK_ERROR(SUCCEEDED(m_pDevice->CreateDepthStencilView(m_pDepthBuffer.Get(), nullptr, &m_pDepthStencilView)), "CreateDepthStencilView failed");
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	
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
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
	initWCBuffer(bufDesc, sizeof(VSCBPerDraw));
	m_pVSInSSOBuffer = createBuffer(bufDesc, nullptr);

	// Initialize world view matrix buffer
	initWCBuffer(bufDesc, sizeof(VSCBPerFrame));
	m_pVBPerFrame = createBuffer(bufDesc, nullptr);

	// Initializing color buffer
	initWCBuffer(bufDesc, sizeof(PSCBPerDraw));
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
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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

	desc.ByteWidth = numVertices * sizeof(Vertex);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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
	CDxBuffer * pNewVertexBuffer = new CDxBuffer;
	pNewVertexBuffer->assign(pNewDXOBuffer);
	return pNewVertexBuffer;
}

IBuffer * CDxRenderer::createIndexBuffer(uint32_t length, uint * pInitData, int access)
{
	D3D11_BUFFER_DESC desc;
	setNull(desc);

	desc.ByteWidth = length;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.StructureByteStride = sizeof(int);

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

void CDxRenderer::setVSDrawProperties(Vertex position, Vertex scaling, Vertex rotate, Vertex rotateCenter) const
{
	VSCBPerDraw data;
	data.position[0] = position.x;
	data.position[1] = position.y;
	data.position[2] = position.z;
	data.position[3] = 0;

	data.scaling[0] = scaling.x;
	data.scaling[1] = scaling.y;
	data.scaling[2] = scaling.z;
	data.scaling[3] = 0;

	data.rotate[0] = rotate.x;
	data.rotate[1] = rotate.y;
	data.rotate[2] = rotate.z;
	data.rotate[3] = 0;

	data.rotateCenter[0] = rotateCenter.x;
	data.rotateCenter[1] = rotateCenter.y;
	data.rotateCenter[2] = rotateCenter.z;
	data.rotateCenter[3] = 0;
	m_pDeviceContext->VSSetConstantBuffers(VSCBSLOT_PERDRAW, 1, m_pVSInSSOBuffer.GetAddressOf());
	m_pDeviceContext->UpdateSubresource(m_pVSInSSOBuffer.Get(), 0, 0, &data, 0,
		0);
}

void CDxRenderer::setPSDrawProperties(Color color) const
{
	PSCBPerDraw cb;
	cb.color = color.getAsInteger();
	
	m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pPSColorBuffer.GetAddressOf());
	m_pDeviceContext->UpdateSubresource(m_pPSColorBuffer.Get(), 0, 0, &cb, 0,
		0);
}

void CDxRenderer::renderMap() const
{
	ID3D11Buffer* pVB = (ID3D11Buffer*)g_pMap->m_pVB->getBufferObject();
	ID3D11Buffer* pIB = (ID3D11Buffer*)g_pMap->m_pIB->getBufferObject();

	// Default properties
	setVSDrawProperties();
	setPSDrawProperties();

	m_pDeviceContext->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);
	
	uint stride = sizeof(Vertex), offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &pVB, &stride, &offset); 
	
	for (int i = 0; i < g_pMap->m_numTileds; i++)
	{
		ID3D11ShaderResourceView* pTexture = (ID3D11ShaderResourceView*)g_pMap->m_pTextures[i]->getResourceObject();
		m_pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
		m_pDeviceContext->DrawIndexed(g_pMap->m_lenVertex[i] * 1.5f, (g_pMap->m_startVertex[i] * 1.5f), 0); // index per texture vertex = 6 / 4 =  1.5
	}
}

void CDxRenderer::drawSprite(const CSprite* pSprite) const
{
	IBuffer* pVB = pSprite->getVertexBuffer();
	ID3D11Buffer* pDXOVB = static_cast<ID3D11Buffer*>(pVB->getBufferObject());
	
	IBuffer* pIB = pSprite->getIndexBuffer();
	ID3D11Buffer* pDXOIB = static_cast<ID3D11Buffer*>(pIB->getBufferObject());
	
	ITexture* pTexture = pSprite->getTexture();
	ID3D11ShaderResourceView* pSRV = static_cast<ID3D11ShaderResourceView*>(pTexture->getResourceObject());
	
	Color color = pSprite->getColor();

	uint32_t perVertexSize = sizeof(Vertex);
	uint32_t offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &pDXOVB, &perVertexSize, &offset);
	m_pDeviceContext->IASetIndexBuffer(pDXOIB, DXGI_FORMAT_R32_UINT, 0);

	setVSDrawProperties(Vertex(mapNS::UNDEFINED_POSITION, 0));
	setPSDrawProperties(color);

	m_pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	m_pDeviceContext->PSSetSamplers(0, 1, m_pSampleState.GetAddressOf());
	m_pDeviceContext->DrawIndexed(6, 0, 0);
}

void CDxRenderer::drawModel(const CModel* pModel) const
{
	IBuffer* pVB = pModel->getVertexBuffer();
	ID3D11Buffer* pDXOVB = static_cast<ID3D11Buffer*>(pVB->getBufferObject());

	IBuffer* pIB = pModel->getIndexBuffer();
	ID3D11Buffer* pDXOIB = static_cast<ID3D11Buffer*>(pIB->getBufferObject());

	m_pDeviceContext->IASetIndexBuffer(pDXOIB, DXGI_FORMAT_R32_UINT, 0);

	uint stride = sizeof(Vertex), offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &pDXOVB, &stride, &offset);

	Vertex position = pModel->getPosition();
	position.z = -5;
	std::vector<Face> faces = pModel->getFaces();
	for (int i = 0; i < faces.size(); i++)
	{
		setVSDrawProperties(position, Vertex(1, 1, 1), faces[i].rotate, faces[i].origin);
		setPSDrawProperties(faces[i].color);

		ID3D11ShaderResourceView* pTexture = (ID3D11ShaderResourceView*)faces[i].pTexture->getResourceObject();
		m_pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
		
		m_pDeviceContext->DrawIndexed(6, i * 6 , 4 * i);
	}
}

void CDxRenderer::setWorldViewMatrix(Matrix* mat)
{
	m_pDeviceContext->VSSetConstantBuffers(VSCBSLOT_PERFRAME, 1, m_pVBPerFrame.GetAddressOf());
	m_pDeviceContext->UpdateSubresource(m_pVBPerFrame.Get(), 0, 0, mat, 0,
		0);
}

ITexture * CDxRenderer::loadTextureFromFile(const wchar_t * texFileName)
{
	static std::hash<std::wstring> hasher;
	uint hash = hasher(texFileName);

	static std::map <uint, ITexture*> loaded;

	if (loaded[hash] != nullptr)
	{
		return loaded[hash];
	}

	wchar_t texPath[256] = { 0 };
	wsprintfW(texPath, L"Assets\\Textures\\%s", texFileName);

	// Load the texture
	ID3D11Resource* pResource;
	ID3D11ShaderResourceView* pSRV;

	CHECK_ERROR(SUCCEEDED(CreateWICTextureFromFile(m_pDevice.Get(), texPath, &pResource, &pSRV)), strFormat("Failed to load %s", texPath).c_str());
	CDxTexture* pNewTexture = new CDxTexture;
	pNewTexture->assign(pResource, pSRV);
	loaded[hash] = pNewTexture;
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
	setNull(sampDesc);
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;// D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;// CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;// D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = -FLT_MAX;
	sampDesc.MaxLOD = FLT_MAX;
	sampDesc.MaxAnisotropy = 4;
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

void initWCBuffer(D3D11_BUFFER_DESC & desc, uint32_t size, uint stride)
{
	setNull(desc);
	desc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_DYNAMIC;
	//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.StructureByteStride = stride;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
}
