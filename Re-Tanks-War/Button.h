#pragma once

#include "Widget.h"

class CButton : public IWidget
{
public:
	void update() override;
	void reset() override;
	void draw() override;

 	void setText(std::string text);
	void setOnClickColor(Color color);
	void setOnHoverColor(Color color);
	void setAlign(ImVec2 align);

protected:	
	bool m_clicked;
 	std::string m_text;
	ImVec4 m_hoverColor = INVALID_VEC4;
	ImVec2 m_align = INVALID_VEC2;
};
 
