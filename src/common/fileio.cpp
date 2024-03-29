// fileio.cpp
// Author: abramann

#include "fileio.h"
#include <iostream>
#include <fstream>
#include <dirent.h>

#pragma warning(disable : 4800)	// no warn to convert double to bool when read file by std::atof

FileIO::FileIO()
{
}

FileIO::~FileIO()
{
}

bool FileIO::isFileExist(const std::string& file)
{
	std::ifstream f(file);
	bool exist = f.is_open();
	return exist;
}

uint8 FileIO::getDirFiles(const std::string& directory)
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
	return str.find_last_of(c);
}

std::vector<std::string> FileIO::getDirFileList(const char * directory, const char * start, const char* end, bool extension)
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

GameSettings FileIO::readGameSettings()
{
	GameSettings gameSettings = { 0 };
	std::ifstream file(fileNS::GAME_INFO_PATH);
	if (!file.is_open())
		createGameSettings();

	std::string line;
	readValues<uint32>(file, { &gameSettings.width, &gameSettings.height,
		&gameSettings.aiLevel, &gameSettings.aiCount });
	readValues<bool>(file,
	{
		&gameSettings.windowed, &gameSettings.vsync ,
		&gameSettings.audio, &gameSettings.computeShader,
		&gameSettings.debugMode
	}
	);
	return gameSettings;
}

MapInfo FileIO::readMapInfo(std::ifstream& ifs)
{
	MapInfo mapInfo;
	readValues<int16>(ifs, { &mapInfo.width,&mapInfo.height });
	readValues<int8>(ifs, { &mapInfo.bitmaps });
	for (int8 i = 0; i < mapInfo.bitmaps; i++)
	{
		BitmapAttribute bmAttribute = { 0 };
		int8 attribute = 0;
		readValues<int8>(ifs, { &attribute });
		if (attribute)
		{
			readValues<bool>(ifs, { &bmAttribute.obstructed });
			readValues<float>(ifs, { &bmAttribute.damageFactor,&bmAttribute.velocityFactor });
		}
		else
		{
			bmAttribute.damageFactor = logicNS::DEFAULT_BITMAP_DAMAGEFACTOR;
			bmAttribute.velocityFactor = logicNS::DEFAULT_BITMAP_VELOCITYFACTOR;
		}

		mapInfo.bitmapAttribute.push_back(bmAttribute);
	}

	return mapInfo;
}

Crc32 FileIO::getCRC32(const std::string& file)
{
	Crc32 result = 0;
	std::ifstream f(file, std::ios::binary | std::ios::ate);
	if (!f.is_open())
		return result;

	size_t size = static_cast<size_t>(f.tellg());
	f.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	if (f.read(buffer.data(), size))
		result = crc32_fast(&buffer[0], size);

	return result;
}

#ifdef _CLIENT_BUILD
ClientInfo FileIO::readClientInfo()
{
	ClientInfo clientInfo;
	std::ifstream file(fileNS::CLIENT_INFO_PATH);
	std::string line;
	std::string s1, s2;
	readValues(file, { &s1, &s2 });
	strcpy(clientInfo.name, s1.c_str());
	strcpy(clientInfo.serverIP, s2.c_str());
	readValues<Port>(file, { &clientInfo.serverPort });
	return clientInfo;
}

void FileIO::createClientInfo(const ClientInfo* pClientInfo)
{
	std::ofstream file(fileNS::CLIENT_INFO_PATH);
	file << "PlayerName =" << pClientInfo->name << '\n'
		<< "ServerIP =" << pClientInfo->serverIP << '\n'
		<< "Port =" << pClientInfo->serverPort << std::endl;
}

#else #ifdef _SERVER_BUILD
void FileIO::createServerInfo(const ServerInfo& serverInfo)
{
	std::ofstream file(fileNS::SERVER_INFO_PATH);
	file << "Port =" << serverInfo.port << std::endl;
	file << "Max Clients =" << std::to_string(serverInfo.maxClients) << std::endl;
}

ServerInfo FileIO::readServerInfo()
{
	ServerInfo serverInfo;
	std::ifstream file(fileNS::SERVER_INFO_PATH);
	std::string line;
	readValues<Port>(file, { &serverInfo.port });
	readValues<int32>(file, { &serverInfo.maxClients });
	return serverInfo;
}
#endif

void FileIO::createGameSettings(const GameSettings* info)
{
	uint32 width, height,
		aiLevel, aiCount;
	bool windowed, vsync,
		audio, computeShader,
		debugMode;
	if (info)
	{
		width = info->width;
		height = info->height;
		windowed = info->windowed;
		vsync = info->vsync;
		audio = info->audio;
		computeShader = info->computeShader;
		aiLevel = info->aiLevel;
		debugMode = info->debugMode;
		aiCount = info->aiCount;
	}
	else
	{
		width = gameNS::MIN_RESOLUTION_WIDTH;
		height = gameNS::MIN_RESOLUTION_HEIGHT;
		windowed = gameNS::WINDOWED_DEFAULT;
		vsync = gameNS::VSYNC_DEFAULT;
		audio = gameNS::AUDIO_DEFAULT;
		computeShader = gameNS::COMPUTESHADER_DEFAULT;
		debugMode = gameNS::COMPUTESHADER_DEFAULT;
		aiLevel = gameNS::AILEVEL_DEFAULT;
		aiCount = 0;
	}

	std::ofstream file(fileNS::GAME_INFO_PATH);
	file << "Width=" << std::to_string(width) << '\n'
		<< "Height=" << std::to_string(height) << '\n'
		<< "AI Level=" << std::to_string(aiLevel) << '\n'
		<< "AI Count=" << std::to_string(aiCount) << '\n'
		<< "Windowed=" << std::to_string(windowed) << '\n'
		<< "VSync=" << std::to_string(vsync) << '\n'
		<< "Audio=" << std::to_string(audio) << '\n'
		<< "Compute Shader=" << std::to_string(computeShader) << '\n'
		<< "Debug Mode=" << std::to_string(debugMode) << std::endl;
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
		*value = static_cast<T>(atof(getTargetEqualStringValue(line).c_str()));
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