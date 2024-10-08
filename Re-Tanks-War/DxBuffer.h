#pragma once

#include "Buffer.h"
#include <d3d11.h>


class DxRenderer;

class CDxBuffer : public IBuffer
{
public:	
	CDxBuffer();
	~CDxBuffer();

	void readData(Vertex* pData, uint32_t size) override;
	void writeData(Vertex* pData, uint32_t size) override;
	void* getBufferObject() override;

	void assign(ID3D11Buffer * pDXOBuffer);

private:
	ID3D11Buffer* m_pDXOBuffer;
};

