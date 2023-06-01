// audio.cpp
// Author: abramann
// Note this file is influnced by audio.cpp from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0 

#include "audio.h"
#include "fileio.h"

using namespace irrklang;

Audio::Audio() : m_pSound(NULL)
{
}


Audio::~Audio()
{
	release();
}

bool Audio::initialize()
{
	m_pSound = createIrrKlangDevice();
	if (m_pSound == 0)
		return false;

	/*
	for (int i = 0; i < MUSICS; i++)
		m_pSourceMusic[i] = m_pSound->addSoundSourceFromFile(MUSIC_PATH[i]);
	for (int i = 0; i < SOUNDS; i++)
		m_pSource[i] = m_pSound->addSoundSourceFromFile(SOUND_PATH[i]);
	*/	
	return true;
}

void Audio::play(Sound sound)
{
	return;
	m_pSound->play2D(m_pSource[sound]);
}

void Audio::stop(Sound sound)
{
}

void Audio::playMusic(Music music, bool looped)
{
	return;
	if (m_pSound->isCurrentlyPlaying(m_pSourceMusic[music]))
		return;

	m_pSound->play2D(m_pSourceMusic[music], looped);
}

void Audio::stopMusic(Music music)
{
}

void Audio::stopAll()
{
}

void Audio::release()
{
	if (m_pSound)
		m_pSound->drop();
}
