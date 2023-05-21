#pragma once
#include "constants.h"
#include "texture.h"

class Game;
class TextureManger;
class Graphics;

const auto VERTICES_IMAGE = 6;
const auto UPDATE_DELAY_IMAGE = 50.0f;;

class Image2
{
public:

	Image2();
	~Image2();
	virtual void initialize(Texture* texture, const Game* game, int8 columns = 1, int8 rows = 1,
		float updateDelay = UPDATE_DELAY_IMAGE);
	virtual void update(const float frameTime);
	virtual void draw() const;
	virtual V3 getRotateCenter() const;

	int16 getHeight() const { return m_height; }
	int16 getVertices() const { return m_vertices; }
	int16 getWidth() const { return m_width; }
	LPTextureD3D getTexture() const { return m_pTexture->getTexture(); }
	LPVertexBuffer getVertexBuffer() const { return m_lpVertexBuffer; }
	inline V3 getPosition() const { return m_position; }
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

protected:

	virtual void createVertexBuffer();
	virtual void setLocalCoordinate();
	virtual void executeAnimateRepeat() { m_row = 1, m_column = 1; }
	

	Graphics* m_pGraphics;
	Texture* m_pTexture;
	int32 m_textureWidth, m_textureHeight;
	float m_updateDelay, m_timeUntilLastUpdate;
	LPVertexBuffer m_lpVertexBuffer;
	V3 m_position, m_scalling, m_rotate;
	int16 m_vertices, m_width, m_height;
	int8_t m_columns, m_rows, m_column, m_row;
	bool m_initialized;

private:

	virtual void updateTextureCoordinate(float frameTime);
	virtual void setNextImageTextureCoordinate();

	bool m_animate;
};
