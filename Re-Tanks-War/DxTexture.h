#pragma once

#include "ITexture.h"
#include <d3d11.h>




class CDxTexture : public ITexture
{
public:
	CDxTexture();
	virtual ~CDxTexture();

	void* getResourceObject() const override;
	int getWidth() const override;
	int getHeight() const override;

	void assign(ID3D11Resource* pResource ,ID3D11ShaderResourceView* pSRV);

private:
	ID3D11Resource* m_pResource;
	ID3D11ShaderResourceView* m_pSRV;
};

