#include "DiskLoader.h"

// Only for releasing copies with dtor, won't be used anywhere
static diskLoader dummy;

diskLoader::~diskLoader()
{
	releaseAllCopies();
}

void diskLoader::loadBinary(const std::string& filepath, uchar* pInBuf, bool savecopy)
{
}

void diskLoader::loadMesh(const std::string& filepath, CMesh* pMesh)
{
}

void diskLoader::saveCopy(const std::string& filepath, uchar* pIn, uint size)
{
}

void diskLoader::getCopy(const std::string& filepath, uchar* pOut)
{
}

void diskLoader::releaseCopy(const std::string& filepath)
{
}

void diskLoader::releaseAllCopies()
{
}
