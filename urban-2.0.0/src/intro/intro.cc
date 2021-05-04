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
#include "intro.h"
#include "datfile.h"
#include "engine.h"
#include "game.h"
#include "icache.h"
#include "object.h"
#include "urbfont.h"
#include <allegro.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
/**************************************************************************/
#define FRAME_DELAY 100
/**************************************************************************/
UrbanFont* lg;
int quit;
static char flctext[256];
/**************************************************************************/
Intro::Intro()
{
    lg = new UrbanFont(SMALL_FONT);
}
/**************************************************************************/
Intro::~Intro()
{
    delete lg;
}
/**************************************************************************/
extern "C" {
static auto callback() -> int
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

    if (keypressed() != 0) {
        quit = 1;
    }

    if ((count == (num_frames - 1)) || (quit != 0)) {

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
                                                 \
        if (keypressed()) {                      \
                                                 \
            QUIT;                                \
        }                                        \
    }

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
/**************************************************************************/
void Intro::RunIntro()
{
    datfile dat("intro.dat");
    char* buf = nullptr;
    int i = 0;
    BITMAP* bmp = nullptr;
    quit = 0;
    char text[128];
    char temptext[128];

    PLAY_MOD("%s/snd/modules/warning.mod");
    // Warning:
    DISPLAY_IMAGE("warning.pcx");
    // Wait for keypress
    while (keypressed() == 0) {
        ;
    }

    fade_out(5);

    PLAY_MOD("%s/snd/modules/blazer.xm");

    // Limousine:

    DISPLAY_IMAGE("intro/1.pcx");

    rest(2000);

    ANIM_TEXT("Container terminal 3B\nLos Angeles Harbour, USA\n11:45 am... Present Day");

    rest(1000);

    PLAY_FLC("gfx/intro/2.flc",
        "                              ");

    PLAY_FLC("gfx/intro/3.flc",
        "Zee lab und all our equipment\niz ready. Zee only thing we need now\niz ein object. ");

    rest(1000);

    PLAY_FLC("gfx/intro/4.flc",
        "Sure, I'll take care of it.\nDoes he need any special abilities?          ");

    PLAY_FLC("gfx/intro/3.flc",
        "Of course, here you have eine list...     ");

    PLAY_FLC("gfx/intro/5.flc",
        "                                   ");

    PLAY_FLC("gfx/intro/6.flc",
        "Aah... I think I just found\nthe perfect victim!          ");

    rest(1000);

    DISPLAY_IMAGE("intro/id.pcx");

    rest(2000);

    ANIM_TEXT("Yes, He will be perfect!\n       He He He He!");

    rest(1000);

    DISPLAY_IMAGE("intro/1.pcx");

    rest(3000);

    fade_out(4);

    rest(2000);

    DISPLAY_IMAGE("intro/nevada.pcx");

    rest(1000);

    ANIM_TEXT("Three months later...\nLocation: Top secret military\nfacility for weapon development.\nSomewhere in the Nevada desert.");

    rest(3000);

    PLAY_FLC("gfx/intro/7.flc", "                      ");

    PLAY_FLC("gfx/intro/8.flc", "This iz zee last stage\nin zee process.   ");

    rest(1000);

    PLAY_FLC("gfx/intro/9.flc", "Great! \nSo we can soon begin\nthe battle testing?   ");

    PLAY_FLC("gfx/intro/8.flc", "Yes, in about zree days\nfrom now.    ");

    PLAY_FLC("gfx/intro/7.flc", "Zhen you can play with it\nas much as you like.\n      \nPerfect....    ");

    PLAY_FLC("gfx/intro/11.flc", "                             \nWHAT?!!      ");

    PLAY_FLC("gfx/intro/12.flc", "He iz not suppozed to\nwake up now.     \nThiz can become eine...\n...grosse problem.");

    rest(2000);
}

/**************************************************************************/
