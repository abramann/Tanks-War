#pragma once

#include "UIComponent.h"
#include "ImGui/imgui.h"

class CButton : public IUIComponent
{
public:
	virtual void update();
	virtual void reset();
	virtual void draw();
	virtual bool wasClicked() const;
 	virtual void setText(std::string text);
 	
protected:
	mutable bool m_clicked;
 	std::string m_text;
 };

