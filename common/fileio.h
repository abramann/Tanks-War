#ifndef _FILEIO_H
#define _FILE_IO_H
#include "constants.h"

class FileIO
{
public:

	FileIO(std::string sFile, uint8_t readType);
	~FileIO();
	static uint8_t getDirFiles(std::string dir);
	static std::vector<std::string> getFilesList(const char* dir, const char* startWith);
	static 	GameInfo readGameInfo();
	static 	ObjectInfo* readObjectInfo(std::string name);
	static ImageInfo* readImageInfo(std::string name);
	static TankInfo* readTankInfo(std::string name);
	static FireInfo* readFireInfo(std::string name);
	static TextureInfo* readTextureInfo(std::string name);

private:

	template <typename T>
	static void readValues(std::ifstream & file, std::initializer_list<T*> values);
	static void readValues(std::ifstream & file, std::initializer_list<std::string*> values);
	std::string m_name;
	uint8_t m_readType;
	
};


#endif