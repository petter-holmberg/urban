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
#include "allegro.h"
#include "engine.h"
#include "object2.h"
#include <vector>

/****************************************************************************/
inline constexpr auto STATE_NONE = 0x00;
inline constexpr auto STATE_WAIT = 0x01;
inline constexpr auto STATE_STRIKE = 0x02;
inline constexpr auto STATE_DONE = 0x03;

inline constexpr auto NUM_NUKES = 90;
inline constexpr auto NUKE_MULTI = 10;

inline constexpr auto AREA_START = -300;
inline constexpr auto AREA_END = 300;
inline constexpr auto AREA_WIDTH = 600;
inline constexpr auto AREA_MUL = AREA_WIDTH / NUM_NUKES;

inline constexpr auto STRIKE_DELAY = 600;

inline constexpr auto STRIKE_START = -20;
inline constexpr auto STRIKE_END = 20;

inline constexpr auto STRIKE_SAMPLE = "samples/hellfire.wav";
inline constexpr auto WARNING_SAMPLE = "samples/airnuke.wav";

/****************************************************************************/
Airstrike_o::Airstrike_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];

    images.resize(1, nullptr);
    sprintf(filename, "mine2.pcx");
    images[0] = icache.GetImage(filename, pal);

    current_image = 0;

    height = images[0]->h;
    width = images[0]->w;
    x += TILE_WIDTH / 2;
    x -= width / 2;
    y -= height;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;
    energy = 1;
    strength = 0;
    speed_x = 0;
    // Fall to the floor
    speed_y = 0;
    speed_z = 0;
    counter = 0;
    //	evil = 1;
    me = FRIEND_DEKOR;
    friends = 0;
    enemies = 0;

    counter2 = 0;
    counter3 = 0;
    state = STATE_NONE;
}
/****************************************************************************/
Airstrike_o::~Airstrike_o()
    = default;

/****************************************************************************/
auto Airstrike_o::update() -> int
{

    if (counter != 0) {
        counter--;
    }

    if (counter2 != 0) {
        counter2--;
    }

    if (state == STATE_DONE) {
        return -1;
    }

    switch (state) {
    case STATE_NONE:
        if (PLAYER->GetX() > x + STRIKE_START && PLAYER->GetX() < x + STRIKE_END) {
            state = STATE_WAIT;
            counter = STRIKE_DELAY;
            ENGINE.PushMessage("Airstrike alert!");
            SOUND.PlaySFX_Critical(WARNING_SAMPLE);
        }
        break;
    case STATE_WAIT:
        if (counter == 0) {
            state = STATE_STRIKE;
        }
        break;
    case STATE_STRIKE:
        SOUND.PlaySFX_Critical(STRIKE_SAMPLE);
        for (int i = 0; i < NUM_NUKES; i++) {
            ENGINE.create_alwaysupdate(new AirGrenade_o(x + AREA_START + i * AREA_MUL, y - i * 15, (random() % 3) * TILE_TOP_HEIGHT, 2, 5, 0));
        }
    default:
        state = STATE_DONE;
        break;
    }

    return 0;
}

void Airstrike_o::Collision(Object* o)
{
}
