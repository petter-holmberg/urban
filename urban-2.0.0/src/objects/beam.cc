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
#include "object2.h"
#include <allegro.h>

inline constexpr auto X_SPEED = 1;
inline constexpr auto FRAME_DELAY = 2;

inline constexpr auto LIFE_FRAMES = 9;

Beam_o::Beam_o(int X, int Y, int Z, int Friends, int SpeedX, int SpeedY)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    anim.reset();
    images = new BITMAP*[5];

    for (i = 0; i < 5; i++) {
        sprintf(filename, "beam/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
        if (images[i] != nullptr) {
            num_images++;
        }
    }

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    direction = SpeedX < 0 ? LEFT_DIR : RIGHT_DIR;

    current_image = direction == LEFT_DIR ? 0 : 8;
    //stå med fötterna
    //        y -= (height / 2);

    x = (direction == LEFT_DIR ? x - width : x);
    energy = 10000000;
    strength = 10;
    speed_x = SpeedX + (direction == LEFT_DIR ? -X_SPEED : X_SPEED);
    speed_y = SpeedY;
    speed_z = 0;
    score = 0;
    friends = Friends | FRIEND_FIREBALL | FRIEND_HS_BULLET | FRIEND_DEKOR;
    enemies = ~friends;
    counter = LIFE_FRAMES;
    me = FRIEND_BEAM;
}

Beam_o::~Beam_o()
    = default;

auto Beam_o::update() -> int
{

    if (counter != 0) {
        counter--;
    }

    if (counter == 0) {
        return -1;
    }

    current_image = anim.next_frame(4, FRAME_DELAY);

    x += speed_x;
    y += speed_y;
    z += speed_z;

    if (energy == 0) {
        return -1;
    }

    return 0;
}

void Beam_o::Collision(Object* o)
{

    if ((o->GetWho() & friends) == 0U) {
        strength = 0;
    }
}
