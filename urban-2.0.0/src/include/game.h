/******************************************************************************

    URBAN
    Copyright (C) 1999  Thomas Nyberg and Jonas Borgström

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact the authors:
    Snailmail:
    Thomas Nyberg					Jonas Borgström
    Brännbergsvägen 21b					Bondevägen 6
    92136 Lycksele					92332 Storuman
    Sweden						Sweden

    or email:

    thomas.nyberg@usa.net				jonas_b@bitsmart.com
*****************************************************************************/
#ifndef __GAME_H__
#define __GAME_H__

#include "engine.h"

#ifndef __DISTRIBUTION__
#ifndef DATPATH
inline constexpr auto DATPATH = "../data";
#endif
#else
#ifndef DATPATH
inline constexpr auto DATPATH = ".";
#endif
#endif

extern int NewGame(int slot = 0);
extern void PlayDemo();
extern void PlayCredits();
extern int Do_Menu(const char *text, int num_items, int pos = 1);

extern char *level_pw[];
extern char *demo_filename;

struct SaveGameData {
	struct PlayerData dat;
	int level;
        char name[80];
};

extern struct SaveGameData SavedGames[5];
extern struct PlayerData DefaultPData;

extern uint32_t flags_;

inline constexpr auto FLAG_VERT_RETRACE	= 0x01;
inline constexpr auto FLAG_SOUND		= 0x02;
inline constexpr auto FLAG_BACKGROUND		= 0x04;
inline constexpr auto FLAG_SOUNDFX		= 0x08;
inline constexpr auto FLAG_LEVEL		= 0x10;
inline constexpr auto FLAG_NO_INTRO		= 0x20;

inline bool is_sound_on () {
    return flags_ & FLAG_SOUND;
}

inline bool is_sound_off () {
    return !is_sound_on();
}

inline bool is_vert_retrace_on () {
    return flags_ & FLAG_VERT_RETRACE;
}

inline bool is_vert_retrace_off() {
    return !is_vert_retrace_on();
}

inline bool is_background_on () {
    return flags_ & FLAG_BACKGROUND;
}

inline bool is_background_off () {
    return !is_background_on();
}

inline bool is_soundfx_on () {
    return flags_ & FLAG_SOUNDFX;
}

inline bool is_soundfx_off () {
    return !is_soundfx_on();
}

inline bool is_spec_level () {
    return flags_ & FLAG_LEVEL;
}

inline bool is_intro_on () {
    return flags_ & FLAG_NO_INTRO;
}

inline void set_sound_on () {
    flags_ |= FLAG_SOUND;
}

inline void set_sound_off () {
    flags_ &= ~FLAG_SOUND;
}

inline void set_vert_retrace_on () {
    flags_ |= FLAG_VERT_RETRACE;
}

inline void set_vert_retrace_off () {
    flags_ &= ~FLAG_VERT_RETRACE;
}

inline void set_background_on () {
    flags_ |= FLAG_SOUNDFX;
}

inline void set_background_off () {
    flags_ &= ~FLAG_SOUNDFX;
}

inline void set_soundfx_on() {
    flags_ |= FLAG_SOUNDFX;
}

inline void set_soundfx_off () {
    flags_ &= ~FLAG_SOUNDFX;
}

inline void set_intro_off () {
    flags_ &= ~FLAG_NO_INTRO;
}

inline void set_intro_on () {
    flags_ |= FLAG_NO_INTRO;
}

inline void set_spec_level_on() {
    flags_ |= FLAG_LEVEL;
}

inline void set_spec_level_off () {
    flags_ &= ~FLAG_LEVEL;
}




#endif
