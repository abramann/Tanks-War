#include "graphics.h"
#include "texture.h"
#include "fileio.h"
#include "image2.h"
#include "camera.h"
#include "game.h"

#ifdef _BUILD_WITH_D3D9
#include "imgui\imgui_impl_dx9.h"
#else ifdef _BUILD_WITH_D3D11
#include "imgui\imgui_impl_dx11.h"
#endif

uint64_t  g_frameCounter = 0;

Graphics::Graphics() : m_lpDevice3d(NULL), m_deviceState(NULL)
{
	m_pCamera = new Camera;
#ifdef _BUILD_WITH_D3D9
	m_lpDirect3d = NULL;
//	m_sprite(NULL)
	ZeroMemory(&m_PresentParameter, sizeof(D3DPRESENT_PARAMETERS));
#else ifdef _BUILD_WITH_D3D11
	m_lpDeviceContext = NULL;
	m_lpRenderTargetView = NULL;
	m_lpSwapChain = NULL;
#endif
}

Graphics::~Graphics()
{
	release();
}

bool Graphics::initialize(const Game* game)
{
	HWND hwnd = game->getHwnd();
	m_pCamera->initialize(game);

#ifdef _BUILD_WITH_D3D9
	m_lpDirect3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(m_lpDirect3d))
		return false;

	try
	{
		m_PresentParameter.BackBufferWidth = ::g_gameInfo.width;
		m_PresentParameter.BackBufferHeight = ::g_gameInfo.height;
		m_PresentParameter.BackBufferCount = 1;
		m_PresentParameter.hDeviceWindow = hwnd;
		m_PresentParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
		if (::g_gameInfo.windowed == 0)
		{
			if (isAdaptereCompatility())
				m_PresentParameter.Windowed = FALSE;
			else
				return false;
		}
		else
		{
			m_PresentParameter.BackBufferFormat = D3DFMT_UNKNOWN;
			m_PresentParameter.Windowed = TRUE;
		}
	}
	catch (...)
	{
		return false;
	}

	DWORD behavior = getBehaviorCompatility();
	Result r;
	r = m_lpDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, behavior, &m_PresentParameter,
		&m_lpDevice3d);
	if (FAILED(r))
		return false;

	r = D3DXCreateSprite(m_lpDevice3d, &m_sprite);
	if (FAILED(r))
		return false;

	bool result = ImGui_ImplDX9_Init(m_lpDevice3d);
	Matrix proj;
	D3DXMatrixPerspectiveFovLH(&proj, PROJECT_FOV, g_gameInfo.width / g_gameInfo.height, PROJECT_NEAR_PLANE, PROJECT_FAR_PLANE);
	m_lpDevice3d->SetTransform(D3DTS_PROJECTION, &proj);
	m_lpDevice3d->SetRenderState(D3DRS_LIGHTING, false);
	m_lpDevice3d->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_lpDevice3d->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_lpDevice3d->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);

	return true;

#else ifdef _BUILD_WITH_D3D11

	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = g_gameInfo.width;
	bufferDesc.Height = g_gameInfo.height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = g_gameInfo.windowed;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr;
	uint32 flags = 0;
