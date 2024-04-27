#include "DxTexture.h"


CDxTexture::CDxTexture()
{
}


CDxTexture::~CDxTexture()
{
	m_pResource->Release();
	m_pSRV->Release();
}

void * CDxTexture::getResourceObject() const
{
	return static_cast<void*>(m_pSRV);
}

int CDxTexture::getWidth() const
{
	ID3D11Texture2D* pTex2D = static_cast<ID3D11Texture2D*>(m_pResource);
	D3D11_TEXTURE2D_DESC desc;
	pTex2D->GetDesc(&desc);
	return desc.Width;
}

int CDxTexture::getHeight() const
{
	ID3D11Texture2D* pTex2D = static_cast<ID3D11Texture2D*>(m_pResource);
	D3D11_TEXTURE2D_DESC desc;
	pTex2D->GetDesc(&desc);
	return desc.Height;
}

void CDxTexture::assign(ID3D11Resource* pResource, ID3D11ShaderResourceView* pSRV)
{
	m_pResource = pResource;
	m_pSRV = pSRV;
}
