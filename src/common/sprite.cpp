// sprite.cpp
// Author: abramann

#include "sprite.h"
#include "game.h"
#include "graphics.h"
#include "timer.h"
#include "texturemanger.h"
#include "texture.h"

#pragma warning(disable : 4244)

using namespace ImGui;

constexpr float FADE_AMOUNT = 0.0005f;

Sprite::Sprite() : m_fade(graphicsNS::FADE_INC), m_fadeInc(graphicsNS::FADE_INC)
{
	m_windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
}

Sprite::Sprite(const Game * const pGame, std::string texture, V2 position, V2 scaling) : m_fade(graphicsNS::FADE_INC), m_fadeInc(graphicsNS::FADE_INC)
{
	m_windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	initialize(pGame, texture, position, scaling);
}

Sprite::~Sprite()
{
}

void Sprite::initialize(const Game* const pGame, std::string texture, V2 position, V2 scaling)
{
	m_pGraphics = pGame->getGraphics();
	m_pTimer = pGame->getTimer();
	auto textureManger = pGame->getTextureManger();
	Texture* tex = textureManger->getTexture(texture);
	m_width = tex->getWidth(),
		m_height = tex->getHeight();
	m_spriteTexture = tex->getTexture();
	m_position = position,
		m_scaling = scaling;
}

void Sprite::draw()
{
	Vec2 size = Vec2(m_width, m_height),
		position = Vec2(m_position.x, m_position.y);
	SetNextWindowSize(size);
	SetNextWindowPos(position);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2(0, 0));
	Begin(" ", 0, m_windowFlags);
	ImGui::Image(m_spriteTexture, size);
	End();
	PopStyleVar();
}

bool Sprite::fadeDraw(int64 time)
{
	Vec2 size = Vec2(m_width*m_scaling.x, m_height*m_scaling.y),
		position = Vec2(m_position.x, m_position.y);
	static float alpha = 0.0f;
	SetNextWindowSize(size);
	SetNextWindowPos(position);
	PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2(0, 0));
	Begin(" ", 0, m_windowFlags);
	alpha += m_fade;
	ImGui::Image(m_spriteTexture, size, Vec2(0, 0), Vec2(1, 1), Vec4(1, 1, 1, alpha));
	End();
	PopStyleVar();
	if (alpha > 1.001f)
	{
		alpha = 1.0f,
			m_fade = 0;
		if (m_viewTimeEnd == 0)
			m_viewTimeEnd = time + m_pTimer->getCurrentTime();
	}
	else if (m_pTimer->getCurrentTime() >= m_viewTimeEnd && m_fade == 0)
		m_fade = -abs(m_fadeInc);
	else if (alpha < 0.0f)
	{
		alpha = 0;
		m_fade = abs(m_fade);
		m_viewTimeEnd = 0;
		return true;
	}

	return false;
}