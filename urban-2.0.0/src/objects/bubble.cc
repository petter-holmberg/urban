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
#include <cstdlib>
#include <cstring>

#define STATE_DIE 0x01
#define STATE_NONE 0x00
#define MAX_LIFE 200
#define MIN_LIFE 50

/****************************************************************************/
Bubble_o::Bubble_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    RGB pal[256];
    char filename[512];

    images = new BITMAP*;
    switch (random() % 2) {
    case 0:
        sprintf(filename, "bubble.pcx");
        break;
    case 1:
        sprintf(filename, "bubbles.pcx");
        break;
    default:
        break;
    }
    images[0] = icache.GetImage(filename, pal);
    if (images[0] != nullptr) {
        num_images++;
    }

    current_image = 0;

    height = images[0]->h;
    width = images[0]->w;
    //	x += TILE_WIDTH / 2;
    //	x -= width / 2;
    x += TILE_WIDTH;
    y -= height;
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

    me = FRIEND_BUBBLE;
    friends = 0;
    enemies = 0;

    anim.reset();

    counter2 = 0;
    counter3 = 0;
    counter4 = MIN_LIFE + (random() % MAX_LIFE);
    counter = 0;
    state = STATE_NONE;
}
/****************************************************************************/
Bubble_o::~Bubble_o()
    = default;

/****************************************************************************/
auto Bubble_o::update() -> int
{

    if (state == STATE_DIE) {
        return -1;
    }

    if (counter4 != 0) {
        counter4--;
    } else {
        state = STATE_DIE;
    }

    if (counter != 0) {
        counter--;
    }

    if (counter2 != 0) {
        counter2--;
    }

    if (counter == 0) {
        y--;
        counter = 2;
    }
    if (counter2 == 0) {
        x += (-1 + random() % 3);
        counter2 = 8;
    }

    if (ENGINE.check_floor(x, y, z)) {
        return -1;
    }

    return 0;
}

void Bubble_o::Collision(Object* o)
{
    if (o->GetWho() == FRIEND_WATER) {
        state = STATE_DIE;
    }
}
