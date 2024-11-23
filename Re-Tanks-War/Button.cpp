#include "Button.h"
#include "UI.h"

void CButton::update()
{
	if (m_clicked)
	{
		m_pSystemHandler->handleEvent(this, UI::BUTTON_CLICKED, nullptr);
		m_clicked = false;
	}
}

void CButton::reset()
{
	m_text = "";
	m_clicked = false;
}

void CButton::draw()
{
	ImGui::SetCursorPos(m_position);
	
	int num_style_vars = 0;
	
	if (m_borderSize != INVALID_BORDERSIZE)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, m_borderSize);
		num_style_vars++;
	}

	if (m_align != INVALID_VEC2)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, m_align);
		num_style_vars++;
	}

	int num_color_vars = 0;

	if (m_color != INVALID_VEC4)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, m_color);
		num_color_vars++;
	}

	if (m_borderColor != INVALID_VEC4)
	{
		ImGui::PushStyleColor(ImGuiCol_Border, m_borderColor);
		num_color_vars++;
	}

	if (m_hoverColor != INVALID_VEC4)
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_hoverColor);
		num_color_vars++;
	}

	if (m_activeColor != INVALID_VEC4)
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_activeColor);
		num_color_vars++;
	}
	
	bool clicked = ImGui::Button(m_text.c_str(), m_size);
	if (!m_clicked)
		m_clicked = clicked;

	ImGui::PopStyleVar(num_style_vars);
	ImGui::PopStyleColor(num_color_vars);
}

void CButton::setText(std::string text)
{
	m_text = text;
}

void CButton::setOnClickColor(Color color)
{
	m_activeColor = ColorToVec4(color);
}

void CButton::setOnHoverColor(Color color)
{
	m_hoverColor = ColorToVec4(color);
}

void CButton::setAlign(ImVec2 align)
{
	m_align = align;
}
