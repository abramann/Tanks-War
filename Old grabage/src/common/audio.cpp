// audio.cpp
// Author: abramann
// Note this file is influenced by audio.cpp from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#include "audio.h"
#include "fileio.h"
#include "inlined.inl"

using namespace sf;

Audio::Audio() : m_pSoundBuffer(0)
{
}

Audio::~Audio()
{
	safeDeleteArray(m_pSoundBuffer);
}

void Audio::initialize()
{
	auto soundFiles = FileIO::getDirFileList(fileNS::AUDIO_DIR, 0, ".wav", false);
	m_pSoundBuffer = new SoundBuffer[soundFiles.size()];
	int8 notLoaded = 0;
	for (size_t i = 0; i < soundFiles.size(); i++)
	{
		std::string soundFile = soundFiles[i];
		std::string sFile = strFormat("%s%s%s", fileNS::AUDIO_DIR, soundFile.c_str(), ".wav");
		bool loaded = m_pSoundBuffer[i].loadFromFile(sFile);
		m_soundAssembler[soundFile].setBuffer(m_pSoundBuffer[i]);
		if (!loaded)
		{
			IMPL_WARNING(strFormat("Failed to load %s!", sFile.c_str()));
			notLoaded++;
		}
	}

	CHECK_WARNING(notLoaded == 0, strFormat("%d files failed to load", notLoaded))
}

void Audio::play(const std::string& sound)
{
	if (g_pGameSettings->audio)
		m_soundAssembler[sound].play();
}

void Audio::stop(const std::string& sound)
{
	m_soundAssembler[sound].stop();
}

void Audio::stopAll()
{
}