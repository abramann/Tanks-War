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
	static MapData readMapInfo(std::ifstream& ifs);
	static Crc32 getCRC32(const char* file);

#ifdef _CLIENT_BUILD
	static ClientInfo readClientInfo();
	static void createClientInfo(const ClientInfo& clientInfo);
#else ifdef _SERVER_BUILD
	static ServerInfo readServerInfo();
	static void createServerInfo(const ServerInfo& serverInfo);
#endif
	static void createGameInfo(const GameInfo& info);
	
private:

	template <typename T>
	static void readValues(std::ifstream & file, std::initializer_list<T*> values);
	static void readValues(std::ifstream & file, std::initializer_list<std::string*> values);
};


#endif