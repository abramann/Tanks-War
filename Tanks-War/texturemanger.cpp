#include "texturemanger.h"
#include <fstream>
#include <initializer_list>

TextureManger::TextureManger() : m_width(0), m_height(0), m_lpTexture(NULL)
{
}

TextureManger::~TextureManger()
{
	release();
}

bool TextureManger::initialize(const char* file, Graphics* graphics)
{
	m_pGraphics = graphics;
	m_file = file;
	read();
	return m_pGraphics->loadTexture(m_file, m_width, m_height, m_textureInfo.transparency, m_lpTexture);
}

void TextureManger::onLostDevice()
{
	release();
}

void TextureManger::onResetDevice()
{
	m_pGraphics->loadTexture(m_file, m_width, m_height, m_textureInfo.transparency, m_lpTexture);
}

void TextureManger::release()
{
	SAFE_RELEASE(m_lpTexture);
}

template <typename T>
void ReadLineToNumeric(std::ifstream &file, std::initializer_list<T>)
{
	for (auto value : T)
	{
		std::string line;
		std::getline(file, line);
		value = getTargetEqualStringValue(line);
	}
}

void TextureManger::read()
{
	std::string file = m_file;
	std::string name = &file[file.find_last_of('\\') + 1];
	char* ad = &name[0];
	file.erase(file.find_last_of('\\') + 1, file.length());
	file.insert(file.length(), "textures-info.txt");
	char* tt = &file[0];
	std::ifstream fInfo(file);
	if (!fInfo.is_open())
		return;

	std::string line;
	while (std::getline(fInfo, line))
		if (line.compare(name) == 0)
		{
			std::getline(fInfo, line);
			m_textureInfo.rows = std::stoi(getTargetEqualStringValue(line));
			std::getline(fInfo, line);
			m_textureInfo.columns = std::stoi(getTargetEqualStringValue(line));
			std::getline(fInfo, line);
			m_textureInfo.width = std::stoi(getTargetEqualStringValue(line));
			std::getline(fInfo, line);
			m_textureInfo.height = std::stoi(getTargetEqualStringValue(line));
			std::getline(fInfo, line);
			m_textureInfo.transparency = std::stoi(getTargetEqualStringValue(line));
			std::getline(fInfo, line);
			m_textureInfo.animateSpeed = ::atof(getTargetEqualStringValue(line).c_str());
			return;
		}

	m_textureInfo.transparency =  D3DCOLOR_ARGB(255, 255, 0, 255);
}
