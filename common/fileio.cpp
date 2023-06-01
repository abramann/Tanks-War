// fileio.cpp
// Author: abramann

#include "fileio.h"
#include <iostream>
#include <fstream>
#include <dirent.h>

FileIO::FileIO() 
{
}

FileIO::~FileIO()
{
}

bool FileIO::isFileExist(const char* file)
{
	std::ifstream f(file);
	return f.is_open();
}

uint8 FileIO::getDirFiles(std::string directory)
{
	uint8 files = 0;
	DIR* dir = opendir(directory.c_str());
	struct dirent *ent;
	while ((ent = readdir(dir)) != NULL)
		if (ent->d_type == DT_REG)
			files++;

	closedir(dir);
	return files;
}

size_t find_last_of(std::string str, char c)
{
	size_t result = str.find_last_of(c);
	return result;
}

std::vector<std::string> FileIO::getDirFileList(const char * directory, const char * start, const char* end , bool extension)
{
	std::vector<std::string> list;
	DIR* dir = opendir(directory);
	struct dirent* ent;
	while ((ent = readdir(dir)) != NULL)
		if (ent->d_type == DT_REG)
		{
			if (start != nullptr)
			{
				std::string startWith = ent->d_name;
				startWith.erase(strlen(start), startWith.length());
				if (startWith.compare(start) != 0)
					continue;
			}
			if (end != nullptr)
				if (strcmp(&ent->d_name[find_last_of(ent->d_name, '.')], end) != 0)
					continue;
			if (!extension)
			{
				char* ext = &ent->d_name[find_last_of(ent->d_name, '.')];
				memset(ext, 0, strlen(ext));
			}

			list.push_back(ent->d_name);
		}

	closedir(dir);
	return list;
}

GameInfo FileIO::readGameInfo()
{
	GameInfo gameInfo = { 0 };
	std::ifstream file(GAME_INFO_PATH);
	std::string line;
	readValues<int8>(file, { &gameInfo.windowed });
	readValues<int16>(file, { &gameInfo.width, &gameInfo.height });
	
	return gameInfo;
}

MapData FileIO::readMapInfo(std::ifstream& ifs)
{
	MapData mapData;
	readValues<int16>(ifs, { &mapData.width,&mapData.height });
	int8 preventedBMs = 0;
	readValues<int8>(ifs, { &mapData.bitmaps,&preventedBMs });
	for (auto i = 0; i < preventedBMs; i++)
	{
		int8 bm = 0;
		readValues<int8>(ifs, { &bm });
		mapData.preventedBM.push_back(bm);
	}

	return mapData;
}

Crc32 FileIO::getCRC32(const char* file)
{
	Crc32 result = 0;
	std::ifstream f(file, std::ios::binary | std::ios::ate);
	if (!f.is_open())
		return result;

	std::streamsize size = f.tellg();
	f.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	if (f.read(buffer.data(), size))
	{
		result = crc32_fast(&buffer[0], size);
	}

	return result;
}

#ifdef _CLIENT_BUILD
ClientInfo FileIO::readClientInfo()
{
	ClientInfo clientInfo;
	std::ifstream file(CLIENT_INFO_PATH);
	std::string line;
	std::string s1, s2;
	readValues(file, { &s1, &s2 });
	strcpy(clientInfo.name, s1.c_str());
	strcpy(clientInfo.serverIP, s2.c_str());
	readValues<Port>(file, { &clientInfo.serverPort });
	return clientInfo;
}

void FileIO::createClientInfo(const ClientInfo& clientInfo)
{
	std::ofstream file(CLIENT_INFO_PATH);
	file << "PlayerName =" << clientInfo.name << std::endl;
	file << "ServerIP =" << clientInfo.serverIP << std::endl;
	file << "Port =" << clientInfo.serverPort;
}

#else #ifdef _SERVER_BUILD

void FileIO::createServerInfo(const ServerInfo& serverInfo)
{
	std::ofstream file(SERVER_INFO_PATH);
	file << "Port =" << serverInfo.port << std::endl;
	file << "Players =" << std::to_string(serverInfo.players) << std::endl;
}

ServerInfo FileIO::readServerInfo()
{
	ServerInfo serverInfo;
	std::ifstream file(SERVER_INFO_PATH);
	std::string line;
	readValues<Port>(file, { &serverInfo.port });
	readValues<uint8>(file, { &serverInfo.players });

	return serverInfo;
}
#endif

void FileIO::createGameInfo(const GameInfo& info)
{
	int16 width, height;
	GameInfo oInfo = readGameInfo();
	std::ofstream file(GAME_INFO_PATH);
	int8 windowed = (info.windowed == -1) ? oInfo.windowed : info.windowed; // check if windowed requires to change in case change width and height only
	width =  (info.width == -1) ? oInfo.width : info.width; // check if width and height require to change in case change windowed only
	height = (info.height == -1) ? oInfo.height : info.height;
	file << "windowed=" << windowed << std::endl;
	file << "width=" << std::to_string(width) << std::endl;
	file << "height=" << std::to_string(height) << std::endl;
}

inline std::string getTargetEqualStringValue(std::string str)
{
	str.erase(0, str.find('=') + 1);
	return str;
}

template<typename T>
void FileIO::readValues(std::ifstream & file, std::initializer_list<T*> values)
{
	for (auto value : values)
	{
		std::string line;
		std::getline(file, line);
		*value = ::atof(getTargetEqualStringValue(line).c_str());
	}
}

void FileIO::readValues(std::ifstream & file, std::initializer_list<std::string*> values)
{
	for (auto value : values)
	{
		std::string line;
		std::getline(file, line);
		*value = getTargetEqualStringValue(line);
	}
}