// file: GameData.h
// Contains game specific constants
//
#ifndef GAMEDATA_H
#define GAMEDATA_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//=============================================================================
//                  Constants
//=============================================================================

// graphic images
const char NEBULA_IMAGE[]   = "pictures\\orion.jpg";  // photo source NASA/courtesy of nasaimages.org
const char TEXTURES_IMAGE[] = "pictures\\textures.png"; // picture of planet
const char ENGINE_POWERUP_IMAGE[] = "pictures\\engine_powerup.png"; 
const char WEAPON_POWERUP1_IMAGE[] = "pictures\\weapon_powerup1.png"; 
const char WEAPON_POWERUP2_IMAGE[] = "pictures\\weapon_powerup2.png"; 
const char LIVES_POWERUP_IMAGE[] = "pictures\\lives_powerup.png";

// window
const char CLASS_NAME[] = "Aroids";
const char GAME_TITLE[] = "Aroids";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  640;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels

// game
const float FRAME_RATE = 200.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float GRAVITY = 6.67428e-11f;             // gravitational constant

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\Game.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Game.xsb";

// audio cues
const char LASER_SHOT[] = "laser_shot";
const char ASTEROID_EXPLOSION[] = "asteroid_explosion";
const char POWER_UP[] = "power_up";
const char SHIP_EXPLOSION[] = "ship_explosion";
const char SHIP_THRUST[] = "ship_thrust";
const char TAKE_OFF[] = "take_off";

// key mappings

#endif