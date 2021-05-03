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
#define DATPATH "../data"
#endif
#else
#ifndef DATPATH
#define DATPATH "."
#endif
#endif

extern int NewGame(int slot = 0);
extern void PlayDemo();
extern void PlayCredits();
extern int Do_Menu(char *text, int num_items, int pos = 1);

extern char *level_pw[];
extern char *demo_filename;

struct SaveGameData {
	struct PlayerData dat;
	int level;
        char name[80];
};

extern struct SaveGameData SavedGames[5];
extern struct PlayerData DefaultPData;

extern unsigned long _flags;

#define FLAG_VERT_RETRACE	0x01
#define FLAG_SOUND		0x02
#define FLAG_BACKGROUND		0x04
#define FLAG_SOUNDFX		0x08
#define FLAG_LEVEL		0x10
#define FLAG_NO_INTRO		0x20

#define IS_SOUND_ON (_flags & FLAG_SOUND)
#define IS_SOUND_OFF (!IS_SOUND_ON)
#define IS_VERT_RETRACE_ON (_flags & FLAG_VERT_RETRACE)
#define IS_VERT_RETRACE_OFF (!IS_VERT_RETRACE_ON)
#define IS_BACKGROUND_ON (_flags & FLAG_BACKGROUND)
#define IS_BACKGROUND_OFF (!IS_BACKGROUND_ON)
#define IS_SOUNDFX_ON (_flags & FLAG_SOUNDFX)
#define IS_SOUNDFX_OFF (!IS_SOUNDFX_ON)
#define IS_SPEC_LEVEL (_flags & FLAG_LEVEL)
#define IS_INTRO_ON (_flags & FLAG_NO_INTRO)

#define _SET_SOUND_ON (_flags |= FLAG_SOUND)
#define _SET_SOUND_OFF (_flags &= ~FLAG_SOUND)
#define _SET_VERT_RETRACE_ON (_flags |= FLAG_VERT_RETRACE)
#define _SET_VERT_RETRACE_OFF (_flags &= ~FLAG_VERT_RETRACE)
#define _SET_BACKGROUND_ON (_flags |= FLAG_SOUNDFX)
#define _SET_BACKGROUND_OFF (_flags &= ~FLAG_SOUNDFX)
#define _SET_SOUNDFX_ON (_flags |= FLAG_SOUNDFX)
#define _SET_SOUNDFX_OFF (_flags &= ~FLAG_SOUNDFX)
#define _SET_INTRO_OFF (_flags &= ~FLAG_NO_INTRO)
#define _SET_INTRO_ON (_flags |= FLAG_NO_INTRO)
#define _SET_SPEC_LEVEL_ON (_flags |= FLAG_LEVEL)
#define _SET_SPEC_LEVEL_OFF (_flags &= ~FLAG_LEVEL)



#endif
