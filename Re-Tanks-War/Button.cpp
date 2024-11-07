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

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, m_borderSize);
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, m_align);
	
	// Colors
	ImGui::PushStyleColor(ImGuiCol_Button, m_color);
	ImGui::PushStyleColor(ImGuiCol_Border, m_borderColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_hoverColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_activeColor);

	bool clicked = ImGui::Button(m_text.c_str(), m_size);
	if (!m_clicked)
		m_clicked = clicked;

	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(2);
}

void CButton::setText(std::string text)
{
	m_text = text;
}

void CButton::setOnClickColor(Color color)
{
	m_activeColor = ColorToVec4(color);
}
