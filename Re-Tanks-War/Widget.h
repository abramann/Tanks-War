#pragma once

#include "SystemComponent.h"
#include "ColorTable.h"

class UI;

class IWidget : public ISystemComponent
{
	struct Float
	{
		float f;

		void invalidate()
		{
			f = std::numeric_limits<float>::quiet_NaN();
		}

		bool isNan() 
		{
			return std::isnan(f);
		}
	};

public:
	virtual std::string getName() const { return "Widget"; };

	IWidget() :
		m_position(0, 0)
	{
		m_size.invalidate();
		m_borderSize.invalidate();
		m_color.invalidate();
		m_activeColor.invalidate();
		m_borderColor.invalidate();
	}

	virtual void draw() = 0;

	void setPosition(int x, int y) { setPosition(ImVec2(x, y)); }
	void setPosition(const ImVec2& position) { m_position = position; }
	void setSize(int w, int h) { setSize(ImVec2(w, h)); }
	void setSize(const ImVec2& size) { m_size = size; }
	void setColor(const Color& color) { m_color = color.toVec4(); }
	void setBorderSize(float size) { m_borderSize.f = size; }
	void setBorderColor(const Color& color) { m_borderColor = color.toVec4(); }

protected:
	ImVec2 m_position;
	ImVec2 m_size;
	Float m_borderSize;
	ImVec4 m_color;
	ImVec4 m_activeColor;
	ImVec4 m_borderColor;
};
