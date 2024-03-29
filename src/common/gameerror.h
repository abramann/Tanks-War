// Note this code is licensed under CC BY 3.0

// Programming 2D Games www.programming2dgames.com
// Copyright (c) 2011 by:
// Charles Kelly
// gameError.h v1.2
// Error class thrown by game engine.

#pragma once

#define WIN32_LEAN_AND_MEAN

class GameError;

#include <string>
#include <exception>

namespace gameErrorNS
{
	// Error codes
	// Negative numbers are fatal errors that may require the game to be shutdown.
	// Positive numbers are warnings that do not require the game to be shutdown.
	const int FATAL_ERROR = -1;
	const int WARNING = 1;
}

// Game Error class. Thrown when an error is detected by the game engine.
// Inherits from std::exception
class GameError : public std::exception
{
private:
	int     errorCode;
	std::string message;
public:
	// default constructor
	GameError() :errorCode(gameErrorNS::FATAL_ERROR), message("Undefined Error in game.") {}
	// copy constructor
	GameError(const GameError& e) :std::exception(e), errorCode(e.errorCode), message(e.message) {}
	// constructor with args
	GameError(int code, const std::string s) :errorCode(code), message(s) {}
	// assignment operator
	GameError& operator= (const GameError& rhs)
	{
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}
	// destructor
	virtual ~GameError() {};

	// override what from base class
	virtual const char* what() const { return this->getMessage(); }

	const char* getMessage() const throw() { return message.c_str(); }
	int getErrorCode() const { return errorCode; }
};
