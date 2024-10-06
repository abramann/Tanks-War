#pragma once

#include "VecMath.h"
#include "types.h"
#include <vector>
#include "Values.h"

class IBuffer;
class ITexture;

struct Face
{
	ITexture* pTexture;
	Vertex  origin;
	Vertex rotate;
	Color color;
};

class CModel
{
	static std::vector<std::pair<IBuffer*, IBuffer*>> s_pBuffers;
	static std::vector<float> s_radius;
	static std::vector<std::vector<Face>> s_faces;

	friend class RENDER_CLASS;
public:
	CModel();
	virtual ~CModel();

	virtual void setPosition(Vertex newPosition);
	virtual Vertex getPosition() const;

	static CModel* loadOBJModel(const char* modelPath);

private:
	IBuffer* getVertexBuffer() const;
	IBuffer* getIndexBuffer() const; 
	void assignVertexBuffer(IBuffer* pVB);
	void assignIndexBuffer(IBuffer* pIB);

	void setFaces(std::vector<Face> faces);
	std::vector<Face> getFaces() const;
	
	float radius;
	std::vector<Face> m_faces;
	Vertex m_position;
	IBuffer* m_pVertexBuffer;
	IBuffer* m_pIndexBuffer;
};

extern CModel* loadOBJModel(const char* modelPath);
