#include "Model.h"
#include "Renderer.h"
#include "File.hpp"
#include "String.h"
#include "GameError.h"
#include "Renderer.h"
#include "GameData.h"

using namespace evt;
using namespace std;

vector<float> CModel::s_radius;
std::vector<std::vector<Face>> CModel::s_faces;
std::vector<std::pair<IBuffer*, IBuffer*>> CModel::s_pBuffers;

CModel* CModel::loadOBJModel(const char* name)
{
	CModel* model = new CModel();

	std::hash<std::string> hasher;
	uint hash = hasher(name);

	static std::map<uint, int> s_loaded;

	int i = s_loaded[hash];
	if (i > 0)
	{
		model->assignVertexBuffer(s_pBuffers[i].first);
		model->assignIndexBuffer(s_pBuffers[i].second);
		model->m_faces = s_faces[i];
		model->radius = s_radius[i];
	}
	struct FaceVertices
	{
		Vertex v[4];
	}; 
	
	std::vector<Face> modelFaces;
	Face parsedFace;
	std::vector<FaceVertices> vertices;
	std::vector<uint> indices;


	strFormat("%s%s", fileNS::MODEL_DIR, name);
	auto hFile = FileIO::plainText(name);
	string line;

	uint vertIndex = 0;
	while (line = hFile.read(), true)
	{
		if (line.empty())
		{
			modelFaces.push_back(parsedFace);
			break;
		}
		if (strIsEqual(line, "o"))
		{
			if (vertices.size() == 0) // If on first object
			{
				vertices.push_back(FaceVertices());
				continue;
			}
			modelFaces.push_back(parsedFace);

			setNull(parsedFace);
			parsedFace.color = Color();

			vertices.push_back(FaceVertices());
			vertIndex = vertices.size() - 1;
			modelFaces.back().color = Color();
			continue;
		}
		if (strIsEqual(line, "v"))
		{
			// Read vertices
			for (int i = 0; i < 4; i++)
			{
				vertices[vertIndex].v[i].y = stof(hFile.read());
				vertices[vertIndex].v[i].x = stof(hFile.read());
				vertices[vertIndex].v[i].z = stof(hFile.read());
				if (i < 3)
					hFile.read();
			}

			// indices
			indices.push_back(1);
			indices.push_back(3);
			indices.push_back(2);

			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(0);
		}
		else if (strIsEqual(line, "vn"))
		{
			// Read normal
			hFile.getline();
		}
		else if (strIsEqual(line, "vt"))
		{
			// Read texture coordinates

			for (int i = 0; i < 4; i++)
			{
				if (i == 3 || i == 0)
				{
					vertices[vertIndex].v[i].u = 1 - stof(hFile.read());
					vertices[vertIndex].v[i].v = 1 - stof(hFile.read());
					if (i == 0)
						hFile.read();
					continue;
				}
				vertices[vertIndex].v[i].u = stof(hFile.read());
				vertices[vertIndex].v[i].v = stof(hFile.read());
				hFile.read();
			}
		}
		else if (strIsEqual(line, "usemtl"))
		{
			line = hFile.read();
			parsedFace.pTexture = g_pRenderer->loadTextureFromFile(to_wstring(line).c_str());
		}
		else if (strIsEqual(line, "org"))
		{
			// Read origin coordinates
			parsedFace.origin.x = stof(hFile.read());
			parsedFace.origin.y = stof(hFile.read());
			parsedFace.origin.z = stof(hFile.read());
		}
	}
	
	IBuffer* vb = g_pRenderer->createVertexBuffer(vertices.size() * 4, (Vertex*) & vertices[0], IRenderer::BUFFER_IMMUTABLE);
	model->assignVertexBuffer(vb);

	IBuffer* ib = g_pRenderer->createIndexBuffer(sizeof(int) * indices.size(), (uint*) & indices[0], IRenderer::BUFFER_IMMUTABLE);
	model->assignIndexBuffer(ib);

	model->setFaces(modelFaces);

	return model;
}

CModel::CModel()
{
	g_pRenderer->registerModel(this);
}

CModel::~CModel()
{
	g_pRenderer->releaseBuffer(m_pVertexBuffer);
	g_pRenderer->releaseBuffer(m_pIndexBuffer);
	//for (auto pTexture : m_pTextures)
	//	g_pRenderer->releaseTexture(pTexture);
}

void CModel::setPosition(Vertex newPosition)
{
	m_position = newPosition;
}

Vertex CModel::getPosition() const
{
	return m_position;
}

IBuffer * CModel::getVertexBuffer() const
{
	return m_pVertexBuffer;
}

IBuffer * CModel::getIndexBuffer() const
{
	return m_pIndexBuffer;
}

void CModel::assignVertexBuffer(IBuffer* pVB)
{
	m_pVertexBuffer = pVB;
}

void CModel::assignIndexBuffer(IBuffer* pIB)
{
	m_pIndexBuffer = pIB;
}

void CModel::setFaces(std::vector<Face> faces)
{
	m_faces = faces;
}

std::vector<Face> CModel::getFaces() const
{
	return m_faces;
}
