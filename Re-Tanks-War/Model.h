#pragma once

#include "VecMath.h"
#include "ColorTable.h"
#include "Values.h"
#include "Subsystem.h"
#include "RenderComponent.h"
#include <vector>

class IBuffer;
class ITexture;

struct Face
{
	ITexture* pTexture;
	Vertex  origin;
	Vertex rotate;
	Color color;
};

class CMesh : public ISystem, public IRenderComponent
{
	static std::vector<std::pair<IBuffer*, IBuffer*>> s_pBuffers;
	static std::vector<float> s_radius;
	static std::vector<std::vector<Face>> s_faces;

	friend class RENDER_CLASS;
public:
	std::string getName() const override { return "Mesh"; }
	
	~CMesh();
	
	virtual void startup() = 0;
	virtual void update() = 0;
	virtual void reset() = 0;
	virtual void perform() = 0;
	virtual void onStartGame() = 0;
	virtual void onQuitGame() = 0;
	virtual void onPauseGame() = 0;
	virtual void onResumGame() = 0;
	virtual void registerComponent(ISystemComponent* pComponent) = 0;
	virtual void unregisterComponent(ISystemComponent* pComponent) = 0;
	virtual void handleEvent(ISystemComponent* pComponent, int eventCode, void* event) {};

	// void loadOBJFile
	static CMesh* loadOBJModel(const char* modelPath);

private:
	void setFaces(std::vector<Face> faces);
	std::vector<Face> getFaces() const;
private:
	float radius;
	std::vector<Face> m_faces;
	Vertex m_position;
	IBuffer* m_pVertexBuffer;
	IBuffer* m_pIndexBuffer;
};

extern CMesh* loadOBJModel(const char* modelPath);
