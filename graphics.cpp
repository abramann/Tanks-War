#include "graphics.h"

UINT64  frameCounter = 0;

Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	sprite = NULL;
	hWnd = NULL; 
	fullscreen = NULL;
	deviceState = NULL;
	ZeroMemory(&pPresentParameter, sizeof(D3DPRESENT_PARAMETERS));
}

Graphics::~Graphics()
{
	release();
}

bool Graphics::initialize(HWND _hWnd, bool _fullscreen)
{
	hWnd = _hWnd;
	fullscreen = _fullscreen;
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(direct3d))
		return false;
	try
	{
		pPresentParameter.BackBufferWidth = GAME_WIDTH;
		pPresentParameter.BackBufferHeight = GAME_HEIGHT;
		pPresentParameter.BackBufferCount = 1;
		pPresentParameter.hDeviceWindow = hWnd;
		pPresentParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
		if (fullscreen && isAdaptereCompatility())
		{
			pPresentParameter.Windowed = FALSE;
		}
		else
		{
			pPresentParameter.BackBufferFormat = D3DFMT_UNKNOWN;
			pPresentParameter.Windowed = TRUE;
		}
	}
	catch (...)
	{
		return false;
	}

	DWORD behavior = getBehaviorCompatility();
	HRESULT hr;
	hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, behavior, &pPresentParameter, &device3d);
	if (FAILED(hr))
		return false;

	hr = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(hr))
		return false;
	return true;
}

HRESULT Graphics::createVertexBuffer(UINT length)
{
	m_vertices = length;
	HRESULT hr;
	hr = device3d->CreateVertexBuffer(length*sizeof(Vertex), D3DUSAGE_WRITEONLY, VERTEX_FVF, D3DPOOL_DEFAULT, &m_vb, 0);
	return hr;
}

void Graphics::spriteBegin()
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void Graphics::spriteEnd()
{
	sprite->End();
}

void Graphics::spriteDraw(SpriteData sd)
{
	V2 translate = V2(sd.x, sd.y);
	V2 center = sd.center;
	V2 scalling = V2(sd.scalling, sd.scalling);
	float angle = sd.angle;
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix, NULL, NULL, &scalling, &center, angle, &translate);
	sprite->SetTransform(&matrix);
	sprite->Draw(sd.texture, &sd.rect, NULL, NULL, sd.filterColor);
}

void Graphics::release()
{
	SAFE_RELEASE(m_vb);
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

HRESULT Graphics::reset()
{
	sprite->OnLostDevice();
	HRESULT result = device3d->Reset(&pPresentParameter);
	sprite->OnResetDevice();
	return result;
}

HRESULT Graphics::begin()
{
	device3d->Clear(NULL, NULL, D3DCLEAR_TARGET, COLOR_XRGB( 200,100, 80), 1.0f, NULL);
	HRESULT hr;
	hr = device3d->BeginScene();
	device3d->SetFVF(VERTEX_FVF);
	device3d->SetStreamSource(0, m_vb, 0, sizeof(Vertex));
	return hr;
}

HRESULT Graphics::end()
{
	HRESULT hr;
	hr = device3d->EndScene();

	return hr;
}

HRESULT Graphics::showBackbuffer()
{
	deviceState = device3d->TestCooperativeLevel();
	HRESULT hr;
	hr = device3d->Present(NULL, NULL, NULL, NULL);
	if(SUCCEEDED(hr))
		frameCounter++;
	return hr;
}

HRESULT Graphics::drawPrimitive(UINT startVertex, UINT count)
{
	HRESULT hr;
	hr = device3d->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, count);
	return hr;
}

bool Graphics::loadTexture(const char* textureFile, int& width, int& height, COLOR transpanceyC, LPDIRECT3DTEXTURE9& texture)
{
	D3DXIMAGE_INFO info;
	HRESULT hr;
	hr = D3DXGetImageInfoFromFileA(textureFile, &info);
	if (FAILED(hr))
		return false;

	width = info.Width;
	height = info.Height;
	ZeroMemory(&texture, sizeof(texture));
	hr = D3DXCreateTextureFromFileExA(
		device3d,
		textureFile,
		width,
		height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transpanceyC,
		&info,
		NULL,
		&texture
		);
	if (FAILED(hr))
		return false;
	return true;
}

void Graphics::setVertexBuffer(void * pData, UINT size)
{
	Vertex* ptr;
	m_vb->Lock(0, 0,(void**)&ptr, 0);
	memcpy(ptr, pData, size);
	m_vb->Unlock();
}

HRESULT Graphics::createLine(ID3DXLine** line)
{
	HRESULT result = D3DXCreateLine(device3d, line);
	return result;
}

bool Graphics::isAdaptereCompatility()
{
	D3DDISPLAYMODE dMode;
	direct3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dMode);
	pPresentParameter.BackBufferFormat = D3DFMT_X8R8G8B8;
	int modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, pPresentParameter.BackBufferFormat);
	if (modes == 0)
	{
		throw GameError(gameErrorNS::FATAL_ERROR, "Your adapter doesn't support game format ! try to run as windowed .");
		return false;
	}
	for (int i = 0; i <= modes; i++)
	{
		direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, pPresentParameter.BackBufferFormat, i, &dMode);
		if (dMode.Height == pPresentParameter.BackBufferHeight && dMode.Width == pPresentParameter.BackBufferWidth && pPresentParameter.BackBufferFormat == dMode.Format)
			return true;
	}
	return false;
}

HRESULT Graphics::getDeviceState()
{
	HRESULT hr;
	hr = device3d->TestCooperativeLevel();
	return hr;
}

DWORD Graphics::getBehaviorCompatility()
{
	DWORD behavior;
	D3DCAPS9 caps;
	direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
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
