// graphics.h
// Author: abramann
// Note this file is influenced by graphics.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#pragma once

#include "types.h"
#include <memory>
#include <wrl\client.h>

class Game;
class Image;
class Camera;
class Dx11Wrapper;
class Line;

struct Resolution
{
	Resolution() {}
	Resolution(int16 w, int16 h) : width(w), height(h) {}
	int16 width, height;
};

class Graphics
{
public:

	friend Camera;

	Graphics();
	~Graphics();
	bool initialize(const Game* pGame);
	bool loadTexture(const std::string& file, uint32& width, uint32& height, LPTextureD3D& texture);
	void beginRender();
	void drawImage(const Image* image);
	void drawLine(const Vector3D& line);
	void drawBox(const Space& space, const float& size = 1);
	void drawPrimitive(uint32 startVertex, uint32 count);
	void endRender();
	void showBackbuffer();
	std::vector<std::string> getSupportedResolutionAsString() const;
	int32 getIndexCurrentResolution() const;
	void onResize();
	void setDrawProperties(V3 position = V3(0, 0, 0), V3 scall = V3(1, 1, 1), V3 rotate = V3(0, 0, 0), V3 rotateCenter = V3(0, 0, 0));
	void setTexture(LPTextureD3D texture);
	void setObjectConstBuffer(void* pData);
	void setWorldViewMatrix(Matrix* worldViewMatrix);
	Camera* getCamera() const { return m_pCamera.get(); }

private:

	HWND m_hwnd;
	Matrix m_wvp;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Line> m_pLine;
	mutable std::vector<Resolution> m_suppModes;
	Dx11Wrapper* m_pDx11Wrapper;
	Microsoft::WRL::ComPtr<DxBuffer> m_pWVMBuf, m_pImageMatrixBuf, m_pImageIndexBuf, m_pViewport;
};