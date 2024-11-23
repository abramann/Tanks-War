#pragma once

#include "types.h"
#include "Math.h"
#include <vector>

struct AdapterMode
{
	int width;
	int height;
	int format;
};

class IBuffer;
class CSprite;
class ITexture;
class IModel;

struct MeshSegmentData
{
	int indexCount;
	ITexture* pTexture;
};

struct MeshData
{
	std::vector<MeshSegmentData> segments;
	IBuffer* pVB, * pIB;
};

class IRenderer
{
public:
	enum
	{
		BUFFER_IMMUTABLE,
		BUFFER_READ_ONLY,
		BUFFER_WRITE_ONLY,
		BUFFER_READ_WRITE
	};
	
	virtual void initialize() = 0;
	virtual void newFrame() = 0;
	virtual void renderFrame() = 0;
	virtual std::vector<AdapterMode>& getAdapterModes() = 0;
	virtual IBuffer* createVertexBuffer(uint32_t numVertices, Vertex * pInitData, int access) = 0;
	virtual IBuffer* createIndexBuffer(uint32_t length, uint * pInitData, int access) = 0;
	virtual void bindVertexBuffer(IBuffer* pBuffer, uint32_t startIndex = 0) = 0;
	virtual void bindIndexBuffer(IBuffer* pBuffer, uint32_t startIndex = 0) = 0;
	virtual void bindTexture(ITexture* pTexuture) = 0;
	virtual void releaseBuffer(IBuffer* pBuffer) = 0;
	virtual void renderUI() = 0;
	virtual void drawMesh(const MeshData& meshData) = 0;
	virtual void drawSprite(const CSprite* pSprite) = 0;
	virtual void drawModel(const IModel* pModel) = 0;
	virtual void setWorldViewMatrix(Matrix* mat) = 0;
	virtual class ITexture* loadTextureFromFile(const wchar_t* texFileName) = 0;
	virtual void releaseTexture(ITexture* pTexture) = 0;

	int getBackbufferWidth() const { return m_backbufferWidth; };
	int getBackbufferHeight() const { return m_backbufferHeight; };
	void getRelativeScreenPoint(float& x, float& y) const {
		x *= m_backbufferWidth / 100; y *= m_backbufferHeight / 100; };

protected:
	int m_backbufferWidth;
	int m_backbufferHeight;
};

extern IRenderer* g_pRenderer;