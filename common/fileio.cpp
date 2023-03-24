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

uint8_t FileIO::getDirFiles(std::string directory)
{
	uint8_t files = 0;
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
	return str.find_last_of(c);
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
				if (strcmp(&ent->d_name[find_last_of(ent->d_name, '.')], end) == !0)
					continue;
			if (!extension)
			{
				char* ext = &ent->d_name[find_last_of(ent->d_name, '.')];
				memset(ext, 0, strlen(ext));
			}

			list.push_back(ent->d_name);

		}

	return list;
}

GameInfo FileIO::readGameInfo()
{
	GameInfo gameInfo = { INVALID_DATA };
	std::ifstream file(GAME_INFO_PATH);
	std::string line;
	readValues<uint8_t>(file, { &gameInfo.fullScreen });
	readValues<uint16_t>(file, { &gameInfo.width, &gameInfo.height });
	
	return gameInfo;
}

ObjectInfo * FileIO::readObjectInfo(std::string name)
{
	ObjectInfo* pObjectInfo = new ObjectInfo;;
	std::ifstream file(OBJECT_INFO_PATH);
	std::string line;
	while (std::getline(file, line))
		if (line.compare(name) == 0)
		{

			readValues<float>(file, { &pObjectInfo->health, &pObjectInfo->speed });
			readValues<uint8_t>(file, { &pObjectInfo->deathTexture });
			return pObjectInfo;
		}

	pObjectInfo->speed = INVALID_DATA;
	return pObjectInfo;
}

ImageInfo* FileIO::readImageInfo(std::string name)
{
	ImageInfo* pImageInfo = 0;
	std::ifstream file(IMAGE_INFO_PATH);
	std::string line;
	while (std::getline(file, line))
		if (line.compare(name) == 0)
		{
			pImageInfo = new ImageInfo;
			readValues<uint16_t>(file, { &pImageInfo->width, &pImageInfo->height });
			readValues<uint8_t>(file, { &pImageInfo->columns, &pImageInfo->rows });
			readValues<bool>(file, { &pImageInfo->animate });
			readValues<float>(file, { &pImageInfo->animateSpeed, &pImageInfo->scalling });
			return pImageInfo;
		}

	const char* wq = name.c_str();
	return pImageInfo;
}

TankInfo * FileIO::readTankInfo(std::string name)
{
	TankInfo* pTankInfo = 0;
	std::ifstream file(TANK_INFO_PATH);
	std::string line;
	while (std::getline(file, line))
		if (line.compare(name) == 0)
		{
			pTankInfo = new TankInfo;
			readValues<uint8_t>(file, { &pTankInfo->fireTexture });
			return pTankInfo;
		}

	return pTankInfo;
}

FireInfo* FileIO::readFireInfo(std::string name)
{
	std::ifstream file(FIRE_DATA_PATH);
	FireInfo* pFireInfo = 0;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.compare(name) == 0)
		{
			pFireInfo = new FireInfo;
			readValues<float>(file,  {&pFireInfo->speed,&pFireInfo->damage });
			readValues<uint8_t>(file, { &pFireInfo->endTexture });
			return pFireInfo;
		}
	}

	return pFireInfo;
}

TextureInfo * FileIO::readTextureInfo(std::string name)
{
	TextureInfo* pTextureInfo = new TextureInfo;
	*pTextureInfo = { 0 };
	pTextureInfo->imageInfo = FileIO::readImageInfo(name);
	pTextureInfo->objectInfo = FileIO::readObjectInfo(name);
	pTextureInfo->tankInfo = FileIO::readTankInfo(name);
	pTextureInfo->fireInfo = FileIO::readFireInfo(name);
	return pTextureInfo;
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
	static ClientInfo clientInfo;
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
	file << "Players =" << serverInfo.players << std::endl;
	file << "Map =" << serverInfo.map;
}

ServerInfo FileIO::readServerInfo()
{
	ServerInfo serverInfo;
	std::ifstream file(SERVER_INFO_PATH);
	std::string line;
	readValues<Port>(file, { &serverInfo.port });
	readValues<uint8_t>(file, { &serverInfo.players });
	std::string s1;
	readValues(file, { &s1 });
	strcpy(serverInfo.map, s1.c_str());

	return serverInfo;
}
#endif

void FileIO::createGameInfo(const GameInfo& info)
{
	bool fullScreen;
	uint16_t width, height;
	GameInfo oInfo = readGameInfo();
	std::ofstream file(GAME_INFO_PATH);
	fullScreen = (info.fullScreen == BYTE_INVALID_DATA) ? oInfo.fullScreen : info.fullScreen;
	width = (info.width == INVALID_DATA) ? oInfo.width : info.width;
	height = (info.height == INVALID_DATA) ? oInfo.height : info.height;

	file << "fullscreen=" << fullScreen << std::endl;
	file << "width=" << std::to_string(width) << std::endl;
	file << "height=" << std::to_string(height) << std::endl;
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
	va_list arguments;
	for (auto value : values)
	{

		std::string line;
		std::getline(file, line);
		*value = getTargetEqualStringValue(line);
	}
}