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
#include "object2.h"
#include <vector>

inline constexpr auto FIRE_SAMPLE = "samples/missile1.wav";
/****************************************************************************/
inline constexpr auto MISSILE_SPEED = 10;
/****************************************************************************/

Missile_o::Missile_o(int X, int Y, int Z, int Speed)
    : Object(X, Y, Z)
{
    int i = 0;
    PALETTE pal;
    char filename[512];

    images.resize(2, nullptr);

    engine->sound.PlaySFX(FIRE_SAMPLE);
    for (i = 0; i < 2; i++) {
        sprintf(filename, "%s/%d.pcx", Speed > 0 ? "skotth" : "skottv", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    current_image = 0;

    friends = (FRIEND_GRENADE | FRIEND_EXPLOSION | FRIEND_DOOR | FRIEND_HS_BULLET);
    enemies = ~0;
    me = FRIEND_GRENADE;

    speed_x = Speed;
    speed_y = 0;
    speed_z = 0;
    direction = (Speed < 0 ? RIGHT_DIR : LEFT_DIR);

    counter = 0;

    energy = 10000000;
    strength = 0;

    counter = 0;
    anim.reset();
}
/****************************************************************************/
Missile_o::~Missile_o()
    = default;

/****************************************************************************/
auto Missile_o::update() -> int
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

    current_image = anim.next_frame(1, 5);

    // Check collision with walls
    if (engine->check_wall(x, y, z)
        || engine->check_wall(x + width, y, z)) {

        //		        SOUND.PlaySFX(EXPLOSION_SAMPLE);
        engine->create_object(new explosion_o(x, y - TILE_SIDE_HEIGHT + 15, z));
        //		       	engine->create_object(new explosion_o(x + 15, y - TILE_SIDE_HEIGHT, z));
        //		       	engine->create_object(new explosion_o(x - 15, y - TILE_SIDE_HEIGHT, z));

        return -1;
    }

    return 0;
}

void Missile_o::Collision(Object* o)
{
    if (energy == 0) {
        return;
    }
    if ((friends & o->GetWho()) != 0U) {
        return;
    }
    energy = 0;
    //        SOUND.PlaySFX(EXPLOSION_SAMPLE);
    engine->create_object(new explosion_o(x, y - TILE_SIDE_HEIGHT + 15, z));
    //       	engine->create_object(new explosion_o(x + 15, y - TILE_SIDE_HEIGHT, z));
    //       	engine->create_object(new explosion_o(x - 15, y - TILE_SIDE_HEIGHT, z));
}
