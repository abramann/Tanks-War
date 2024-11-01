#pragma once

#include "SystemComponent.h"
#include "Types.h"
#include "ImGui/imgui.h"

inline ImVec4 ColorToVec4(Color color)
{
	float r = color.r / 255.0f;
	float g = color.g / 255.0f;
	float b = color.b / 255.0f;
	float alpha = color.a / 255.0f;
	return ImVec4(r, g, b, alpha);
}

class IUIComponent : public ISystemComponent
{
	virtual void reset()
	{
		m_position = ImVec2(0, 0);
		m_size = ImVec2(0, 0);
		m_color = ImVec4(1, 1, 1, 1);
		m_borderColor = ImVec4(1, 1, 1, 1);
		m_borderSize = 0;
	}

	virtual void draw() = 0;

	virtual void setPosition(int x, int y)
	{
		m_position = ImVec2(x, y);
	}

	virtual void setSize(int width, int height)
	{
		m_size = ImVec2(width, height);
	}

	virtual void setColor(Color color)
	{
		m_color = ColorToVec4(color);
	}

	virtual void setSelectColor(Color color)
	{
		m_selectColor = ColorToVec4(color);
	}

	virtual void setBorderSize(int size)
	{
		m_borderSize = size;
	}

	virtual void setBorderColor(Color color)
	{
		m_borderColor = ColorToVec4(color);
	}

	std::string getName() const override { return "UIComponent"; }

protected:
	ImVec2 m_position;
	ImVec2 m_size;
	ImVec4 m_color;
	ImVec4 m_selectColor;
	int m_borderSize;
	ImVec4 m_borderColor;
};