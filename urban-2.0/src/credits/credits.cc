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

    Bumpmapper for the URBAN credits
    Copyright (C) 1999  Thomas Nyberg

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

    Contact the author:
    Snailmail:
    Thomas Nyberg
    Brännbergsvägen 21b
    92136 Lycksele
    Sweden

    or email:

    thomas.nyberg@linux.nu


    The bumpmapper is based upon code provided by Sqrt(-1)

*****************************************************************************/

#include "allegro.h"
#include "engine.h"
#include "icache.h"
#include "urbfont.h"
#include <algorithm>
#include <cmath>

inline constexpr auto LIGHT_SIZE = 128.0;

inline void cblit(BITMAP* bmp, BITMAP* dest, int x, int y)
{
    masked_blit(bmp, dest, 0, 0, (x) - (bmp)->w / 2, y, (bmp)->w, (bmp)->h);
}

struct Lightinfo {
    int x;
    int y;
    int enabled;
};

Lightinfo LI[] = {
    { 25, 50, 0 },
    { 250, 150, 0 },
    { 250, 15, 0 },
    { 50, 175, 0 },
    { 160, 120, 0 },
};

inline constexpr auto NUM_LIGHTS = (signed)(sizeof(LI) / sizeof(LI[0]));

struct CreditsInfo {
    char title[32];
    char name[32];
    int erase_title;
} _ci[] = {
    { "PROGRAMMING",
        "THOMAS\nNYBERG", 0 },
    { "PROGRAMMING",
        "  JONAS\nBORGSTROM", 1 },
    { "GRAPHICS",
        "  DICK\nADOLFSSON", 1 },
    { "DESIGN",
        " PETTER\nHOLMBERG", 0 },
    { "DESIGN",
        "MARKUS\nBOMAN", 1 },
    { "SOUND",
        "  EMIL\nERIKSSON", 0 },
    { "SOUND",
        "RICHARD\nBERGMARK", 1 },
    { "MUSIC",
        "SAMUEL\nPERSSON", 1 },
};

inline constexpr auto NUMCI = (signed)(sizeof(_ci) / sizeof(_ci[0]));

const char* special_thanks = "\n"
                             " SPECIAL THANKS\n"
                             "\n"
                             " PER JONNY KACK\n"
                             "  TOR SANDEN\n"
                             "MARKUS WILLANDER\n"
                             "    UFFEMAN\n"
                             "    HASSEMAN\n"
                             "  ANNA NORBERG\n"
                             "\n"
                             "    DESCENT\n"
                             "\n"
                             " AND OURSELVES\n"
                             "\n"
                             "      NOW\n"
                             "  PLAY URBAN";

unsigned char lightmap[256 * 256] = {};
BITMAP* _buffer_ = nullptr;
BITMAP* heightmap = nullptr;

static void InitLightmap()
{
    static int first = 1;

    if (first == 0) {
        return;
    }

    first = 0;

    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            float nx = ((x - 128) / LIGHT_SIZE);
            float ny = ((y - 128) / LIGHT_SIZE);
            float nz = 1 - std::sqrt(nx * nx + ny * ny);

            if (nz < 0) {
                nz = 0;
            }

            lightmap[x + y * 256] = static_cast<unsigned char>(std::min(255.0F, nz * 191 + nz * nz * nz * nz * nz * nz * nz * nz * nz * 64));
        }
    }
}

static void DoBump(BITMAP* hmap, int starty, int stopy)
{
    int nx = 0;
    int ny = 0;
    int rx = 0;
    int ry = 0;
    int nx1 = 0;
    int ny1 = 0;
    int lval = 0;
    unsigned int i = 0;

    if (starty < 1) {
        starty = 1;
    }

    if (stopy > SCREEN_HEIGHT - 2) {
        stopy = SCREEN_HEIGHT - 2;
    }

    for (int y = starty; y < stopy; y++) {
        for (int x = 1; x < 319; x++) {

            nx = hmap->line[y][x + 1] - hmap->line[y][x - 1];
            ny = hmap->line[y + 1][x] - hmap->line[y - 1][x];

            for (i = 0, lval = 0; i < NUM_LIGHTS; i++) {
                if (LI[i].enabled == 0) {
                    continue;
                }

                rx = x - LI[i].x;
                ry = y - LI[i].y;

                nx1 = nx - (rx - 128);
                ny1 = ny - (ry - 128);

                if (nx1 > 255 || nx1 < 0) {
                    nx1 = 255;
                }

                if (ny1 > 255 || ny1 < 0) {
                    ny1 = 255;
                }
                lval += lightmap[nx1 + ny1 * 256];
            }
            if (lval > 255) {
                lval = 255;
            }
            putpixel(_buffer_, x, y, lval);
        }
    }
}

static auto morf(BITMAP* source, BITMAP* dest, int starty, int stopy, int numframes) -> BITMAP*
{
    int i = 0;
    int j = 0;
    int k = 0;
    BITMAP* tmp = nullptr;

    tmp = create_bitmap(source->w, source->h);

    for (k = 1; k <= numframes; k++) {
        for (j = starty; j < stopy; j++) {
            for (i = 0; i < 320; i++) {
                (tmp->line[j][i]) = (source->line[j][i]) + (int)((float)((dest->line[j][i]) - (source->line[j][i])) * ((float)k / (float)numframes));
            }
        }
        DoBump(tmp, starty + 1, stopy - 1);
        blit(_buffer_, screen, 0, starty, 0, starty, _buffer_->w, stopy - starty + 1);

        if (keypressed() != 0) {
            break;
        }
    }
    return tmp;
}

inline void DELAY(int x)
{
    int mul = 0;
    mul = (x) / 10;
    for (; mul != 0; mul--) {
        rest(10);
        if (keypressed() != 0) {
            break;
        }
    }
}

