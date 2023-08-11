// graphics.h
// Author: abramann
// Note this file is influenced by graphics.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#pragma once

#include "types.h"
#include <memory>
#ifdef _BUILD_WITH_D3D11
#include <wrl\client.h>
#endif

class Game;
class Image;
class Camera;
class Dx11Wrapper;

#ifdef _BUILD_WITH_D3D9
typedef LPDIRECT3DDEVICE9 LPDevice;
#else ifdef _BUILD_WITH_D3D11
typedef ID3D11Device* LPDevice;
#endif

struct Resolution
{
	int16 width, height;
};

class Graphics
{
public:

	friend class Camera;

	Graphics();
	~Graphics();
	bool initialize(const Game* game);
	bool isWindowed() const;
	bool loadTexture(const char* file, int32& width, int32& height, LPTextureD3D& texture);
	static Matrix V3ToMatrix(const V3 v3, MatrixType type);
	void beginRender();
	void drawImage(const Image* image);
	void drawPrimitive(uint32 startVertex, uint32 count);
	void endRender();
	void showBackbuffer();
	Resolution getResolution();
	std::vector<Resolution> getSupportedResolutions() const;
	void release();
	void resize(uint16_t width, uint16_t height);
	void setDrawProperties(V3 position = V3(0, 0, 0), V3 scall = V3(1, 1, 1), V3 rotate = V3(0, 0, 0), V3 rotateCenter = V3(0, 0, 0));
	void setWindowed(const bool& windowed);
	void setResolution(const Resolution& resolution);
	void setTexture(LPTextureD3D texture);
	void setWorldMatrix(Matrix* worldMatrix);
	bool checkFullscreenSupport() const;
	Camera* getCamera() const { return m_pCamera.get(); }
	LPVertexBuffer createVertexBuffer(uint32 vertices, Vertex* data = 0);
	void Graphics::setVertexBuffer(LPVertexBuffer vb, Vertex * vertex, int32_t vertices);
	void streamVertexBuffer(LPVertexBuffer vb);
	void Graphics::setVertexBufferUV(LPVertexBuffer vb, Vertex * vertex, int8 len);

private:

	void setViewMatrix(Matrix* wvp);

	HWND m_hwnd;


	
	Matrix m_wvp;

	DWORD m_deviceState;
	std::shared_ptr<Camera> m_pCamera;
	Dx11Wrapper* m_pDx11Wrapper;
};