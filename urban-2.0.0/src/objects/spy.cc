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
/****************************************************************************/
inline constexpr auto STRENGTH_MINE = 100;
inline constexpr auto STATE_NONE = 0x00;
inline constexpr auto STATE_EXPLODE = 0x01;
/****************************************************************************/
Spy_o::Spy_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    images.resize(1);

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
    speed_y = 5;
    speed_z = 0;
    me = FRIEND_MINE;
    friends = 0;
    enemies = 0;
}
/****************************************************************************/
Spy_o::~Spy_o()
    = default;

/****************************************************************************/
auto Spy_o::update() -> int
{
    return 0;
}

void Spy_o::Collision(Object* /*o*/)
{
    victim = 1;
}
