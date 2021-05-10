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

inline constexpr auto MAX_Y_SPEED = 8;
inline constexpr auto Y_ACCEL = 1;

blood_o::blood_o(int X, int Y, int Z, int Speed_X, int Speed_Y, int Speed_Z)
    : Object(X, Y, Z)
{
    int i = 0;
    PALETTE pal;
    char filename[512];

    images.resize(3, nullptr);

    for (i = 0; i < 3; i++) {
        sprintf(filename, "dead/blod%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }

    coll_x = 0;
    coll_y = 0;
    coll_width = width = images[0]->w;
    coll_height = height = images[0]->h;
    x = X;
    y = Y;
    z = Z;
    speed_x = Speed_X;
    speed_y = Speed_Y;
    speed_z = Speed_Z;

    if (Speed_X != 0) {
        current_image = 0;
    } else {
        current_image = random() % 2 + 1;
    }
    direction = Speed_X > 0 ? RIGHT_DIR : LEFT_DIR;
    energy = 0;
    strength = 0;
    me = 1;
}

blood_o::~blood_o()
    = default;

auto blood_o::update() -> int
{

    //	x += speed_x;

    if (ENGINE.check_floor(x, y + height, z)
        || ENGINE.check_floor(x + width, y + height, z)) {
        speed_y = 0;
    } else if (speed_y < MAX_Y_SPEED) {
        speed_y += Y_ACCEL;
    }

    //	if (speed_y)
    x += speed_x;
    y += speed_y;

    if (ENGINE.check_floor(x, y + height, z)
        && ENGINE.check_floor(x + images[1]->w, y + height, z)) {
        ENGINE.create_dekoration(new blood_o(x, y, z, 0, 0, 0));
        return -1;
    }
    return 0;
}
