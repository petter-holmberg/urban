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
#include "object.h"
#include <vector>

inline constexpr auto MAX_Y_SPEED = 4;
inline constexpr auto Y_ACCEL = 1;
/****************************************************************************/
Animation_o::Animation_o(int X, int Y, int Z, const char* name, int num_pics, int dek_frame, int Speed_X, int Speed_Y, int Speed_Z)
    : Object(X, Y, Z)
{
    int i = 0;
    PALETTE pal;
    char filename[512];

    images.resize(num_pics, nullptr);

    for (i = 0; i < num_pics; i++) {
        sprintf(filename, "%s%d.pcx", name, i + 1);
        images[i] = icache.GetImage(filename, pal);
    }

    coll_x = 0;
    coll_y = 0;
    coll_width = width = images[0]->w;
    coll_height = height = images[0]->h;

    x = X;
    y = Y - height;
    z = Z;
    speed_x = Speed_X;
    speed_y = Speed_Y;
    speed_z = Speed_Z;
    DekorationFrame = dek_frame;
    anim.reset();

    direction = Speed_X > 0 ? RIGHT_DIR : LEFT_DIR;
    energy = 0;
    strength = 0;
    state = 0;
}
/****************************************************************************/
Animation_o::~Animation_o()
    = default;
/****************************************************************************/
auto Animation_o::update() -> int
{

    if (state != 0) {
        return REMOVE_ME;
    }
    // Fall or stop
    if (engine->check_floor(x, y + height, z) || engine->check_floor(x + width, y + height, z)) {
        speed_y = 0;
    } else {

        y += speed_y;
        speed_y += Y_ACCEL;
        if (speed_y > MAX_Y_SPEED) {
            speed_y = MAX_Y_SPEED;
        }
    }

    current_image = anim.next_frame(images.size(), 8);
    if (current_image == images.size()) {
        if (DekorationFrame != -1) {
            current_image = DekorationFrame;
            engine->create_dekoration(this);
            state = 1; /* avoid a blip - if I return REMOVE_ME one frame wont be drawn*/
        } else {
            return REMOVE_ME;
        }
    }
    return 0;
}
/****************************************************************************/