#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		0, 0, D3D11_SDK_VERSION, &swapChainDesc, &m_lpSwapChain,
		&m_lpDevice3d, NULL, &m_lpDeviceContext);
	if (FAILED(hr))
		return false;

	bool r = ImGui_ImplDX11_Init(m_lpDevice3d, m_lpDeviceContext);
	if (!r)
		return false;

	ID3D11Texture2D* lpBackBuffer;
	hr = m_lpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&lpBackBuffer);

	hr = m_lpDevice3d->CreateRenderTargetView(lpBackBuffer, NULL, &m_lpRenderTargetView);
	lpBackBuffer->Release();
	if (FAILED(hr))
		return false;

	m_lpDeviceContext->OMSetRenderTargets(1, &m_lpRenderTargetView, NULL);

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = g_gameInfo.width;
	depthStencilDesc.Height = g_gameInfo.height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	m_lpDevice3d->CreateTexture2D(&depthStencilDesc, NULL, &m_lpDepthBuffer);
	hr = m_lpDevice3d->CreateDepthStencilView(m_lpDepthBuffer, NULL, &m_lpDepthStencilView);
	if (FAILED(hr))
		return false;

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = g_gameInfo.width;
	viewport.Height = g_gameInfo.height;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
	m_lpDeviceContext->RSSetViewports(1, &viewport);
	ID3D10Blob* lpVSByteCode, *lpPSByteCode;

	D3DX11CompileFromFileA("shader.fx", 0, 0, "VS_Start", "vs_4_0",
		D3DXSHADER_DEBUG, 0, 0, &lpVSByteCode, 0,
		&hr);
	if (FAILED(hr))
		return false;

	hr = m_lpDevice3d->CreateVertexShader(lpVSByteCode->GetBufferPointer(),
		lpVSByteCode->GetBufferSize(),
		0, &m_lpVShader);
	if (FAILED(hr))
		return false;

	m_lpDeviceContext->VSSetShader(m_lpVShader, 0, 0);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_lpDevice3d->CreateInputLayout(layout, 2, lpVSByteCode->GetBufferPointer(),
		lpVSByteCode->GetBufferSize(), &m_lpInputLayout);
	m_lpDeviceContext->IASetInputLayout(m_lpInputLayout);
	m_lpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	SAFE_RELEASE(lpVSByteCode);

	D3DX11CompileFromFileA("shader.fx", 0, 0, "PS_Start", "ps_4_0",
		D3DCOMPILE_DEBUG, 0, 0, &lpPSByteCode, 0,
		&hr);
	if (FAILED(hr))
		return false;

	hr = m_lpDevice3d->CreatePixelShader(lpPSByteCode->GetBufferPointer(),
		lpPSByteCode->GetBufferSize(),
		0, &m_lpPShader);
	if (FAILED(hr))
		return false;

	SAFE_RELEASE(lpPSByteCode);
	
	m_lpDeviceContext->PSSetShader(m_lpPShader, 0, 0);

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(D3DXMATRIX);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = m_lpDevice3d->CreateBuffer(&desc, NULL, &m_lpVSConstBuffer);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;//D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	m_lpDevice3d->CreateSamplerState(&sampDesc, &m_lpSampleState);
	m_lpDeviceContext->PSSetSamplers(0, 1, &m_lpSampleState);
	m_lpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.DestBlendAlpha = D3D11_BLEND_ONE;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;
	float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_lpDevice3d->CreateBlendState(&blendDesc, &m_lpBlendState);
	m_lpDeviceContext->OMSetBlendState(m_lpBlendState, blendFactor, 0xffffffff);
//	getSupportedResolutions();
	return true;
#endif
}

LPVertexBuffer Graphics::createVertexBuffer(uint32 vertices, VB_USAGE usage, Vertex* data)
{
	LPVertexBuffer vb = 0;
	Result r;
	size_t size = vertices*sizeof(Vertex);
#ifdef  _BUILD_WITH_D3D9
	r = m_lpDevice3d->CreateVertexBuffer(size, usage, VERTEX_FVF, D3DPOOL_DEFAULT, &vb, 0);
	if (data)
		setVertexBuffer(vb, data, vertices);
#else ifdef _BUILD_WITH_D3D11
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = vertices*sizeof(Vertex);
	desc.Usage = (D3D11_USAGE)usage;
	uint32 cpuAccess = 0;
	if (usage == VB_USAGE_WRITE)
		cpuAccess = D3D11_CPU_ACCESS_WRITE;

	desc.CPUAccessFlags = cpuAccess;

	if (data == 0)
		m_lpDevice3d->CreateBuffer(&desc, 0, &vb);
	else
	{
		D3D11_SUBRESOURCE_DATA bufData;
		bufData.pSysMem = data;
		m_lpDevice3d->CreateBuffer(&desc, &bufData, &vb);
	}
#endif
	return vb;
}

