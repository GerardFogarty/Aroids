// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Game Engine constants.h v1.5
// Last modification: Feb-17-2013

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this
#define _CONSTANTS_H            // file is included in more than one place

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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

//=============================================================================
//                  Constants
//=============================================================================

// window
extern const char CLASS_NAME[];
extern const char GAME_TITLE[];

extern const bool FULLSCREEN;                  // windowed or fullscreen
extern const UINT GAME_WIDTH;                  // width of game in pixels
extern const UINT GAME_HEIGHT;                 // height of game in pixels

// game
const double PI = 3.14159265;
extern const float FRAME_RATE;          // the target frame rate (frames/sec)
extern const float MIN_FRAME_RATE;      // the minimum frame rate
extern const float MIN_FRAME_TIME;      // minimum desired time for 1 frame
extern const float MAX_FRAME_TIME;      // maximum time used in calculations

// graphic images

// audio files required by audio.cpp

// WAVE_BANK must be location of .xwb file.
extern const char WAVE_BANK[];

// SOUND_BANK must be location of .xsb file.
extern const char SOUND_BANK[];

// audio cues

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY  = '`';         // ` key
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key

#endif
