#ifndef _FILEIO_H
#define _FILE_IO_H
#include "constants.h"

class FileIO
{
public:

	FileIO();
	~FileIO();
	static bool isFileExist(const char* file);
	static uint8_t getDirFiles(std::string dir);
	static std::vector<std::string> getDirFileList(const char* dir, const char* start = 0, const char* end = 0, bool extension = true);
	static 	GameInfo readGameInfo();
	static 	ObjectInfo* readObjectInfo(std::string name);
	static ImageInfo* readImageInfo(std::string name);
	static TankInfo* readTankInfo(std::string name);
	static FireInfo* readFireInfo(std::string name);
	static TextureInfo* readTextureInfo(std::string name);
	static ClientInfo readClientInfo();
	static ServerInfo readServerInfo();
	static void createClientInfo(const ClientInfo& clientInfo);
	static void createGameInfo(const GameInfo& info);
	static void createServerInfo(const ServerInfo& serverInfo);
	
private:

	template <typename T>
	static void readValues(std::ifstream & file, std::initializer_list<T*> values);
	static void readValues(std::ifstream & file, std::initializer_list<std::string*> values);
};


#endif