void Graphics::spriteBegin()
{
//	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void Graphics::spriteEnd()
{
//	m_sprite->End();
}

void Graphics::spriteDraw(SpriteData sd)
{
	V2 translate = V2(sd.x, sd.y);
	V2 center = sd.center;
	float angle = sd.angle;
	Matrix matrix;
	D3DXMatrixTransformation2D(&matrix, NULL, NULL, &sd.scalling, &center, angle, &translate);
//	m_sprite->SetTransform(&matrix);
//	m_sprite->Draw(sd.lpTexture, &sd.rect, NULL, NULL, sd.filterColor);
}

void Graphics::release()
{
#ifdef _BUILD_WITH_D3D9
	SAFE_RELEASE(m_lpDirect3d);
	if(m_lpDevice3d)
		ImGui_ImplDX9_Shutdown();
#else ifdef _BUILD_WITH_D3D11
	SAFE_RELEASE(m_lpDeviceContext);
	SAFE_RELEASE(m_lpRenderTargetView);
	SAFE_RELEASE(m_lpSwapChain);
	SAFE_RELEASE(m_lpDepthStencilView);
	SAFE_RELEASE(m_lpDepthBuffer);
	SAFE_RELEASE(m_lpVSConstBuffer);
	SAFE_RELEASE(m_lpVShader);
	SAFE_RELEASE(m_lpPShader);
	SAFE_RELEASE(m_lpInputLayout);
	SAFE_RELEASE(m_lpSampleState);
	SAFE_RELEASE(m_lpBlendState);
	if (m_lpDevice3d)
		ImGui_ImplDX11_Shutdown();
#endif

	SAFE_RELEASE(m_lpDevice3d);
}

Result Graphics::reset()
{
//	m_sprite->OnLostDevice();
	Result result = 0;// m_lpDevice3d->Reset(&m_PresentParameter);
//	m_sprite->OnResetDevice();
	return result;
}

void handleCamera(LPDIRECT3DDEVICE9 Device)
{
	if (GetAsyncKeyState('P'))
	{
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}
	else if (GetAsyncKeyState('O'))
	{
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		Device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);
	}
	if (GetAsyncKeyState('L'))
	{
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	}
	Matrix proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)1.333F,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	Matrix mat;
	ZeroMemory(mat, sizeof(D3DXMATRIX));
	static float px = 0, py = 0, pz = -300;
	if (GetAsyncKeyState(VK_UP))
		py++;
	else if (GetAsyncKeyState(VK_DOWN))
		py--;

	if (GetAsyncKeyState(VK_RIGHT))
		px++;
	else if (GetAsyncKeyState(VK_LEFT))
		px--;

	if (GetAsyncKeyState(VK_F1))
		pz++;
	else if (GetAsyncKeyState(VK_F2))
		pz--;

	if (GetAsyncKeyState(VK_TAB))
	{
		px = 0; py = 0; pz = 0;
	}
	static float tx = 0, ty = 0, tz = 0;
	if (GetAsyncKeyState('8'))
	{
		tx = 0; ty = 0; tz = 0;
	}
	if (GetAsyncKeyState('1'))
		tx++;
	else if (GetAsyncKeyState('2'))
		tx--;
	if (GetAsyncKeyState('3'))
		ty++;
	else if (GetAsyncKeyState('4'))
		ty--;
	if (GetAsyncKeyState('5'))
		tz++;
	else if (GetAsyncKeyState('6'))
		tz--;
	if (GetAsyncKeyState('W'))
	{
		py++; py++; ty++; ty++;
	}
	else if (GetAsyncKeyState('S'))
	{
		py = py - 2; ty = ty - 2;
	}
	if (GetAsyncKeyState('D'))
	{
		px=px+2; tx=tx+2;
	}
	else if (GetAsyncKeyState('A'))
	{
		px=px-2; tx=tx-2;
	}

	D3DXVECTOR3 position(px, py, pz);
	D3DXVECTOR3 target(tx, ty, tz);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mat, &position, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &mat);
}

