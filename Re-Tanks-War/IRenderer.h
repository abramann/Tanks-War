#pragma once

#include "types.h"
#include "IBuffer.h"
#include "Color.h"
#include <vector>
#include <memory>
#include <set>



class CLine;
class IImage;
class IImage;
class CTankModel;
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
	virtual void showBackbuffer() = 0;
	virtual int getBackbufferWidth() const;
	virtual int getBackbufferHeight() const;
	virtual std::vector<AdapterMode_t> getAdapterModes() = 0;
	virtual IBuffer* createVertexBuffer(uint32_t numVertices, Vertex * pInitData, int access) = 0;
	virtual IBuffer* createIndexBuffer(uint32_t length, int * pInitData, int access) = 0;
	virtual void releaseBuffer(IBuffer* pBuffer) = 0;
	virtual void drawImage(const IImage* pImage) const = 0;
	virtual void drawTank(const CTankModel* pTank) const = 0;
	virtual ITexture* loadTextureFromFile(const wchar_t* texFileName) = 0;
	virtual void releaseTexture(ITexture* pTexture) = 0;
	virtual void registerImage(const IImage* pImage);
	virtual void removeImage(const IImage* pImage);
	virtual void registerTank(const CTankModel* pTankModel);
	virtual void removeTank(const CTankModel* pTankModel);

protected:

	int m_backbufferWidth;
	int m_backbufferHeight;
	std::set<const IImage*> m_pRenderedImages;
	std::set<const CTankModel*> m_pRenderedTanks;
};

extern IRenderer* g_pRenderer;