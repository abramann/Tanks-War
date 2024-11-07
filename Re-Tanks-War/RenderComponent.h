#pragma once

#include "SystemComponent.h"
#include "VecMath.h"

class IBuffer;

class IRenderComponent : public ISystemComponent
{
public:
	virtual std::string getName() const { return "RenderComponent"; }

    void translate(const Vertex& translation);
    void rotate(const float& angle);
    void scale(const float& scale);
    Vertex getTranslation() const;
    float getRotate() const;
    float getScale() const;
    IBuffer* getVertexBuffer() const;
    IBuffer* getIndexBuffer() const;

protected:
    Vertex m_translation;
    float m_rotate;
    float m_angle;
    float m_scale;
    IBuffer* m_pVB;
    IBuffer* m_pIB;
};