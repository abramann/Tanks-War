#pragma once

#include "SystemComponent.h"
#include "VecMath.h"

class IBuffer;

class IRenderComponent : public ISystemComponent
{
public:
	virtual std::string getName() const { return "RenderComponent"; }

    void translate(const Vertex& translation) { m_translation = translation; };
    void rotate(const float& angle) { m_rotate = angle; };
    void scale(const float& scale) { m_scale = scale; };

    Vertex getTranslation() const { return m_translation; };
    float getRotate() const { return m_rotate; };
    float getScale() const { return m_scale; };
    IBuffer* getVertexBuffer() const { return m_pVB; };
    IBuffer* getIndexBuffer() const { return m_pIB; };

protected:
    Vertex m_translation;
    float m_rotate;
    float m_angle;
    float m_scale;
    IBuffer* m_pVB;
    IBuffer* m_pIB;
};