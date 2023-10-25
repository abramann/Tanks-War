// image.h
// Author: abramann

#pragma once

#include "types.h"
#include "texture.h"
#include <wrl\client.h>

class Game;
class TextureManger;
class Graphics;
class Timer;
class Dx11Wrapper;

class Image
{
public:

	Image();
	~Image();
	virtual void initialize(const std::string& texture, const Game* pGame, int8 columns = 1, int8 rows = 1,
		int32 updateDelay = logicNS::UPDATE_DELAY_IMAGE);
	virtual void update();
	virtual void draw() const;
	virtual V3 getRotateCenter() const;
	int16 getHeight() const { return m_height; }
	int16 getWidth() const { return m_width; }
	LPTextureD3D getTexture() const { return m_pTexture->getTexture(); }
	DxBuffer* getVertexBuffer() const { return m_pVertexBuffer.Get(); }
	V3 getPosition() const { return m_position; }
	V3 getRotate() const { return m_rotate; }
	V3 getScalling() const { return m_scalling; }
	void addRotateX(float addRX) { m_rotate.x += addRX; }
	void addRotateY(float addRY) { m_rotate.y += addRY; }
	void addRotateZ(float addRZ) { m_rotate.z += addRZ; }
	void addX(float addX) { m_position.x += addX; }
	void addY(float addY) { m_position.y += addY; }
	void addZ(float addZ) { m_position.z += addZ; }
	void setPosition(V3 positon) { m_position = positon; }
	void setRotate(V3 rotate) { m_rotate = rotate; }
	void setRotateX(float rX) { m_rotate.x = rX; }
	void setRotateY(float rY) { m_rotate.y = rY; }
	void setRotateZ(float rZ) { m_rotate.z = rZ; }
	void setX(float x) { m_position.x = x; }
	void setY(float y) { m_position.y = y; }
	void setZ(float z) { m_position.z = z; }
	Space getSpace(float x0 = 0, float y0 = 0) const;
	float getNegativeRotate() const { return (m_rotate.z <= 0) ? m_rotate.z :-2 * PI + m_rotate.z; };

protected:

	virtual void createVertexBuffer();
	virtual void updateLocalCoordinate();
	virtual void executeAnimateRepeat() { m_row = 1, m_column = 1; }

	Graphics* m_pGraphics;
	Texture* m_pTexture;
	TextureManger* m_pTextureManger;
	Timer* m_pTimer;
	Dx11Wrapper* m_pDx11Wrapper;
	int32 m_textureWidth, m_textureHeight;
	int32 m_updateDelay;
	int64 m_timeUntilLastUpdate;
	Microsoft::WRL::ComPtr<DxBuffer> m_pVertexBuffer, m_pStagingBuffer;
	V3 m_position, m_scalling, m_rotate;
	int16 m_width, m_height;
	int8_t m_columns, m_rows, m_column, m_row;
	bool m_initialized;

private:

	virtual void updateTextureCoordinate(int64 frameTime);
	virtual void setNextImageTextureCoordinate();
	mutable V3* m_edge[4];
	mutable Space m_space;
	mutable float m_spaceAdd1, m_spaceAdd2;
	bool m_animate;
};