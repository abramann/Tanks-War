// sprite.h
// Author: abramann

#pragma once
#include "types.h"
#include "imgui\imgui.h"

class Game;
class Graphics;
class Timer;

class Sprite
{
public:
	Sprite();
	Sprite(const Game* const pGame, std::string texture, V2 position = V2(0, 0), V2 scaling = V2(1, 1));
	~Sprite();

	void initialize(const Game* const pGame, std::string texture, V2 position = V2(0, 0), V2 scaling = V2(1, 1));
	void draw();
	bool fadeDraw(int64 time);
	void setPosition(V2 position) { m_position = position; }
	V2 getPosition() const { return m_position; }
	void setScaling(V2 scaling) { m_scaling = scaling; }
	int32 getWidth() const { return m_width; }
	int32 getHeight() const { return m_height; }

private:

	ImTextureID m_spriteTexture;
	Graphics* m_pGraphics;
	Timer* m_pTimer;
	V2 m_position, m_scaling, m_rotate;
	int32 m_width, m_height;
	int32 m_windowFlags;
	int64 m_viewTimeEnd;
	float m_fade;
	float m_fadeInc;
};
