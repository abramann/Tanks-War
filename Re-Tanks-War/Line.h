#pragma once

#include "VecMath.h"
#include "IImage.h"




class IBuffer;

class CLine : public IImage
{
	const float DEFAULT_THICKNESS = 4.0f;
	const Color DEFAULT_COLOR = makeColor(1, 1, 1);
	const bool DEFAULT_RELATIVECAMERA = false;
public:
	CLine();
	CLine(Vertex begin, Vertex end);
	~CLine();

	void setBegin(Vertex newBegin);
	void setEnd(Vertex newEnd);
	void setBeginEnd(Vertex newBegin, Vertex newEnd);
	Vertex getBegin() const;
	Vertex getEnd() const;
	void setThickness(float newThickness);
	float getThickness() const;
	Rect makeRect();

	static void makeClockwiseOrder(Vertex* pVertices, int numVertices);
	
private:
	void createVertexBuffer() override;
	void createIndexBuffer() override;
	void updateVertexBuffer() override;

	Rect8 makeVertexBufferData(Rect rect);

	Vertex m_begin;
	Vertex m_end;
	float m_thickness;
	IBuffer* m_pVertexBuffer;
	IBuffer* m_pIndexBuffer;
};
