#include "graphics.h"
#include "texture.h"

uint64_t  g_frameCounter = 0;

Graphics::Graphics() : m_lpDirect3d(NULL), m_lpDevice3d(NULL), m_sprite(NULL),
	m_deviceState(NULL), m_lpVB(NULL), m_vertices(0)
{
	ZeroMemory(&m_PresentParameter, sizeof(D3DPRESENT_PARAMETERS));
}

Graphics::~Graphics()
{
	release();
}

bool Graphics::initialize(HWND hWnd)
{
	m_lpDirect3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(m_lpDirect3d))
		return false;

	try
	{
		m_PresentParameter.BackBufferWidth = pGameInfo->width;
		m_PresentParameter.BackBufferHeight = pGameInfo->height;
		m_PresentParameter.BackBufferCount = 1;
		m_PresentParameter.hDeviceWindow = hWnd;
		m_PresentParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
		if (pGameInfo->fullScreen)
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
	HRESULT hr;
	hr = m_lpDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, behavior, &m_PresentParameter, &m_lpDevice3d);
	if (FAILED(hr))
		return false;

	hr = D3DXCreateSprite(m_lpDevice3d, &m_sprite);
	if (FAILED(hr))
		return false;
	return true;
}

HRESULT Graphics::createVertexBuffer(UINT length)
{
	m_vertices = length;
	HRESULT hr;
	hr = m_lpDevice3d->CreateVertexBuffer(length*sizeof(Vertex), D3DUSAGE_WRITEONLY, VERTEX_FVF, D3DPOOL_DEFAULT, &m_lpVB, 0);
	return hr;
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
	V2 scalling = V2(sd.scalling, sd.scalling);
	float angle = sd.angle;
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix, NULL, NULL, &scalling, &center, angle, &translate);
	m_sprite->SetTransform(&matrix);
	m_sprite->Draw(sd.lpTexture, &sd.rect, NULL, NULL, sd.filterColor);
}

void Graphics::release()
{
	SAFE_RELEASE(m_lpVB);
	SAFE_RELEASE(m_sprite);
	SAFE_RELEASE(m_lpDevice3d);
	SAFE_RELEASE(m_lpDirect3d);
}

HRESULT Graphics::reset()
{
	m_sprite->OnLostDevice();
	HRESULT result = m_lpDevice3d->Reset(&m_PresentParameter);
	m_sprite->OnResetDevice();
	return result;
}

HRESULT Graphics::begin()
{
	m_lpDevice3d->Clear(NULL, NULL, D3DCLEAR_TARGET, COLOR_XRGB( 200,100, 80), 1.0f, NULL);
	HRESULT hr;
	hr = m_lpDevice3d->BeginScene();
	m_lpDevice3d->SetFVF(VERTEX_FVF);
	m_lpDevice3d->SetStreamSource(0, m_lpVB, 0, sizeof(Vertex));
	return hr;
}

HRESULT Graphics::end()
{
	HRESULT hr;
	hr = m_lpDevice3d->EndScene();
	return hr;
}

HRESULT Graphics::showBackbuffer()
{
	m_deviceState = m_lpDevice3d->TestCooperativeLevel();
	HRESULT hr;
	hr = m_lpDevice3d->Present(NULL, NULL, NULL, NULL);
	if(SUCCEEDED(hr))
		::g_frameCounter++;
	return hr;
}

HRESULT Graphics::drawPrimitive(UINT startVertex, UINT count)
{
	HRESULT hr;
	hr = m_lpDevice3d->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, count);
	return hr;
}

bool Graphics::loadTexture(const char* file, UINT& width, UINT& height, Color transparency, LPDIRECT3DTEXTURE9& texture)
{
	D3DXIMAGE_INFO info;
	HRESULT hr;
	hr = D3DXGetImageInfoFromFileA(file, &info);
	if (FAILED(hr))
		return false;

	width = info.Width;
	height = info.Height;
	ZeroMemory(&texture, sizeof(texture));
	hr = D3DXCreateTextureFromFileExA(m_lpDevice3d, file, width,
		height, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, transparency,
		&info, NULL, &texture);
	if (FAILED(hr))
		return false;
	return true;
}

void Graphics::setVertexBuffer(void * pData, size_t size)
{
	Vertex* ptr;
	m_lpVB->Lock(0, 0,(void**)&ptr, 0);
	memcpy(ptr, pData, size);
	m_lpVB->Unlock();
}

HRESULT Graphics::createLine(ID3DXLine** line)
{
	HRESULT result = D3DXCreateLine(m_lpDevice3d, line);
	return result;
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

HRESULT Graphics::getDeviceState()
{
	HRESULT hr;
	hr = m_lpDevice3d->TestCooperativeLevel();
	return hr;
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
