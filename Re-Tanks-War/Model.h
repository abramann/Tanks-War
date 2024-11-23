#pragma once

#include "RenderComponent.h"
#include <vector>

class IBuffer;
class ITexture;

struct Poly
{
	ITexture* pTexture;
	Vertex origin;
	Vertex rotate;
	Vertex scale;
};

class IModel : public IRenderComponent
{
public:
	std::string getName() const override { return "Model"; }

	~IModel();

	void parse(const std::string& modelPath);
	const std::vector<Poly>& getPolys() const;

private:
	friend class DiskLoader;

	void assignPolys(const std::vector<Poly>& polys);
	void assignVertexBuffer(IBuffer* pBuffer);
	void assignIndexBuffer(IBuffer* pBuffer);

protected:
	std::vector<Poly> m_polys;
};