D3DXMATRIX handleCameraDX11()
{
	D3DXMATRIX proj, cam;
	D3DXMatrixPerspectiveFovLH(
		&proj, PI * 0.5f, // 90 - degree
		(float)1.333F,
		1.0f,
		1000.0f);

	static float px = 0, py = 0, pz = -400;
	if (GetAsyncKeyState(VK_UP))
		py++;
	else if (GetAsyncKeyState(VK_DOWN))
		py--;

	if (GetAsyncKeyState(VK_RIGHT))
		px++;
	else if (GetAsyncKeyState(VK_LEFT))
		px--;

	if (GetAsyncKeyState(VK_F1))
	{
		pz+=5;
		if (pz == 0)
			pz++;
	}
	else if (GetAsyncKeyState(VK_F2))
	{
		pz-=5;
		if (pz == 0)
			pz--;
	}

	if (GetAsyncKeyState(VK_TAB))
	{
		px = 0; py = 0; pz = -1.0f;
	}
	static float tx = 0, ty = 0, tz = 0;
	if (GetAsyncKeyState('8'))
	{
		tx = 0; ty = 0; tz = 0;
	}
	if (GetAsyncKeyState('1'))
		tx++;
	else if (GetAsyncKeyState('2'))
		tx--;
	if (GetAsyncKeyState('3'))
		ty++;
	else if (GetAsyncKeyState('4'))
		ty--;
	if (GetAsyncKeyState('5'))
		tz++;
	else if (GetAsyncKeyState('6'))
		tz--;
	if (GetAsyncKeyState('W'))
	{
		py+=4; ty+=4;
	}
	else if (GetAsyncKeyState('S'))
	{
		py-=4; ty-=4;
	}
	if (GetAsyncKeyState('D'))
	{
		px+=4; tx+=4;
	}
	else if (GetAsyncKeyState('A'))
	{
		px-=4; tx-=4;
	}
	V3 position = V3(px, py, pz);
	V3 target = V3(tx, ty, tz);
	V3 up = V3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cam, &position, &target, &up);
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	world = world *cam*proj;
	return world;
}

Result Graphics::begin()
{
	Result r = S_OK; 

#ifdef _BUILD_WITH_D3D9
//	handleCamera(m_lpDevice3d);
	ImGui_ImplDX9_NewFrame();
	m_lpDevice3d->Clear(NULL, NULL, D3DCLEAR_TARGET, COLOR_WHITE, 1.0f, NULL);
	m_lpDevice3d->SetFVF(VERTEX_FVF);
	r = m_lpDevice3d->BeginScene();
#else ifdef _BUILD_WITH_D3D11
	ImGui_ImplDX11_NewFrame();
	float bgColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	m_lpDeviceContext->ClearRenderTargetView(m_lpRenderTargetView, bgColor);
//	m_wvp = handleCameraDX11();
#endif
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	return r;
}

void Graphics::drawImage(const Image2* image)
{
	int16 vertices = image->getVertices();
	LPVertexBuffer vb = image->getVertexBuffer();
	LPTextureD3D texture = image->getTexture();
	V3 position = image->getPosition(),
		&scalling = image->getScalling(),
		rotate = image->getRotate(),
		rotateCenter = image->getRotateCenter();
	setDrawProperties(position, scalling, rotate, rotateCenter);
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetStreamSource(0, vb, 0, sizeof(Vertex));
	setTexture(texture);
	m_lpDevice3d->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertices / 3);
#else ifdef _BUILD_WITH_D3D11
	streamVertexBuffer(vb);
	setTexture(texture);
	m_lpDeviceContext->Draw(vertices, 0);
#endif
}

