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
#include "engine.h"
#include "object.h"
#include <allegro.h>
/****************************************************************************/
inline constexpr auto MAX_Y_SPEED = 4;
inline constexpr auto MIN_Y_SPEED = -4;
inline constexpr auto Y_ACCEL = 1;
/****************************************************************************/
inline constexpr auto STATE_NONE = 0;
inline constexpr auto STATE_DESTROY = 1;
/****************************************************************************/
bodyparts_o::bodyparts_o(int X, int Y, int Z, const char* name, int num_pics, int Speed_X, int Speed_Y, int Speed_Z)
    : Object(X, Y, Z)
{
    int i = 0;
    PALETTE pal;
    char filename[512];

    images = new BITMAP*[num_pics];

    for (i = 0; i < num_pics; i++) {
        sprintf(filename, "%s%d.pcx", name, i + 1);
        images[i] = icache.GetImage(filename, pal);
        if (images[i] != nullptr) {
            num_images++;
        }
    }
    coll_x = 0;
    coll_y = 0;
    coll_width = width = images[0]->w;
    coll_height = height = images[0]->h;
    x = X;
    y = Y - height;
    me = 2;
    z = Z;
    speed_x = Speed_X;
    speed_y = Speed_Y;
    speed_z = Speed_Z;
    if (Speed_X != 0) {
        current_image = 0;
    } else {
        current_image = 0;
    }
    direction = Speed_X > 0 ? RIGHT_DIR : LEFT_DIR;
    energy = 0;
    strength = 0;
    anim.reset();

    if (ENGINE.check_floor(x, y + height, z) || ENGINE.check_floor(x + width, y + height, z) || ENGINE.check_wall(x, y + height, z) || ENGINE.check_wall(x + width, y + height, z)) {

        state = STATE_DESTROY;
    } else {
        state = STATE_NONE;
    }
}
/****************************************************************************/
bodyparts_o::~bodyparts_o()
    = default;
/****************************************************************************/
auto bodyparts_o::update() -> int
{
    if (state == STATE_DESTROY) {
        return REMOVE_ME;
    }

    if (ENGINE.check_wall(x + speed_x, y + height, z) || ENGINE.check_wall(x + width + speed_x, y + height, z)) {
        speed_x = -(speed_x / 2);
    }

    if (ENGINE.check_floor(x, y + height + speed_y, z)
        || ENGINE.check_floor(x + width, y + height + speed_y, z)) {
        speed_y = -(speed_y / 2);

        if (speed_y == 0) {

            ENGINE.create_dekoration(this);
            return REMOVE_ME;
        }
        //current_image = 0;
        //ENGINE.create_dekoration(new head_o(x, y, z, 0, 0, 0, "urban\\huvud"));
    }
    if (speed_y < MAX_Y_SPEED) {
        speed_y += Y_ACCEL;
    }

    current_image = anim.next_frame(num_images - 1, 4);
    if (speed_y != 0) {
        x += speed_x;
    }

    y += speed_y;

    return 0;
}
/****************************************************************************/
