// Author: Wefaq
// Brief: diskLoader: load files from disk and keep copy if needed.
#pragma once

#include "Types.h"
#include "StrUtility.h"
#include "Model.h"

class IModel;

// It's supposed to be namespace but we need dtor to release the saved copies safely.
class DiskLoader
{
	// For models
	static std::vector<std::pair<IBuffer*, IBuffer*>> s_pBuffers;
	static std::vector<std::vector<Poly>> s_polys;
public:
	~DiskLoader();

	static void loadBinary(const std::string& filepath, uchar* pInBuf, bool savecopy);
	static void parseModel(IModel* pParsedModel, const std::string& filepath);
	static void saveCopy(const std::string& filepath, uchar* pIn, uint size);
	static void getCopy(const std::string& filepath, uchar* pOut);
	static void releaseCopy(const std::string& filepath);
	static void releaseAllCopies();
};

