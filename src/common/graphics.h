// graphics.h
// Author: abramann
// Note this file is influenced by graphics.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#pragma once

#include "types.h"
#include <memory>

class Game;
class Image;
class Camera;
class Dx11Wrapper;

struct Resolution
{
	Resolution() {}
	Resolution(int16 w, int16 h) : width(w), height(h) {}
	int16 width, height;
};

class Graphics
{
public:

	friend class Camera;

	Graphics();
	~Graphics();
	bool initialize(const Game* game);
	bool loadTexture(const char* file, int32& width, int32& height, LPTextureD3D& texture);
	void beginRender();
	void drawImage(const Image* image);
	void drawPrimitive(uint32 startVertex, uint32 count);
	void endRender();
	void showBackbuffer();
	Resolution getResolution();
	std::vector<Resolution> getSupportedAdapterMode() const;
	std::vector<std::string> getSupportedAdapterModesAsString() const;
	std::string getCurrentAdapterModeAsString() const;
	void setDrawProperties(V3 position = V3(0, 0, 0), V3 scall = V3(1, 1, 1), V3 rotate = V3(0, 0, 0), V3 rotateCenter = V3(0, 0, 0));
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
	std::shared_ptr<Camera> m_pCamera;
	mutable std::vector<Resolution> m_suppModes;
	Dx11Wrapper* m_pDx11Wrapper;
};