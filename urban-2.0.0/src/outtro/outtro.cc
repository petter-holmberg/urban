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
#include "outtro.h"
#include "datfile.h"
#include "engine.h"
#include "game.h"
#include "icache.h"
#include "object.h"
#include "urbfont.h"
#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/**************************************************************************/
#define FRAME_DELAY 100
/**************************************************************************/
static UrbanFont* lg;
static int quit;
static char flctext[256];
/**************************************************************************/
Outtro::Outtro()
{
    lg = new UrbanFont(SMALL_FONT);
}
/**************************************************************************/
Outtro::~Outtro()
{
    delete lg;
}
/**************************************************************************/
extern "C" {
static int callback()
{
    static unsigned int count = 0;
    unsigned int num_frames = strlen(flctext);
    char temptext[200];

    if (count < strlen(flctext)) {

        strncpy(temptext, flctext, count + 1);
        temptext[count + 1] = 0;

    } else {

        strcpy(temptext, flctext);
    }

    lg->print(temptext, 20, 170, screen);

    rest(FRAME_DELAY);

    /*                if(keypressed())
                	quit = 1;*/

    if ((count == (num_frames - 1)) || quit) {

        count = 0;

        return 1;
    }

    count++;
    return 0;
}
}
/**************************************************************************/
#define PLAY_FLC(x, y)                              \
    strcpy(flctext, y);                             \
    if ((buf = dat.load_file_to_memory(x)) == NULL) \
        exit(1);                                    \
    play_memory_fli(buf, screen, 1, callback);      \
    delete[] buf;                                   \
    if (quit) {                                     \
        QUIT;                                       \
    }

#define ANIM_TEXT(x)                             \
    strcpy(text, x);                             \
                                                 \
    clear_keybuf();                              \
                                                 \
    for (i = 0; i < (signed)strlen(text); i++) { \
                                                 \
        strncpy(temptext, text, i + 1);          \
        temptext[i + 1] = 0;                     \
        lg->print(temptext, 50, 170, screen);    \
                                                 \
        rest(FRAME_DELAY);                       \
    }

/*		if(keypressed()) { \
\
                	QUIT; \
		} \
\ */

#define DISPLAY_IMAGE(x)                     \
    bmp = icache.GetImage(x, palette);       \
    set_palette(palette);                    \
    blit(bmp, screen, 0, 0, 0, 0, 320, 240); \
    icache.FreeImage(bmp);

#define QUIT        \
    clear_keybuf(); \
    return;

#define PLAY_MOD(x)                      \
    {                                    \
        char* buf = new char[1024];      \
                                         \
        sprintf(buf, x, DATPATH);        \
        ENGINE.sound.PlayMusic(buf);     \
                                         \
        delete[] buf;                    \
        ENGINE.sound.SetMusicVolume(64); \
    }

//#define PLAY_MOD(x)
/**************************************************************************/
void Outtro::RunOuttro()
{
    datfile dat("intro.dat");
    char* buf = NULL;
    quit = 0;
    PALETTE palette;
    /*        int i;
        BITMAP *bmp;
        char text[128];
	char temptext[128];*/
    BITMAP* bmp;

    clear_keybuf();

    PLAY_MOD("%s/snd/modules/rock.xm");

    //        get_palette(pal);

    DISPLAY_IMAGE("intro/atom.pcx");

    //        fade_in(pal, 5);

    rest(6000);

    fade_out(5);

    PLAY_FLC("gfx/intro/12.flc", "A zhame we had to nuke\nzee complex to destroy him...   ");

    PLAY_FLC("gfx/intro/9.flc", "Well, at least we \nmanaged to stop him.   ");

    //        rest(1000);                      8,12:forskare    9:militär

    PLAY_FLC("gfx/intro/12.flc", "Unfortunately, all zee\nother prototypes were\nlost in zee explozion.   ");

    PLAY_FLC("gfx/intro/9.flc", "Yeah. He managed to make\nquite a mess out there.    ");

    PLAY_FLC("gfx/intro/8.flc", "Yes. Except for zee\nsmall... malfunction, he\nwas operating quite nizely.    ");

    PLAY_FLC("gfx/intro/9.flc", "Lots of money and\nequipment was wasted though.      ");

    PLAY_FLC("gfx/intro/8.flc", "Not to worry. Nobody\nwill ever know what\nhappened out zhere.     ");

    PLAY_FLC("gfx/intro/9.flc", "That's right. I'll\ntake care of the\ncoverup personally.     ");

    PLAY_FLC("gfx/intro/12.flc", "Won't you require\nmore money for zhat?     ");

    PLAY_FLC("gfx/intro/9.flc", "No problem. We can\nalways start a new\nwar somewhere...     ");

    PLAY_FLC("gfx/intro/8.flc", "Aah, Thiz iz why I love\nthiz country so much.\nOn to zee next project!     ");

    fade_out(4);

    rest(2000);

    clear_keybuf();
}

/**************************************************************************/
