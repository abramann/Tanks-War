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

const char* MUSIC_PATH[] = {
	"Assets\\audio\\music-main-menu.wav",
	"Assets\\audio\\music-game.mp3",
	"Assets\\audio\\music-win.mp3",
	"Assets\\audio\\music-lose.mp3",
};

const char* SOUND_PATH[] = {
	"Assets\\audio\\sound-button-clicked.wav",
	"Assets\\audio\\sound-tank-forward.wav",
	"Assets\\audio\\sound-tank-back.wav",
	"Assets\\audio\\sound-tank-right.wav",
	"Assets\\audio\\sound-tank-left.wav",
	"Assets\\audio\\sound-tank-death.wav"
	"Assets\\audio\\sound-tank-attack.wav",
	"Assets\\audio\\sound-tank-attacked.wav",
	"Assets\\audio\\sound-fire-release.wav"
	"Assets\\audio\\sound-fire-hited.wav",
};

bool Audio::initialize()
{
	m_pSound = createIrrKlangDevice();
	if (m_pSound == 0)
		return false;

	for (int i = 0; i < MUSICS; i++)
		m_pSourceMusic[i] = m_pSound->addSoundSourceFromFile(MUSIC_PATH[i]);
	for (int i = 0; i < SOUNDS; i++)
		m_pSource[i] = m_pSound->addSoundSourceFromFile(SOUND_PATH[i]);
	
	return true;
}

void Audio::play(Sound sound)
{
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
	m_pSound->drop();
}
