#include "GameError.h"
#include "DiskLoader.h"
#include "Renderer.h"
#include "File.hpp"
#include "Values.h"
#include <map>

// Only for releasing copies with dtor, won't be used anywhere
static DiskLoader dummy;

std::vector<std::vector<Poly>> DiskLoader::s_polys;
std::vector<std::pair<IBuffer*, IBuffer*>> DiskLoader::s_pBuffers;

using namespace evt;

DiskLoader::~DiskLoader()
{
	releaseAllCopies();
}

void DiskLoader::loadBinary(const std::string& filepath, uchar* pInBuf, bool savecopy)
{
}

void DiskLoader::parseModel(IModel* model, const std::string& modelName)
{
	static std::map<size_t, int> s_loaded;

	size_t hash = strHash(modelName);

	auto it = s_loaded.find(hash);

	if (it != s_loaded.end())
	{
		int index = it->second;

		model->assignVertexBuffer(s_pBuffers[index].first);
		model->assignIndexBuffer(s_pBuffers[index].second);
		model->assignPolys(s_polys[index]);
		return;
	}

	// TODO: Do in better syntax.
	struct PolyVertices
	{
		Vertex v[4];
	};

	std::vector<Poly> modelQuads;
	Poly parsedPoly;
	std::vector<PolyVertices> vertices;
	std::vector<uint> indices;

	auto hFile = FileIO::plainText(strFormat("%s%s", values::MODEL_DIR, modelName));
	std::string line;

	size_t vertIndex = 0;
	while (line = hFile.read(), true)
	{
		if (line.empty())
		{
			modelQuads.push_back(parsedPoly);
			break;
		}

		if (strIsEqual(line, "o"))
		{
			if (vertices.size() == 0) // If on first object
			{
				vertices.emplace_back();
				continue;
			}

			modelQuads.push_back(parsedPoly);


			vertices.emplace_back();
			vertIndex = vertices.size() - 1;
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

			indices.reserve(6);

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
			parsedPoly.pTexture = g_pRenderer->loadTextureFromFile(to_wstring(line).c_str());
		}
		else if (strIsEqual(line, "org"))
		{
			// Read origin coordinates
			parsedPoly.origin.x = stof(hFile.read());
			parsedPoly.origin.y = stof(hFile.read());
			parsedPoly.origin.z = stof(hFile.read());
		}
	}

	IBuffer* vb = g_pRenderer->createVertexBuffer(vertices.size() * 4, (Vertex*)&vertices[0], IRenderer::BUFFER_IMMUTABLE);
	model->assignVertexBuffer(vb);

	IBuffer* ib = g_pRenderer->createIndexBuffer(sizeof(int) * indices.size(), (uint*)&indices[0], IRenderer::BUFFER_IMMUTABLE);
	model->assignIndexBuffer(ib);

	model->assignPolys(modelQuads);
}

void DiskLoader::saveCopy(const std::string& filepath, uchar* pIn, uint size)
{
}

void DiskLoader::getCopy(const std::string& filepath, uchar* pOut)
{
}

void DiskLoader::releaseCopy(const std::string& filepath)
{
}

void DiskLoader::releaseAllCopies()
{
}
