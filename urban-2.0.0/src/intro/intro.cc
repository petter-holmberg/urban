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
inline constexpr auto FRAME_DELAY = 100;
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
inline auto PLAY_FLC(datfile& dat, const char* x, const char* y) -> bool
{
    char* buf = nullptr;
    strcpy(flctext, y);
    if ((buf = dat.load_file_to_memory(x)) == nullptr) {
        exit(1);
    }
    play_memory_fli(buf, screen, 1, callback);
    delete[] buf;
    if (quit != 0) {
        clear_keybuf();
        return false;
    }
    return true;
}

inline auto ANIM_TEXT(const char* x) -> bool
{
    char text[128];
    char temptext[128];
    strcpy(text, x);

    clear_keybuf();

    for (int i = 0; i < (signed)strlen(text); i++) {

        strncpy(temptext, text, i + 1);
        temptext[i + 1] = 0;
        lg->print(temptext, 50, 170, screen);

        rest(FRAME_DELAY);

        if (keypressed() != 0) {
            clear_keybuf();
            return false;
        }
    }
    return true;
}

inline void DISPLAY_IMAGE(BITMAP* bmp, PALETTE& palette, const char* x)
{
    bmp = icache.GetImage(x, palette);
    set_palette(palette);
    blit(bmp, screen, 0, 0, 0, 0, 320, 240);
    icache.FreeImage(bmp);
}

inline void PLAY_MOD(const char* x)
{
    char* buf = new char[1024];

    sprintf(buf, x, DATPATH);
    ENGINE.sound.PlayMusic(buf);

    delete[] buf;
    ENGINE.sound.SetMusicVolume(64);
}
/**************************************************************************/
void Intro::RunIntro()
{
    datfile dat("intro.dat");
    int i = 0;
    BITMAP* bmp = nullptr;
    quit = 0;
    char text[128];
    char temptext[128];

    PLAY_MOD("%s/snd/modules/warning.mod");
    // Warning:
    DISPLAY_IMAGE(bmp, palette, "warning.pcx");
    // Wait for keypress
    while (keypressed() == 0) {
        ;
    }

    fade_out(5);

    PLAY_MOD("%s/snd/modules/blazer.xm");

    // Limousine:

    DISPLAY_IMAGE(bmp, palette, "intro/1.pcx");

    rest(2000);

    if (!ANIM_TEXT("Container terminal 3B\nLos Angeles Harbour, USA\n11:45 am... Present Day")) {
        return;
    }

    rest(1000);

    if (!PLAY_FLC(dat, "gfx/intro/2.flc",
            "                              ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/3.flc",
            "Zee lab und all our equipment\niz ready. Zee only thing we need now\niz ein object. ")) {
        return;
    }

    rest(1000);

    if (!PLAY_FLC(dat, "gfx/intro/4.flc",
            "Sure, I'll take care of it.\nDoes he need any special abilities?          ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/3.flc",
            "Of course, here you have eine list...     ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/5.flc",
            "                                   ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/6.flc",
            "Aah... I think I just found\nthe perfect victim!          ")) {
        return;
    }

    rest(1000);

    DISPLAY_IMAGE(bmp, palette, "intro/id.pcx");

    rest(2000);

    if (!ANIM_TEXT("Yes, He will be perfect!\n       He He He He!")) {
        return;
    }

    rest(1000);

    DISPLAY_IMAGE(bmp, palette, "intro/1.pcx");

    rest(3000);

    fade_out(4);

    rest(2000);

    DISPLAY_IMAGE(bmp, palette, "intro/nevada.pcx");

    rest(1000);

    if (!ANIM_TEXT("Three months later...\nLocation: Top secret military\nfacility for weapon development.\nSomewhere in the Nevada desert.")) {
        return;
    }

    rest(3000);

    if (!PLAY_FLC(dat, "gfx/intro/7.flc", "                      ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/8.flc", "This iz zee last stage\nin zee process.   ")) {
        return;
    }

    rest(1000);

    if (!PLAY_FLC(dat, "gfx/intro/9.flc", "Great! \nSo we can soon begin\nthe battle testing?   ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/8.flc", "Yes, in about zree days\nfrom now.    ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/7.flc", "Zhen you can play with it\nas much as you like.\n      \nPerfect....    ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/11.flc", "                             \nWHAT?!!      ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/12.flc", "He iz not suppozed to\nwake up now.     \nThiz can become eine...\n...grosse problem.")) {
        return;
    }

    rest(2000);
}

/**************************************************************************/
