#ifndef _PLAYER_H
#define _PLAYER_H
#include "always.h"

enum PLAYER_STATE
{
	PLAYER_IDLE,
	PLAYER_MOVE,
	PLAYER_NITROMOVE,
	PLAYER_ATTACK,
	PLAYER_DIE
};

template <typename T>
class Player
{

public:

	Player();
	~Player();
	void update(float frameTime);
	void render();
	/*
	void update(float frameTime);
	int8_t m_playerRank;
	uint8_t m_playerState;
	bool alive;
	bool sheild;
	uint8_t healthLine;
	uint16_t ammo;
	std::string name;
	*/
	T m_object;
	Image m_image;
};


#endif

template<typename T>
inline Player<T>::Player()
{
}

template<typename T>
inline Player<T>::~Player()
{
}

template<typename T>
inline void Player<T>::update(float frameTime)
{
	m_object.update(frameTime);
}