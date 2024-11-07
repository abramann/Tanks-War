// Author: Wefaq
// Brief: diskLoader: load files from disk and keep copy if needed.
#pragma once

#include "Types.h"
#include "String.h"

class CMesh;

// It's supposed to be namespace but we need dtor to release the saved copies safely.
class diskLoader
{
public:
	~diskLoader();

	static void loadBinary(const std::string& filepath, uchar* pInBuf, bool savecopy);
	static void loadMesh(const std::string& filepath, CMesh* pMesh);

	static void saveCopy(const std::string& filepath, uchar* pIn, uint size);
	static void getCopy(const std::string& filepath, uchar* pOut);
	static void releaseCopy(const std::string& filepath);
	static void releaseAllCopies();
};

