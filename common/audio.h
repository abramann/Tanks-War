#pragma once
#include "constants.h"
#include "irrklang\irrKlang.h"

enum Sound
{
	SOUND_BUTTON_CLICKED,
	SOUND_TANK_FORWARD,
	SOUND_TANK_BACK,
	SOUND_TANK_RIGHT,
	SOUND_TANK_LEFT,
	SOUND_TANK_DEATH,
	SOUND_TANK_ATTACK,
	SOUND_TANK_ATTACKED,
	SOUND_FIRE_RELEASE,
	SOUND_FIRE_EXPLOSTION,
	SOUNDS
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
	void playCue(int i) {};

private:

	irrklang::ISoundEngine* m_pSound;
	irrklang::ISoundSource* m_pSource[SOUNDS];
	irrklang::ISoundSource* m_pSourceMusic[MUSICS];


};

