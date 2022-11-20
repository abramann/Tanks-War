// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine constants.h v3.1
// Last modification: Dec-24-2013

#pragma once
#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "gameError.h"
#include <windows.h>

//=============================================================================
//                  Constants
//=============================================================================


const UINT GAME_WIDTH = 1024;               // width of game in pixels
const UINT GAME_HEIGHT = 768;               // height of game in pixels
const int TEXTURE_SIZE = 32;
const int SCREEN_WIDTH = GAME_WIDTH / TEXTURE_SIZE + 2;
const int SCREEN_HEIGHT = GAME_HEIGHT / TEXTURE_SIZE + 2;

// game
const double PI = 3.14159265;
const float MIN_FRAME_TIME = 10.0f;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 80.0f;

//Chat
const int chatSize = 8;
const int chatFontSize = 12;
const int chatX = 605;
const int chatY = 640;

//Tile ID's
const int backgroundTile = 64;
const int backgroundTiles[] = { 64, 65, 66, 67, 68, 69, 70, 71 };
const int backgroundTilesCount = 8;
const int chestTile = 2;
const int floorTile[] = { 32, 33, 34, 35, 51 /*43*/, /*44*/51, 45, 46, 51, 52, 32/*53*/, 45/*54*/ };
const int doubleWallHorizontal = 42;
const int doubleWallVertical = 50;
const int topWall = 37;
const int bottomWall = 36;
const int leftWall = 56;
const int rightWall = 57;
const int leftTopCorner = 40;
const int leftBottomCorner = 48;
const int rightBottomCorner = 49;
const int rightTopCorner = 41;
const int horizontalDoorLeft = 13;
const int horizontalDoorRight = 5;
const int verticalDoorTop = 4;
const int verticalDoorBottom = 12;
const int cornerSouthWest = 21;
const int cornerSouthEast = 20;
const int cornerNorthEast = 28;
const int cornerNorthWest = 29;
const int stairsDown[] = { 14, 15 };
const int stairsUp[] = { 6, 7 };
const int fogTile = backgroundTile;

const int ITEMS_SPAWNED_PER_FLOOR = 12;

const int LEVEL_WIDTH = 400;
const int LEVEL_HEIGHT = 400;
const int LEVEL_LAYERS = 8;

const int OBSTRUCTION_LAYER = 5;				//This layer should contain any artifacts that obstruct player movement
const int COLLISION_LAYER = 6;					//Anything on this layer will fire a collision event
const int ABOVE_PLAYER_LAYER = LEVEL_LAYERS - 1;	//This layer contains anything that the player can walk under


const int HALLWAY_MAX_LENGTH = 7;
const int HALLWAY_MIN_LENGTH = 3;
const int HALLWAY_DIFF_LENGTH = HALLWAY_MAX_LENGTH - HALLWAY_MIN_LENGTH;

const int ROOM_MAX_SIZE = 15;
const int ROOM_MIN_SIZE = 6;
const int ROOM_DIFF_SIZE = ROOM_MAX_SIZE - ROOM_MIN_SIZE;

const int CHANCE_TO_GEN_ROOM = 6;
const int CHANCE_LESS_THAN = 4;
const int ATTEMPTS_BEFORE_FAIL = 10;			//Attemps to generate randomly sized rooms before failing to place a room on that side
const int MOB_SIGHT_RADIUS = 5;
const int MAX_ENTITY_MONSTER = 100;
const int MAX_ITEMS = 100;
const int PLAYER_MAX_ITEMS = 27;

const int DIR_LEFT = 0;
const int DIR_UP = 1;
const int DIR_RIGHT = 2;
const int DIR_DOWN = 3;

const int RANDOM_WEIGHTED_ITEMS[] = { 0, 0 };



const int FOG_VISIBLITY = 7;				 //Radius around the player that fog is revealed




const int TEXTURE_SHEET_COLS = 8;



// audio files required by audio.cpp

// audio cues
const char MUSIC[] = "BackgroundMusic";
const char MvP[] = "MonsterHitPlayer";
const char PvM[] = "PlayerHitMonster";
const char eatFood[] = "EatFood";
const char armorblock[] = "ArmorBlock";
const char collectMoney[] = "CollectMoney";
const char footsteps[] = "footsteps";
const char mapReveal[] = "mapReveal";
const char potionUse[] = "potionUse";
const char teleport[] = "teleport";
const char pixkaxe[] = "pickaxe";
const char levelUp[] = "levelUp";
const char bookUse[] = "bookUse";
const char itemPickup[] = "itemPickup";

enum ITEM_SLOT { WEAPON, ARMOR, RING, NORMAL };

// audio cues

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY = '`';         // ` key
const UCHAR ESC_KEY = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY = VK_MENU;     // Alt key
const UCHAR ENTER_KEY = VK_RETURN;   // Enter key
const UCHAR LEFT_KEY = VK_LEFT;
const UCHAR RIGHT_KEY = VK_RIGHT;
const UCHAR UP_KEY = VK_UP;
const UCHAR DOWN_KEY = VK_DOWN;
const UCHAR LEFT_KEY2 = 0x41;
const UCHAR RIGHT_KEY2 = 0x44;
const UCHAR UP_KEY2 = 0x57;
const UCHAR DOWN_KEY2 = 0x53;


//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
	if (ptr)
		ptr->onLostDevice();
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
	if (ptr)
		ptr->onResetDevice();
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

#endif
