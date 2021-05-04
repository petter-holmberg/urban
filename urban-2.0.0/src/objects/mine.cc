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
#include <cstdlib>
#include <cstring>
/****************************************************************************/
inline constexpr auto STRENGTH_MINE = 100;
inline constexpr auto STATE_NONE = 0x00;
inline constexpr auto STATE_EXPLODE = 0x01;
/****************************************************************************/
mine_o::mine_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];

    images = new BITMAP*[2];
    sprintf(filename, "mine1.pcx");
    images[0] = icache.GetImage(filename, pal);
    if (images[0] != nullptr) {
        num_images++;
    }
    sprintf(filename, "mine2.pcx");
    images[1] = icache.GetImage(filename, pal);
    if (images[1] != nullptr) {
        num_images++;
    }

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
    strength = STRENGTH_MINE;
    speed_x = 0;
    // Fall to the floor
    speed_y = 5;
    speed_z = 0;
    counter = random() % 50;
    counter2 = 0;

    me = FRIEND_MINE;
    friends = ~FRIEND_PLAYER;
    enemies = ~friends;

    state = STATE_NONE;
}
/****************************************************************************/
mine_o::~mine_o()
    = default;

/****************************************************************************/
auto mine_o::update() -> int
{
    if (counter != 0) {
        counter--;
    }

    if (counter == 0) {
        switch (state) {
        case STATE_NONE:
            current_image = current_image == 0 ? 1 : 0;
            if (current_image != 0) {
                counter = 100;
            } else {
                counter = 10;
            }
            break;
        case STATE_EXPLODE:
            strength = 0;
            counter = 10;
            counter2++;
            if (counter2 >= 3) {
                energy = 0;
            }
            switch (counter2) {
            case 1:
                ENGINE.create_object(new explosion_o(x, y - TILE_SIDE_HEIGHT, z));
                break;
            case 2:
                ENGINE.create_object(new explosion_o(x - TILE_WIDTH, y - TILE_SIDE_HEIGHT, z));
                break;
            case 3:
                ENGINE.create_object(new explosion_o(x + TILE_WIDTH, y - TILE_SIDE_HEIGHT, z));
                break;
            };
        };
    }

    // Fall or Stop
    if (ENGINE.check_floor(x, y + height, z) || ENGINE.check_floor(x + width, y + height, z)) {
        speed_y = speed_y;
    } else {
        y += speed_y;
    }

    // Delete if already used
    if (energy == 0) {
        return -1;
    }
    return 0;
}

void mine_o::Collision(Object* o)
{
    if (energy == 0) {
        return;
    }

    if (o->GetWho() == 0 || ((friends & o->GetWho()) != 0U)) {
        return;
    }

    if (state != STATE_EXPLODE) {
        state = STATE_EXPLODE;
        counter = 0;
        energy = 100000;
    }
}
