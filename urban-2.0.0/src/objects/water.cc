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
Water_o::Water_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];

    images.resize(2, nullptr);
    for (int i = 0; i < 2; i++) {
        sprintf(filename, "water%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }
    current_image = 0;

    height = images[0]->h;
    width = images[0]->w;
    y -= height;
    y += 10;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;
    energy = 1;
    strength = 0;

    speed_x = 0;
    speed_y = 0;
    speed_z = 0;
    counter = 0;

    me = FRIEND_WATER;
    friends = 0;
    enemies = 0;

    anim.reset();

    counter2 = 0;
    counter3 = 0;
    counter = 0;
}
/****************************************************************************/
Water_o::~Water_o()
    = default;

/****************************************************************************/
auto Water_o::update() -> int
{
    current_image = anim.next_frame(1, 60);
    return 0;
}

void Water_o::Collision(Object* o)
{
    if (o->GetSpeedY() > 0 && o->GetY() > y) {
        o->SetMode(MODE_WATER);
        ENGINE.map.SetPal(PAL_WATER);
    } else if (o->GetMode() == MODE_WATER && o->GetY() <= y) {
        o->SetMode(MODE_NORMAL);
        ENGINE.map.SetPal(PAL_AIR);
    }
}
