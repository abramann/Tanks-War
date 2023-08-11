// audio.cpp
// Author: abramann
// Note this file is influenced by audio.cpp from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#include "audio.h"
#include "fileio.h"

Audio::Audio()
{
}

Audio::~Audio()
{
	stopAll();
}

bool Audio::initialize()
{
	for (int i = 0; i < SOUNDS; i++)
	{
		bool loaded = m_soundBuffer[i].loadFromFile(SOUND_PATH[i]);
		m_sound[i].setBuffer(m_soundBuffer[i]);
		//	if (!loaded)
		//	return false;
	}
	for (int i = 0; i < MUSICS; i++)
	{
		bool opened = m_music[i].openFromFile(MUSIC_PATH[i]);
		//	if (!opened)
		//	return false;
	}

	return true;
}

void Audio::play(Sound sound)
{
	m_sound[sound].play();
}

void Audio::stop(Sound sound)
{
	m_sound[sound].stop();
}

void Audio::playMusic(Music music, bool looped)
{
	if (m_music->getStatus() == sf::SoundSource::Status::Playing)
		return;

	m_music[music].setLoop(looped);
	m_music[music].play();
}

void Audio::stopMusic(Music music)
{
	m_music[music].stop();
}

void Audio::stopAll()
{
	for (int i = 0; i < MUSICS; i++)
		m_music[i].stop();
	for (int i = 0; i < SOUNDS; i++)
		m_sound[i].stop();
}