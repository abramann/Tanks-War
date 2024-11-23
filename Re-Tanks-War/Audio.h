#pragma once

//#include "SFML/Audio.hpp"
//#include "SFML/System.hpp"
#include <string>

class CAudio
{
public:

	CAudio();
	~CAudio();
	void initialize();
	int play(const std::string& sound, float volume);
	void stop(int id);
	void stopAll();
	void release();

private:

	//sf::SoundBuffer* m_pSoundBuffer;
	//std::map<std::string, sf::Sound> m_soundAssembler;
};
