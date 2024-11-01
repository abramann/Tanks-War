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
	void play(const std::string& sound);
	void stop(const std::string& sound);
	void stopAll();
	void release();

private:

	//sf::SoundBuffer* m_pSoundBuffer;
	//std::map<std::string, sf::Sound> m_soundAssembler;
};