Result Graphics::end()
{
	Result r = S_OK;
	ImGui::EndFrame();
	ImGui::Render();
#ifdef _BUILD_WITH_D3D9
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	r = m_lpDevice3d->EndScene();
#else _BUILD_WITH_D3D11
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	return r;
}

Result Graphics::showBackbuffer()
{
	// m_deviceState = m_lpDevice3d->TestCooperativeLevel();
	Result r;
#ifdef _BUILD_WITH_D3D9
	r = m_lpDevice3d->Present(NULL, NULL, NULL, NULL);
#else #ifdef _BUILD_WITH_D3D11
	r = m_lpSwapChain->Present(0, 0);
#endif

	if(SUCCEEDED(r))
		::g_frameCounter++;
	return r;
}

Result Graphics::drawPrimitive(UINT startVertex, UINT count)
{
	Result r = S_OK;
#ifdef _BUILD_WITH_D3D9
	r = m_lpDevice3d->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, count);
#else ifdef _BUILD_WITH_D3D11
	m_lpDeviceContext->Draw(count*3, startVertex);
#endif
	return r;
}

void Graphics::setDrawProperties(V3 position, V3 scall, V3 rotate, V3 rotateCenter)
{
	Matrix rot = V3ToMatrix(rotate, MATRIX_TYPE_ROTATE),
		rotCent = V3ToMatrix(rotateCenter, MATRIX_TYPE_TRANSLATE),
		rotCentN = V3ToMatrix(-1 * rotateCenter, MATRIX_TYPE_TRANSLATE),
		scal = V3ToMatrix(scall, MATRIX_TYPE_SCALL),
		pos = V3ToMatrix(position, MATRIX_TYPE_TRANSLATE);

	Matrix prop = scal*rot*pos;
//	Matrix prop = scal*rotCentN*rot*rotCent*pos;

#ifdef _BUILD_WITH_D3D9
	setWorldMatrix(prop);
#else ifdef _BUILD_WITH_D3D11
	prop *= m_wvp;
	D3DXMatrixTranspose(&prop, &prop);
	setWorldMatrix(prop);
#endif
}

bool Graphics::loadTexture(const char* file, UINT& width, UINT& height, Color transparency, LPTextureD3D& texture)
{
	Result r;
#ifdef _BUILD_WITH_D3D9
	D3DXIMAGE_INFO info;
	r = D3DXGetImageInfoFromFileA(file, &info);
	if (FAILED(r))
		return false;

	width = info.Width;
	height = info.Height;
	ZeroMemory(&texture, sizeof(texture));
	r = D3DXCreateTextureFromFileExA(m_lpDevice3d, file, width,
		height, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, transparency,
		&info, NULL, &texture);
	if (FAILED(r))
		return false;
	return true;
#else ifdef _BUILD_WITH_D3D11
	D3DX11_IMAGE_INFO info;

	D3DX11GetImageInfoFromFileA(file, 0, &info, &r);
	if (FAILED(r))
		return false;

	width = info.Width;
	height = info.Height;

	D3DX11CreateShaderResourceViewFromFileA(m_lpDevice3d, file, 0, 0, &texture, &r);
	if (FAILED(r))
		return false;

	return true;
#endif
}

