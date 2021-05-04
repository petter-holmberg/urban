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

#define EXIST_DELAY 100
#define MOVE_DELAY 2

Points_o::Points_o(int X, int Y, int Z, int score)
    : Object(X, Y, Z)
{
    RGB pal[256];
    char filename[512];
    int i = 0;

    images = new BITMAP*[10];
    //        sprintf(filename, "xbarrel.pcx");
    for (i = 0; i < 10; i++) {
        sprintf(filename, "points/%d00.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
        if (images[i] != nullptr) {
            num_images++;
        }
    }

    current_image = (score / 100) - 1;
    if (current_image < 0) {
        current_image = 0;
    }
    if (current_image > 9) {
        current_image = 9;
    }

    height = images[0]->h;
    width = images[0]->w;
    //	x += TILE_WIDTH / 2;
    //	x -= width / 2;
    //        y -= height;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    //	rect(images[0], 0, 0, images[0]->w - 1, images[0]->h - 1, 15);

    energy = 1;
    strength = 0;
    speed_x = 0;

    speed_y = 0;
    speed_z = 0;
    speed_x = 0;
    counter = EXIST_DELAY;
    me = 0;
    friends = 0;
    enemies = 0;
}
/****************************************************************************/
Points_o::~Points_o()
    = default;

/****************************************************************************/
auto Points_o::update() -> int
{
    if (counter != 0) {
        counter--;
    }

    if (counter == 0) {
        return -1;
    }

    if ((counter % MOVE_DELAY) == 0) {
        y--;
    }
    return 0;
}

void Points_o::Collision(Object* o)
{
}
