#include "INI.h"

CINI::CINI(const std::string& filename) : m_hFile(filename)
{
}

void CINI::setSection(const std::string& section)
{
	m_section = section;
}

void CINI::readString(const std::string & key, std::string & value) const
{
	char strBuf[128];

	GetPrivateProfileStringA(NULL, key.c_str(), nullptr, strBuf, 128, m_hFile.getFileName().c_str());

	value = strBuf;
}

void CINI::readInteger(const std::string & key, int & value) const
{
	std::string strInt;

	readString(key, strInt);

	value = std::stoi(strInt);
}

void CINI::readFloat(const std::string & key, float & value) const
{
	std::string strFloat;

	readString(key, strFloat);

	value = std::stof(strFloat);
}

void CINI::writeString(const std::string & key, const std::string & value)
{
	char strBuf[128];

	strcpy(strBuf, value.c_str());

	WritePrivateProfileStringA(NULL, key.c_str(), strBuf, m_hFile.getFileName().c_str());
}

void CINI::writeInteger(const std::string & key, const int & value)
{
	std::string strInt = std::to_string(value);

	writeString(key, strInt);
}

void CINI::writeFloat(const std::string & key, const float & value)
{
	std::string strInt = std::to_string(value);

	writeString(key, strInt);
}

void CINI::readArrayString(const std::string & key, const Ref<std::string>& pArray) const
{
}

void CINI::readArrayInteger(const std::string & key, const Ref<int>& pArray) const
{
}

void CINI::readArrayFloat(const std::string & key, const Ref<float>& pArray) const
{
}

void CINI::readArray2DInteger(const std::string& key, const Ref<int*>& pArray) const
{
}

void CINI::writeArrayString(const std::string & key, const Ref<std::string>& pArray)
{
}

void CINI::writeArrayInteger(const std::string & key, const Ref<int>& pArray)
{
}

void CINI::writeArrayFloat(const std::string & key, const Ref<float>& pArray)
{
}
