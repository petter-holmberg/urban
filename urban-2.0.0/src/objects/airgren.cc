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

/****************************************************************************/
inline constexpr auto GRENADE_SPEED = 4;
inline constexpr auto ACCEL_COUNTER = 3;
/****************************************************************************/
AirGrenade_o::AirGrenade_o(int X, int Y, int Z, int SpeedX, int SpeedY, int SpeedZ)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];

    images.resize(1, nullptr);

    sprintf(filename, "airgren.pcx");
    images[0] = icache.GetImage(filename, pal);

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    current_image = 0;

    friends = (FRIEND_GRENADE | FRIEND_EXPLOSION | FRIEND_DOOR | FRIEND_DEKOR);
    enemies = ~friends;
    me = FRIEND_GRENADE;

    speed_x = SpeedX;
    speed_z = SpeedZ;
    speed_y = SpeedY;

    energy = 10000000;
    strength = 0;

    counter = 0;
}
/****************************************************************************/
AirGrenade_o::~AirGrenade_o()
    = default;

/****************************************************************************/
auto AirGrenade_o::update() -> int
{
    x += speed_x;
    y += speed_y;
    z += speed_z;

    if (z > MIN_Z) {
        z = MIN_Z;
    }
    if (z < MAX_Z) {
        z = MAX_Z;
    }
    layer = z / TILE_TOP_HEIGHT;

    if (counter < 0) {

        speed_y++;

        counter = ACCEL_COUNTER;
    }

    counter--;
    if (energy <= 0) {
        return -1;
    }

    // Check collision with walls
    if (ENGINE.check_wall(x, y + TILE_SIDE_HEIGHT, z)
        || ENGINE.check_wall(x + width, y + TILE_SIDE_HEIGHT, z)) {

        //		        SOUND.PlaySFX(EXPLOSION_SAMPLE);
        ENGINE.create_alwaysupdate(new AirExplosion_o(x, y - TILE_SIDE_HEIGHT + 15, z));

        return -1;
    }

    return 0;
}

void AirGrenade_o::Collision(Object* o)
{
    if (energy == 0) {
        return;
    }
    if ((friends & o->GetWho()) != 0U) {
        return;
    }
    energy = 0;
    //        SOUND.PlaySFX(EXPLOSION_SAMPLE);
    ENGINE.create_alwaysupdate(new AirExplosion_o(x, y - TILE_SIDE_HEIGHT + 15, z));
}