void Graphics::setVertexBuffer(LPVertexBuffer vb, Vertex * vertex, int32_t vertices)
{
#ifdef _BUILD_WITH_D3D9
	Vertex* ptr;
	vb->Lock(0, 0,(void**)&ptr, 0);
	memcpy(ptr, vertex, vertices*sizeof(Vertex));
	vb->Unlock();
#else ifdef _BUILD_WITH_D3D11
	D3D11_MAPPED_SUBRESOURCE data;
	m_lpDeviceContext->Map(vb,0 , D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy(data.pData, vertex, sizeof(Vertex)*vertices);
	m_lpDeviceContext->Unmap(vb, 0);
#endif
}

void Graphics::setVertexBufferUV(LPVertexBuffer vb, Vertex * vertex, int8_t len)
{
#ifdef _BUILD_WITH_D3D9
	Vertex* v;
	vb->Lock(0, 0, (void**)&v, 0);
	for (int i = 0; i < len; i++)
	{
		v[i].u = vertex[i].u;
		v[i].v = vertex[i].v;
	}
	vb->Unlock();
#else ifdef _BUILD_WITH_D3D11
	D3D11_MAPPED_SUBRESOURCE data;
	m_lpDeviceContext->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	Vertex* v =(Vertex*) data.pData;
	v+= sizeof(float) * 12;
	memcpy((void*)v, vertex, sizeof(float) * 2);
	m_lpDeviceContext->Unmap(vb, 0);
#endif
}

void Graphics::setWorldMatrix(Matrix worldMatrix)
{
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetTransform(D3DTS_WORLD, &worldMatrix);
#else ifdef _BUILD_WITH_D3D11
	setVSConstBuffer(worldMatrix);
#endif
}

bool Graphics::isAdaptereCompatility()
{
#ifdef _BUILD_WITH_D3D9
	D3DDISPLAYMODE dMode;
	m_lpDirect3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dMode);
	m_PresentParameter.BackBufferFormat = D3DFMT_X8R8G8B8;
	int modes = m_lpDirect3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, m_PresentParameter.BackBufferFormat);
	if (modes == 0)
	{
		throw GameError(gameErrorNS::FATAL_ERROR, "Your adapter doesn't support game format! Try to run as windowed.");
		return false;
	}

	for (auto i = 0; i <= modes; i++)
	{
		m_lpDirect3d->EnumAdapterModes(D3DADAPTER_DEFAULT, m_PresentParameter.BackBufferFormat, i, &dMode);
		if (dMode.Height == m_PresentParameter.BackBufferHeight && dMode.Width == m_PresentParameter.BackBufferWidth & m_PresentParameter.BackBufferFormat == dMode.Format)
			return true;
	}

	return false;
#else ifdef _BUILD_WITH_D3D11
	return true;
#endif
}

Result Graphics::getDeviceState()
{
	Result r=S_OK;
#ifdef _BUILD_WITH_D3D9
	r = m_lpDevice3d->TestCooperativeLevel();
#else ifdef BUILD_WITH_D3D11
#endif
	return r;
}

std::vector<Resolution> Graphics::getSupportedResolutions()
{
	std::vector<Resolution> resolution;
#ifdef _BUILD_WITH_D3D9
	int modes = m_lpDirect3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, m_PresentParameter.BackBufferFormat);
	D3DDISPLAYMODE dMode;
	for (auto i = 0; i <= modes; i++)
	{
		m_lpDirect3d->EnumAdapterModes(D3DADAPTER_DEFAULT, m_PresentParameter.BackBufferFormat, i, &dMode);
		if (dMode.Width < MIN_RESOLUTION_WIDTH || dMode.Height < MIN_RESOLUTION_HEIGHT || dMode.Height >= dMode.Width)
			continue;

		Resolution resol;
		resol.width = dMode.Width, resol.height = dMode.Height;
		resolution.push_back(resol);
	}

#else ifdef _BUILD_WITH_D3D11
	IDXGIFactory* factory;
/*
	CreateDXGIFactory(IID_PPV_ARGS(&factory)); 
	IDXGIOutput* adapterOutput;
	unsigned numerator = 0;
	unsigned denominator = 1;
	IDXGIAdapter* adapter;
	factory->EnumAdapters(0, &adapter);
	adapter->EnumOutputs(0, &adapterOutput);

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display
	// format for the adapter output (monitor).
	unsigned int numModes;
	DXGI_MODE_DESC* displayModeList;
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes, NULL);

	// Create a list to hold all the possible display modes for this monitor/video
	// card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList);

	// Now go through all the display modes and find the one that matches the screen
	// width and height. When a match is found store the numerator and denominator
	// of the refresh rate for that monitor.
	for (size_t i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width = 800 && displayModeList[i].Height == 600)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
			break;
		}
	}
	SAFE_DELETE_ARRAY(displayModeList);*/
