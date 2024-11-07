#pragma once

#include "SystemComponent.h"
#include "ColorTable.h"

class UI;

class IWidget : public ISystemComponent
{
public:
	virtual std::string getName() const { return "Widget"; };

	IWidget() :
		m_position(0, 0),
		m_align(0, 0),
		m_size(0, 0),
		m_borderSize(0)
	{}

	void reset() override {}

	virtual void draw() = 0;

	void setPosition(int x, int y);
	void setPosition(ImVec2 position);
	void setSize(int width, int height);
	void setSize(ImVec2 size);
	void setColor(Color color);
	void setOnHoverColor(Color color);
	void setBorderSize(float size);
	void setBorderColor(Color color);
	void setAlign(ImVec2 align);

protected:
	ImVec2 m_position;
	ImVec2 m_align;
	ImVec2 m_size;
	float m_borderSize;
	// Colors
	ImVec4 m_color;
	ImVec4 m_hoverColor;
	ImVec4 m_onClickColor;
	ImVec4 m_borderColor;
};