#include "fileio.h"
#include <iostream>
#include <fstream>
#include <dirent.h>

FileIO::FileIO(std::string sFile, uint8_t readType)
{
	m_name = sFile;
	m_readType = readType;
}

FileIO::~FileIO()
{
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

std::vector<std::string> FileIO::getFilesList(const char * directory, const char * startWith)
{
	std::vector<std::string> list;
	DIR* dir = opendir(directory);
	struct dirent* ent;
	while ((ent = readdir(dir)) != NULL)
		if (ent->d_type == DT_REG)
		{
			std::string fileStartWith = ent->d_name;
			fileStartWith.erase(strlen(startWith), fileStartWith.length());
			const char *qwe = fileStartWith.c_str();
			if (fileStartWith.compare(startWith) == 0)
				list.push_back(ent->d_name);
		}
	return list;
}


GameInfo * FileIO::readGameInfo()
{
	GameInfo* pGameInfo = 0;
	std::ifstream file(GAME_INFO_PATH);
	std::string line;
	while (std::getline(file, line))
		if (line.compare("start") == 0)
		{
			pGameInfo = new GameInfo;
			readValues<bool>(file, { &pGameInfo->fullScreen });
			readValues<uint16_t>(file, { &pGameInfo->width, &pGameInfo->height });
			return pGameInfo;
		}
	
	pGameInfo->fullScreen = INVALID_DATA;
	return pGameInfo;
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