void showcredits()
{
    PALETTE pal;
    BITMAP* hmap = nullptr;
    BITMAP* tmp = nullptr;
    BITMAP* txt = nullptr;

    int starty = 0;
    int stopy = SCREEN_HEIGHT;
    int i = 0;
    int quitting = 0;

    InitLightmap();

    UrbanFont ufont(LARGE_FONT);
    _buffer_ = create_bitmap(320, SCREEN_HEIGHT);

    hmap = icache.GetImage("credits/bump.pcx", pal);

    icache.GetImage("credits/palette.pcx", pal);

    clear(screen);
    set_palette(pal);

    if (heightmap == nullptr) {
        heightmap = create_bitmap(320, 240);
    }

    clear(heightmap);
    clear(_buffer_);

    blit(hmap, heightmap, 0, 0, 0, 0, hmap->w, hmap->h);

    ufont.SetScale(180);
    txt = ufont.print("URBAN");
    cblit(txt, heightmap, 160, 30);
    destroy_bitmap(txt);

    ufont.SetScale(60);
    txt = ufont.print("THE");
    cblit(txt, heightmap, 160, 120);
    destroy_bitmap(txt);

    txt = ufont.print("CYBORG PROJECT");
    cblit(txt, heightmap, 160, 150);
    destroy_bitmap(txt);

    for (i = 0; i < NUM_LIGHTS; i++) {
        DELAY(500);
        engine->sound.PlaySFX("samples/spots.wav");
        LI[i].enabled = 1;
        DoBump(heightmap, 0, SCREEN_HEIGHT);
        blit(_buffer_, screen, 0, 0, 0, 0, _buffer_->w, _buffer_->h);
        if (keypressed() != 0) {
            for (; i >= 0; i--) {

                rest(500);
                engine->sound.PlaySFX("samples/spots.wav");
                LI[i].enabled = 0;
                DoBump(heightmap, 0, SCREEN_HEIGHT);
                blit(_buffer_, screen, 0, 0, 0, 0, _buffer_->w, _buffer_->h);
            }
            fade_out(10);
            destroy_bitmap(_buffer_);
            return;
        }
    }
    DELAY(1200);
    destroy_bitmap(morf(heightmap, hmap, 0, SCREEN_HEIGHT, 10));
    for (i = 0; i < NUMCI; i++) {
        blit(hmap, heightmap, 0, 0, 0, 0, hmap->w, hmap->h);

        ufont.SetScale(80);
        txt = ufont.print(_ci[i].title);
        cblit(txt, heightmap, 160, 20);
        destroy_bitmap(txt);
        ufont.SetScale(100);

        txt = ufont.print(_ci[i].name);
        cblit(txt, heightmap, 160, 80);

        if (i == 0 || (i > 0 && (_ci[i - 1].erase_title != 0))) {
            starty = 10;
        } else {
            starty = 70;
        }

        stopy = 80 + txt->h;
        destroy_bitmap(txt);

        tmp = morf(hmap, heightmap, starty, stopy, 10);

        DELAY(900);

        if (keypressed() != 0) {
            destroy_bitmap(tmp);
            tmp = morf(hmap, heightmap, 0, SCREEN_HEIGHT, 1);
            for (i = NUM_LIGHTS - 1; i >= 0; i--) {

                rest(500);
                engine->sound.PlaySFX("samples/spots.wav");
                LI[i].enabled = 0;

                DoBump(tmp, 0, SCREEN_HEIGHT);
                blit(_buffer_, screen, 0, 0, 0, 0, _buffer_->w, _buffer_->h);
            }
            return;
        }

        if (_ci[i].erase_title == 0) {
            starty = 70;
        } else {
            starty = 10;
        }
        destroy_bitmap(tmp);
    }

    if ((special_thanks != nullptr) && (quitting == 0)) {
        ufont.SetScale(60);
        txt = ufont.print(special_thanks);

        blit(hmap, heightmap, 0, 0, 0, 0, hmap->w, hmap->h);
        cblit(txt, heightmap, 160, SCREEN_HEIGHT - 60);
        destroy_bitmap(morf(hmap, heightmap, 0, SCREEN_HEIGHT, 10));
        blit(_buffer_, screen, 0, 0, 0, 0, _buffer_->w, _buffer_->h);
        tmp = create_bitmap(320, SCREEN_HEIGHT - 20);
        int y = 0;
        for (y = SCREEN_HEIGHT - 60, i = NUM_LIGHTS - 1; y > -txt->h; y -= 1) {
            blit(hmap, heightmap, 0, 0, 0, 0, hmap->w, hmap->h);
            clear(tmp);
            cblit(txt, tmp, 160, y - 10);
            masked_blit(tmp, heightmap, 0, 0, 0, 10, tmp->w, tmp->h);

            DoBump(heightmap, 0, SCREEN_HEIGHT);
            blit(_buffer_, screen, 0, 0, 0, 0, _buffer_->w, _buffer_->h);

            if ((keypressed() != 0) || ((y + txt->h < 100) && (y % 20) == 0)) {
                for (i = NUM_LIGHTS - 1; i >= 0; i--) {

                    rest(500);
                    engine->sound.PlaySFX("samples/spots.wav");
                    LI[i].enabled = 0;
                    DoBump(heightmap, 0, SCREEN_HEIGHT);
                    blit(_buffer_, screen, 0, 0, 0, 0, _buffer_->w, _buffer_->h);
                }
                break;
            }
            DELAY(20);
        }
        destroy_bitmap(txt);
        destroy_bitmap(tmp);
    }

    destroy_bitmap(_buffer_);

    fade_out(10);
}
