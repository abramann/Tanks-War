#pragma once

#include "Widget.h"

class CButton : public IWidget
{
public:
	CButton();

	void update() override;
	void reset() override;
	void draw() override;

 	void setText(const std::string& text);
	void setOnClickColor(const Color& color);
	void setOnHoverColor(const Color& color);
	void setAlign(const ImVec2& align);

protected:	
	bool m_clicked;
 	std::string m_text;
	ImVec4 m_hoverColor;
	ImVec2 m_align;
};
 
