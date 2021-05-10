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
#include "object.h"
#include <vector>

inline constexpr auto FRAME_DELAY = 4;
inline constexpr auto BURN_DELAY = 2000;

Fire_o::Fire_o(int X, int Y, int Z, int size)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    anim.reset();
    images.resize(4, nullptr);

    for (i = 0; i < 4; i++) {
        sprintf(filename, "%s/%d.pcx", size == 1 ? "fire" : "smallfir", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    current_image = 0;
    //stå med fötterna
    y -= (height / 2);

    energy = 1;
    strength = 2;
    speed_x = 0;
    speed_y = 0;
    speed_z = 0;
    score = 0;
    friends = ~0;
    enemies = 0;
    counter = BURN_DELAY;
}

Fire_o::~Fire_o()
    = default;

auto Fire_o::update() -> int
{
    if (counter != 0) {
        counter--;
    } else {
        return -1;
    }
    current_image = anim.next_frame(3, FRAME_DELAY);
    return 0;
}
