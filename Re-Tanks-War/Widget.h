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
		m_size(0, 0),
		m_borderSize(0)
	{}

	virtual void draw() = 0;

	void setPosition(int x, int y);
	void setPosition(ImVec2 position);
	void setSize(int width, int height);
	void setSize(ImVec2 size);
	void setColor(Color color);
	void setBorderSize(float size);
	void setBorderColor(Color color);

protected:
	const ImVec4 INVALID_VEC4 = ImVec4(-6553, -6553, -6553, -6553);
	const float INVALID_BORDERSIZE = -1;
	const ImVec2 INVALID_VEC2 = ImVec2(-6553, -6553); 
	
	ImVec2 m_position = INVALID_VEC2;
	ImVec2 m_size = ImVec2(0, 0);
	float m_borderSize = -1;
	// Colors
	ImVec4 m_color = INVALID_VEC4;
	ImVec4 m_activeColor = INVALID_VEC4;
	ImVec4 m_borderColor = INVALID_VEC4;
};