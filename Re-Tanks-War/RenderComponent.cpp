#include "RenderComponent.h"

void IRenderComponent::translate(const Vertex& translation)
{
    m_translation = translation;
}

void IRenderComponent::rotate(const float& angle)
{
    m_rotate = angle;
}

void IRenderComponent::scale(const float& scale)
{
    m_scale = scale;
}

Vertex IRenderComponent::getTranslation() const
{
    return m_translation;
}

float IRenderComponent::getRotate() const
{
    return m_rotate;
}

float IRenderComponent::getScale() const
{
    return m_scale;
}

IBuffer* IRenderComponent::getVertexBuffer() const
{
    return m_pVB;
}

IBuffer* IRenderComponent::getIndexBuffer() const
{
    return m_pIB;
}
