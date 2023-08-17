// fileio.h
// Author: abramann

#pragma once

#include "types.h"
#include <memory>
#include <string>

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
	static Crc32 getCRC32(const std::string& file);
	static char* loadInMemory(const std::string name, uint32& size);

#ifdef _CLIENT_BUILD
	static ClientInfo readClientInfo();
	static void createClientInfo(const ClientInfo* pClientInfo);
#else ifdef _SERVER_BUILD
	static ServerInfo readServerInfo();
	static void createServerInfo(const ServerInfo& serverInfo);
#endif
	static void createGameInfo(const GameInfo* info = nullptr);

private:

	template <typename T>
	static void readValues(std::ifstream & file, std::initializer_list<T*> values);
	static void readValues(std::ifstream & file, std::initializer_list<std::string*> values);
};