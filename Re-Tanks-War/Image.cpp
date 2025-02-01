#include "Image.h"
#include "Renderer.h"
#include "StrUtility.h"

void CImage::draw()
{
	if (!m_pTexture)
		return;

	ImGui::SetCursorPos(m_position);

	int style_vars = 0;

	if (!m_borderSize.isNan())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, m_borderSize.f);
		style_vars++;
	}

	ImGui::Image(m_pTexture, m_size,
		ImVec2(0, 0), ImVec2(1, 1),
		m_color, m_borderColor);

	ImGui::PopStyleVar(style_vars);
}

void CImage::setTexture(ITexture* pTexture)
{
	m_pTexture = pTexture;
}

void CImage::update()
{
}

void CImage::reset()
{
}
