#include "graphics.h"
#include "texture.h"
#include "fileio.h"
#include "imgui\imgui_impl_dx9.h"
#include "image2.h"
uint64_t  g_frameCounter = 0;

Graphics::Graphics() : m_lpDirect3d(NULL), m_lpDevice3d(NULL), m_sprite(NULL),
m_deviceState(NULL)
{
	ZeroMemory(&m_PresentParameter, sizeof(D3DPRESENT_PARAMETERS));
}

Graphics::~Graphics()
{
	release();
	ImGui_ImplDX9_Shutdown();
}

bool Graphics::initialize(HWND hwnd)
{
	m_hwnd = hwnd;
	m_lpDirect3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(m_lpDirect3d))
		return false;

	try
	{
		m_PresentParameter.BackBufferWidth = ::g_gameInfo.width;
		m_PresentParameter.BackBufferHeight = ::g_gameInfo.height;
		m_PresentParameter.BackBufferCount = 1;
		m_PresentParameter.hDeviceWindow = m_hwnd;
		m_PresentParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
		if (::g_gameInfo.fullScreen)
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
	r = m_lpDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd, behavior, &m_PresentParameter,
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
}

LPVertexBuffer Graphics::createVertexBuffer(uint32 vertices, int usage)
{
	LPVertexBuffer vb = 0;
	Result r;
	size_t size = vertices*sizeof(Vertex);
	r = m_lpDevice3d->CreateVertexBuffer(size, usage, VERTEX_FVF, D3DPOOL_DEFAULT, &vb, 0);
	return vb;
}

void Graphics::spriteBegin()
{
	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void Graphics::spriteEnd()
{
	m_sprite->End();
}

void Graphics::spriteDraw(SpriteData sd)
{
	V2 translate = V2(sd.x, sd.y);
	V2 center = sd.center;
	float angle = sd.angle;
	Matrix matrix;
	D3DXMatrixTransformation2D(&matrix, NULL, NULL, &sd.scalling, &center, angle, &translate);
	m_sprite->SetTransform(&matrix);
	m_sprite->Draw(sd.lpTexture, &sd.rect, NULL, NULL, sd.filterColor);
}

void Graphics::release()
{
	SAFE_RELEASE(m_sprite);
	SAFE_RELEASE(m_lpDevice3d);
	SAFE_RELEASE(m_lpDirect3d);
}

Result Graphics::reset()
{
	m_sprite->OnLostDevice();
	Result result = m_lpDevice3d->Reset(&m_PresentParameter);
	m_sprite->OnResetDevice();
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
		py++; ty++;
	}
	else if (GetAsyncKeyState('S'))
	{
		py--; ty--;
	}
	if (GetAsyncKeyState('D'))
	{
		px++; tx++;
	}
	else if (GetAsyncKeyState('A'))
	{
		px--; tx--;
	}
	D3DXVECTOR3 position(px, py, pz);
	D3DXVECTOR3 target(tx, ty, tz);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mat, &position, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &mat);
}
Result Graphics::begin()
{	
	for (int i = 0; i < 10;i++)
	handleCamera(m_lpDevice3d);
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	m_lpDevice3d->Clear(NULL, NULL, D3DCLEAR_TARGET, COLOR_WHITE, 1.0f, NULL);
	Result r;
	r = m_lpDevice3d->BeginScene();
	ImGui::NewFrame();
	m_lpDevice3d->SetFVF(VERTEX_FVF);
	return r;
}

void Graphics::drawImage(const Image2* image)
{
	int16 vertices = image->getVertices();
	LPVertexBuffer vb = image->getVertexBuffer();
	LPDIRECT3DTEXTURE9 texture = image->getTexture();
	V3 position = image->getPosition(),
		&scalling = image->getScalling(),
		rotate = image->getRotate();
	setDrawProperties(position, scalling, rotate);

	m_lpDevice3d->SetStreamSource(0, vb, 0, sizeof(Vertex));
	m_lpDevice3d->SetTexture(0, texture);
	m_lpDevice3d->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertices / 3);
}

Result Graphics::end()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	Result r;
	r = m_lpDevice3d->EndScene();
	return r;
}

Result Graphics::showBackbuffer()
{
	m_deviceState = m_lpDevice3d->TestCooperativeLevel();
	Result r;
	r = m_lpDevice3d->Present(NULL, NULL, NULL, NULL);
	if(SUCCEEDED(r))
		::g_frameCounter++;
	return r;
}

Result Graphics::drawPrimitive(UINT startVertex, UINT count)
{
	Result r;
	r = m_lpDevice3d->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, count);
	return r;
}

void Graphics::setDrawProperties(V3 position, V3 scall, V3 rotate)
{
	Matrix rot = V3ToMatrix(rotate,MATRIX_TYPE_ROTATE),
		scal = V3ToMatrix(scall,MATRIX_TYPE_SCALL),
		pos = V3ToMatrix(position,MATRIX_TYPE_TRANSLATE);
	Matrix prop = scal*rot*pos;
	m_lpDevice3d->SetTransform(D3DTS_WORLD, &prop);
}

bool Graphics::loadTexture(const char* file, UINT& width, UINT& height, Color transparency, LPDIRECT3DTEXTURE9& texture)
{
	D3DXIMAGE_INFO info;
	Result r;
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
}

void Graphics::setVertexBuffer(LPVertexBuffer vb, Vertex * vertex, int32_t numberOfVertices)
{
	Vertex* ptr;
	vb->Lock(0, 0,(void**)&ptr, 0);
	memcpy(ptr, vertex, numberOfVertices*sizeof(Vertex));
	vb->Unlock();
}

void Graphics::setVertexBufferUV(LPVertexBuffer vb, Vertex * vertex, int8_t len)
{
	Vertex* v;
	vb->Lock(0, 0, (void**)&v, 0);
	for (int i = 0; i < len; i++)
	{
		v[i].u = vertex[i].u;
		v[i].v = vertex[i].v;
	}
	vb->Unlock();
}

bool Graphics::isAdaptereCompatility()
{
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
}

Result Graphics::getDeviceState()
{
	Result r;
	r = m_lpDevice3d->TestCooperativeLevel();
	return r;
}

std::vector<Resolution> Graphics::getSupportedResolutions()
{
	std::vector<Resolution> resolution;
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
	info.fullScreen = BYTE_INVALID_DATA;
	info.width = resolution.width;
	info.height = resolution.height;
	FileIO::createGameInfo(info);
}

bool Graphics::isFullScreen()
{
	bool fullscreen = FileIO::readGameInfo().fullScreen;
	return fullscreen;
}

void Graphics::setFullScreen(const bool& fullScreen)
{
	GameInfo info;
	info.width = INVALID_DATA;
	info.height = INVALID_DATA;
	info.fullScreen = fullScreen;
	FileIO::createGameInfo(info);
}

void Graphics::resize(uint16_t width, uint16_t height)
{
	SetWindowPos(m_hwnd, 0, 0, 0, width, height, 6);
	m_PresentParameter.BackBufferWidth = width;
	m_PresentParameter.BackBufferHeight = height;
	m_lpDevice3d->Reset(&m_PresentParameter);
	ImGui_ImplDX9_InvalidateDeviceObjects();
	m_lpDevice3d->Reset(&m_PresentParameter);
	ImGui_ImplDX9_CreateDeviceObjects();
}

void Graphics::streamVertexBuffer(LPVertexBuffer vb)
{
	m_lpDevice3d->SetStreamSource(0, vb, 0, sizeof(Vertex));
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
}
