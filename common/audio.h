// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// audio.h v1.0
#ifndef _AUDIO_H                // Prevent multiple definitions if this 
#define _AUDIO_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#include "directx\include\xact3.h"
#include "constants.h"

typedef std::string Effect;

constexpr auto WAVE_BANK = "Assets//Audio//Win//Wave Bank.xwb";
constexpr auto SOUND_BANK = "Assets//Audio//Win//Sound Bank.xsb";



class Audio
{
	// properties
private:
	IXACT3Engine* xactEngine;   // pointer to XACT sound engine
	IXACT3WaveBank* waveBank;   // pointer to XACT wave bank
	IXACT3SoundBank* soundBank; // pointer to XACT sound bank
	XACTINDEX cueI;             // XACT sound index
	void* mapWaveBank;          // call UnmapViewOfFile() to release file
	void* soundBankData;
	bool coInitialized;         // set true if coInitialize is successful

public:
	// Constructor
	Audio();

	// Destructor
	virtual ~Audio();

	// member functions

	// Initialize Audio
	bool initialize();

	// Perform periodic sound engine tasks.
	void run();

	// Play sound specified by cue from sound bank.
	// If cue does not exist no error occurs, there is simply no sound played.
	void playCue(Effect cue);

	// Stop a playing sound specified by cue from sound bank.
	// If cue does not exist no error occurs.
	void stopCue(Effect cue);
};

#endif
