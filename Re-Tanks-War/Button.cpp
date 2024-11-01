#include "Button.h"

void CButton::update()
{
}

void CButton::reset()
{
	m_text = "";
	m_clicked = false;
}

void CButton::draw()
{
	ImGui::SetCursorPos(m_position);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, m_color);
	//ImGui::PushStyleColor(ImGuiCol_)
	//m_clicked = ImGui::Button(m_text.c_str(), m_size);
	ImGui::PopStyleColor(2);

}

bool CButton::wasClicked() const
{
	return m_clicked;
}

void CButton::setText(std::string text)
{
	m_text = text;
}
