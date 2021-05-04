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
#include "object2.h"
#include <allegro.h>
#include <cstring>

#define EXPLOSION_SAMPLE "samples/heart_1.wav"
#define FIRE_SAMPLE "samples/grenade.wav"
/****************************************************************************/
#define GRENADE_SPEED 4
#define ACCEL_COUNTER 3
/****************************************************************************/

rocket_o::rocket_o(int X, int Y, int Z, int Speed, int Mode)
    : Object(X, Y, Z, Mode)
{
    RGB pal[256];
    char filename[512];

    images = new BITMAP*[1];

    SOUND.PlaySFX(FIRE_SAMPLE);
    if (Speed > 0) {
        sprintf(filename, "grenade.pcx");
    } else {
        sprintf(filename, "grenade2.pcx");
    }

    images[0] = icache.GetImage(filename, pal);
    if (images[0] != nullptr) {
        num_images++;
    }

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    current_image = 0;

    me = FRIEND_GRENADE;
    friends = (me | FRIEND_GRENADE | FRIEND_FIREBALL | FRIEND_EXPLOSION | FRIEND_DOOR);
    enemies = ~friends;

    speed_x = mode == MODE_NORMAL ? Speed : Speed / 2;
    speed_y = 0;
    speed_z = 0;
    direction = (Speed < 0 ? RIGHT_DIR : LEFT_DIR);

    counter = 0;

    energy = 10000000;
    strength = 0;

    counter = 0;
}
/****************************************************************************/
rocket_o::~rocket_o()
    = default;

/****************************************************************************/
auto rocket_o::update() -> int
{
    x += speed_x;

    if (x < 0) { //out of range
        return -1;
    }

    if (energy <= 0) {
        return -1;
    }

    counter += speed_x;

    if (counter > 600) {
        return -1;
    }

    // Check collision with walls
    if (ENGINE.check_wall(x, y, z)
        || ENGINE.check_wall(x + width, y, z)) {

        SOUND.PlaySFX(EXPLOSION_SAMPLE);
        ENGINE.create_object(new explosion_o(x, y - TILE_SIDE_HEIGHT + 15, z));
        //		       	ENGINE.create_object(new explosion_o(x + 15, y - TILE_SIDE_HEIGHT, z));
        //		       	ENGINE.create_object(new explosion_o(x - 15, y - TILE_SIDE_HEIGHT, z));

        return -1;
    }
    if (mode == MODE_WATER) {
        if (speed_x < 0) {
            if ((random() % 10) == 0) {
                ENGINE.create_object(new Bubble_o(x - 20, y, z));
            }
        } else {
            if ((random() % 10) == 0) {
                ENGINE.create_object(new Bubble_o(x - 30, y, z));
            }
        }
    }
    return 0;
}

void rocket_o::Collision(Object* o)
{
    if (energy == 0) {
        return;
    }
    if ((friends & o->GetWho()) != 0U) {
        return;
    }
    energy = 0;
    SOUND.PlaySFX(EXPLOSION_SAMPLE);
    ENGINE.create_object(new explosion_o(x, y - TILE_SIDE_HEIGHT + 15, z));
    //       	ENGINE.create_object(new explosion_o(x + 15, y - TILE_SIDE_HEIGHT, z));
    //       	ENGINE.create_object(new explosion_o(x - 15, y - TILE_SIDE_HEIGHT, z));
}
