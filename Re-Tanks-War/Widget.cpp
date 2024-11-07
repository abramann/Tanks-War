#include "Widget.h"
#include "Renderer.h"

void IWidget::setPosition(int x, int y)
{
	setPosition(ImVec2(x, y));
}

void IWidget::setPosition(ImVec2 position)
{
	m_position = position;
}

void IWidget::setSize(int width, int height)
{
	setSize(ImVec2(width, height));
}

void IWidget::setSize(ImVec2 size)
{
	m_size = size;
}

void IWidget::setColor(Color color)
{
	m_color = ColorToVec4(color);
}

void IWidget::setOnHoverColor(Color color)
{
	m_hoverColor = ColorToVec4(color);
}

void IWidget::setBorderSize(float size)
{
	m_borderSize = size;
}

void IWidget::setBorderColor(Color color)
{
	m_borderColor = ColorToVec4(color);
}

void IWidget::setAlign(ImVec2 align)
{
	m_align = align;
}