#endif
	return resolution;

}

Resolution Graphics::getResolution()
{
	Resolution resolution;
	GameInfo info = FileIO::readGameInfo();
	resolution.width = info.width;
	resolution.height = info.height;
	return resolution;
}

void Graphics::setResolution(const Resolution& resolution)
{
	GameInfo info;
	info.windowed = BYTE_INVALID_DATA;
	info.width = resolution.width;
	info.height = resolution.height;
	FileIO::createGameInfo(info);
}

void Graphics::setTexture(LPTextureD3D texture)
{
#ifdef  _BUILD_WITH_D3D9
	m_lpDevice3d->SetTexture(0, texture);
#else ifdef _BUILD_WITH_D3D11
	m_lpDeviceContext->PSSetShaderResources(0, 1, &texture);
#endif
}

bool Graphics::isFullScreen()
{
	bool windowed = FileIO::readGameInfo().windowed;
	return windowed;
}

void Graphics::setFullScreen(const bool& windowed)
{
	GameInfo info;
	info.width = INVALID_DATA;
	info.height = INVALID_DATA;
	info.windowed = windowed;
	FileIO::createGameInfo(info);
}

void Graphics::resize(uint16_t width, uint16_t height)
{
	/*
	SetWindowPos(m_hwnd, 0, 0, 0, width, height, 6);
	m_PresentParameter.BackBufferWidth = width;
	m_PresentParameter.BackBufferHeight = height;
	m_lpDevice3d->Reset(&m_PresentParameter);
	ImGui_ImplDX9_InvalidateDeviceObjects();
	m_lpDevice3d->Reset(&m_PresentParameter);
	ImGui_ImplDX9_CreateDeviceObjects();
	*/
}

void Graphics::streamVertexBuffer(LPVertexBuffer vb)
{
	UINT stride = sizeof(Vertex), offset = 0;
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetStreamSource(0, vb, 0, stride);
#else #ifdef _BUILD_WITH_D3D11
	m_lpDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
#endif
}

void Graphics::setViewMatrix(Matrix viewMatrix)
{
#ifdef _BUILD_WITH_D3D9
	m_lpDevice3d->SetTransform(D3DTS_VIEW, &viewMatrix);
#else ifdef _BUILD_WITH_D3D11
	m_wvp = viewMatrix;
#endif
}

Matrix Graphics::V3ToMatrix(const V3 v3, MatrixType type)
{
	Matrix mat;
	const float &x = v3.x, &y = v3.y, &z = v3.z;
	switch (type)
	{
	case MATRIX_TYPE_TRANSLATE:
		D3DXMatrixTranslation(&mat, x, y, z);
		break;
	case MATRIX_TYPE_SCALL:
		D3DXMatrixScaling(&mat, x, y, z);
		break;
	case MATRIX_TYPE_ROTATE:
		D3DXMatrixRotationX(&mat, x);
		D3DXMatrixRotationY(&mat, y);
		D3DXMatrixRotationZ(&mat, z);
		break;
	default:
		break;
	}

	return mat;
}

DWORD Graphics::getBehaviorCompatility()
{
	DWORD behavior;
#ifdef _BUILD_WITH_D3D9
	D3DCAPS9 caps;
	m_lpDirect3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if ((caps.DevCaps && D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
	{
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // use software only processing
		return behavior;
	}
	else
	{
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // use hardware only processing
		return behavior;
	}
#endif
	return behavior;
}
#ifdef _BUILD_WITH_D3D11
void Graphics::setVSConstBuffer(const void * data)
{
	m_lpDeviceContext->UpdateSubresource(m_lpVSConstBuffer, 0, 0, data, 0,
		0);
	m_lpDeviceContext->VSSetConstantBuffers(0, 1, &m_lpVSConstBuffer);
}
#endif