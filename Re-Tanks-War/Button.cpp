#include "Button.h"
#include "UI.h"

CButton::CButton() :
	m_clicked(false),
	m_text("")
{
	m_hoverColor.invalidate();
	m_align.invalidate();
}

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
	
	if (!m_borderSize.isNan())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, m_borderSize.f);

		num_style_vars++;
	}

	if (!m_align.isNan())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, m_align);

		num_style_vars++;
	}

	int num_color_vars = 0;

	if (!m_color.isNan())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, m_color);

		num_color_vars++;
	}

	if (!m_borderColor.isNan())
	{
		ImGui::PushStyleColor(ImGuiCol_Border, m_borderColor);

		num_color_vars++;
	}

	if (!m_hoverColor.isNan())
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_hoverColor);

		num_color_vars++;
	}

	if (!m_activeColor.isNan())
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

void CButton::setText(const std::string& text)
{
	m_text = text;
}

void CButton::setOnClickColor(const Color& color)
{
	m_activeColor = color.toVec4();
}

void CButton::setOnHoverColor(const Color& color)
{
	m_hoverColor = color.toVec4();
}

void CButton::setAlign(const ImVec2& align)
{
	m_align = align;
}
