#include "Model.h"
#include "Renderer.h"
#include "File.hpp"
#include "String.h"
#include "GameError.h"
#include "Renderer.h"
#include "Values.h"
#include <map>

using namespace evt;
using namespace std;

vector<float> CMesh::s_radius;
std::vector<std::vector<Face>> CMesh::s_faces;
std::vector<std::pair<IBuffer*, IBuffer*>> CMesh::s_pBuffers;

CMesh* CMesh::loadOBJModel(const char* name)
{/*
	static std::map<size_t, int> s_loaded;
	
	CMesh* model = new CMesh();

	size_t hash = strHash(name);

	auto it = s_loaded.find(hash);

	if (it != s_loaded.end())
	{
		int index = it->second;

		model->assignVertexBuffer(s_pBuffers[index].first);
		model->assignIndexBuffer(s_pBuffers[index].second);
		model->m_faces = s_faces[index];
		model->radius = s_radius[index];
		return model;
	}

	// TODO: Remove garbage
	struct FaceVertices
	{
		Vertex v[4];
	};
	
	std::vector<Face> modelFaces;
	Face parsedFace;
	std::vector<FaceVertices> vertices;
	std::vector<uint> indices;

	strFormat("%s%s", values::MODEL_DIR, name);
	auto hFile = FileIO::plainText(name);
	string line;

	size_t vertIndex = 0;
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
				vertices.emplace_back();
				continue;
			}

			modelFaces.push_back(parsedFace);

			setNull(parsedFace);
			parsedFace.color = Color();

			vertices.emplace_back();
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

	return model;*/
return nullptr;
}

CMesh::~CMesh()
{
	g_pRenderer->releaseBuffer(m_pVertexBuffer);
	g_pRenderer->releaseBuffer(m_pIndexBuffer);
	
	//for (auto pTexture : m_pTextures)
	//	g_pRenderer->releaseTexture(pTexture);
}

void CMesh::setFaces(std::vector<Face> faces)
{
	m_faces = faces;
}

std::vector<Face> CMesh::getFaces() const
{
	return m_faces;
}
