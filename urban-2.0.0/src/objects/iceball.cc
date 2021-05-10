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

inline constexpr auto X_SPEED = 1;
inline constexpr auto FRAME_DELAY = 4;

IceBall_o::IceBall_o(int X, int Y, int Z, int Friends, int SpeedX, int SpeedY, int SpeedZ)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    anim.reset();
    images.resize(16, nullptr);

    for (i = 0; i < 8; i++) {
        sprintf(filename, "ice/v/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 8; i < 16; i++) {
        sprintf(filename, "ice/h/%d.pcx", i - 7);
        images[i] = icache.GetImage(filename, pal);
    }

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    direction = SpeedX < 0 ? LEFT_DIR : RIGHT_DIR;

    current_image = direction == LEFT_DIR ? 0 : 8;

    //stand on feet
    y -= (height / 2);

    x = (direction == LEFT_DIR ? x - width : x);
    energy = 10000000;
    strength = 4;
    speed_x = SpeedX + (direction == LEFT_DIR ? -X_SPEED : X_SPEED);
    speed_y = SpeedY;
    speed_z = SpeedZ;
    score = 0;
    friends = Friends | FRIEND_POWERUP | FRIEND_ICEBALL | FRIEND_HS_BULLET | FRIEND_DEKOR;
    enemies = ~friends;
    counter = 0;
    me = FRIEND_ICEBALL;
}

IceBall_o::~IceBall_o()
    = default;

auto IceBall_o::update() -> int
{
    current_image = (direction == LEFT_DIR ? 0 : 8) + anim.next_frame(8, FRAME_DELAY);
    x += speed_x;
    y += speed_y;
    z += speed_z;

    if (current_image >= (direction == LEFT_DIR ? 0 : 8) + 8) {
        return -1;
    }
    if (energy == 0) {
        return -1;
    }
    return 0;
}

void IceBall_o::Collision(Object* o)
{
    if ((o->GetWho() & friends) == 0U) {
        strength = 0;
    }
}
