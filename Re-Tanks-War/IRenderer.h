#pragma once

#include "types.h"
#include "Buffer.h"
#include "Math.h"
#include <vector>
#include <memory>
#include <set>


class CSprite;
class CModel;
class ITexture;

typedef struct AdapterMode_s
{
	int width;
	int height;
	int format;
} AdapterMode_t;

class IBuffer;

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
	
	virtual ~IRenderer() {}

	virtual void initialize(HWindow hwnd) = 0;
	virtual void beginRendering();
	virtual void renderSprites();
	virtual void renderModels();
	virtual void showBackbuffer() = 0;
	virtual int getBackbufferWidth() const;
	virtual int getBackbufferHeight() const;
	virtual std::vector<AdapterMode_t> getAdapterModes() = 0;
	virtual IBuffer* createVertexBuffer(uint32_t numVertices, Vertex * pInitData, int access) = 0;
	virtual IBuffer* createIndexBuffer(uint32_t length, uint * pInitData, int access) = 0;
	virtual void releaseBuffer(IBuffer* pBuffer) = 0;
	virtual void renderMap() const = 0;
	virtual void drawSprite(const CSprite* pSprite) const = 0;
	virtual void drawModel(const CModel* pModel) const = 0;
	virtual void setWorldViewMatrix(Matrix* mat) = 0;
	virtual ITexture* loadTextureFromFile(const wchar_t* texFileName) = 0;
	virtual void releaseTexture(ITexture* pTexture) = 0;
	virtual void registerSprite(const CSprite* pSprite);
	virtual void removeSprite(const CSprite* pSprite);
	virtual void registerModel(const CModel* pObject);
	virtual void removeObject(const CModel* pObject);

protected:

	int m_backbufferWidth;
	int m_backbufferHeight;
	std::set<const CSprite*> m_pRenderedSprites;
	std::set<const CModel*> m_pRenderedModels;
};

extern IRenderer* g_pRenderer;