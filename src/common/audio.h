// audio.h
// Author: abramann.h
// Note this file is influenced by audio.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0

#pragma once

#include "types.h"
#include "sfml\Audio.hpp"
#include <memory>

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

class Audio
{
public:

	Audio();
	~Audio();
	void initialize();
	void play(std::string sound);
	void stop(std::string  sound);
	void stopAll();
	void release();
	bool isAudioPlaying() const { return m_audioPlay; }

private:

	bool m_audioPlay;
	sf::SoundBuffer* m_pSoundBuffer;
	std::map<std::string, sf::Sound> m_soundAssembler;
};