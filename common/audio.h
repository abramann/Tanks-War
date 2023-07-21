// audio.h
// Author: abramann.h
// Note this file is influenced by audio.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#ifndef _AUDIO_H
#define _AUDIO_H
#include "types.h"
#include "sfml\Audio.hpp"

enum Sound
{
	SOUND_BUTTON_CLICKED,
	SOUND_TANK_FORWARD,
	SOUND_TANK_BACK,
	SOUND_TANK_RIGHT,
	SOUND_TANK_LEFT,
	SOUND_TANK_DESTROY,
	SOUND_TANK_ATTACK,
	SOUND_TANK_ATTACKED,
	SOUND_BULLET_RELEASE,
	SOUND_BULLET_EXPLOSTION,
	SOUNDS
};

constexpr char* SOUND_PATH[] = {
	"Assets\\audio\\sound-button-clicked.wav",
	"Assets\\audio\\sound-tank-forward.wav",
	"Assets\\audio\\sound-tank-back.wav",
	"Assets\\audio\\sound-tank-right.wav",
	"Assets\\audio\\sound-tank-left.wav",
	"Assets\\audio\\sound-tank-destroy.wav",
	"Assets\\audio\\sound-tank-attack.wav",
	"Assets\\audio\\sound-tank-attacked.wav",
	"Assets\\audio\\sound-bullet-release.wav",
	"Assets\\audio\\sound-bullet-explosion.wav",
};

constexpr char* MUSIC_PATH[] =
{
	"Assets\\audio\\music-main-menu.wav",
	"Assets\\audio\\music-game.mp3",
	"Assets\\audio\\music-win.mp3",
	"Assets\\audio\\music-lose.mp3"
};

enum Music
{
	MUSIC_MAIN_MENU,
	MUSIC_GAME,
	MUSIC_WIN,
	MUSIC_LOSE,
	MUSICS
};

class Audio
{
public:

	Audio();
	~Audio();
	bool initialize();
	void play(Sound sound);
	void stop(Sound sound);
	void playMusic(Music music, bool looped = true);
	void stopMusic(Music music);
	void stopAll();
	void release();

private:

	sf::SoundBuffer m_soundBuffer[SOUNDS];
	sf::Sound m_sound[SOUNDS];
	sf::Music m_music[MUSICS];
};

#endif