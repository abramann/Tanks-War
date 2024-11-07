#pragma once

#include "Widget.h"

class CButton : public IWidget
{
public:
	virtual void update();
	virtual void reset();
	virtual void draw();
 	virtual void setText(std::string text);
	virtual void setOnClickColor(Color color);

protected:	
	mutable bool m_clicked;
 	std::string m_text;
	ImVec4 m_activeColor;
 };
 
