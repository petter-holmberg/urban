/******************************************************************************
    URBAN 2.0
    Copyright (C) 2021  Petter Holmberg (petter.holmberg@usa.net)

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
*****************************************************************************/
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
#include <cstdio>
#include <cstring>
/**************************************************************************/
inline constexpr auto FRAME_DELAY = 100;
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

    /*                if(keypressed())
                	quit = 1;*/

    if ((count == (num_frames - 1)) || (quit != 0)) {

        count = 0;

        return 1;
    }

    count++;
    return 0;
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

inline void DISPLAY_IMAGE(BITMAP* bmp, PALETTE& palette, const char* x)
{
    bmp = icache.GetImage(x, palette);
    set_palette(palette);
    blit(bmp, screen, 0, 0, 0, 0, 320, 240);
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
void Outtro::RunOuttro()
{
    datfile dat("intro.dat");
    char* buf = nullptr;
    quit = 0;
    PALETTE palette;
    BITMAP* bmp = nullptr;

    clear_keybuf();

    PLAY_MOD("%s/snd/modules/rock.xm");

    //        get_palette(pal);

    DISPLAY_IMAGE(bmp, palette, "intro/atom.pcx");

    //        fade_in(pal, 5);

    rest(6000);

    fade_out(5);

    if (!PLAY_FLC(dat, "gfx/intro/12.flc", "A zhame we had to nuke\nzee complex to destroy him...   ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/9.flc", "Well, at least we \nmanaged to stop him.   ")) {
        return;
    }

    //        rest(1000);                      8,12:forskare    9:militär

    if (!PLAY_FLC(dat, "gfx/intro/12.flc", "Unfortunately, all zee\nother prototypes were\nlost in zee explozion.   ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/9.flc", "Yeah. He managed to make\nquite a mess out there.    ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/8.flc", "Yes. Except for zee\nsmall... malfunction, he\nwas operating quite nizely.    ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/9.flc", "Lots of money and\nequipment was wasted though.      ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/8.flc", "Not to worry. Nobody\nwill ever know what\nhappened out zhere.     ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/9.flc", "That's right. I'll\ntake care of the\ncoverup personally.     ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/12.flc", "Won't you require\nmore money for zhat?     ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/9.flc", "No problem. We can\nalways start a new\nwar somewhere...     ")) {
        return;
    }

    if (!PLAY_FLC(dat, "gfx/intro/8.flc", "Aah, Thiz iz why I love\nthiz country so much.\nOn to zee next project!     ")) {
        return;
    }

    fade_out(4);

    rest(2000);

    clear_keybuf();
}

/**************************************************************************/
