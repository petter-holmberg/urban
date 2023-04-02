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

inline constexpr auto STATE_NONE = 0;

inline constexpr auto MAX_X_SPEED = 2;
inline constexpr auto X_FRICTION = 1;
inline constexpr auto MIN_X_SPEED = -MAX_X_SPEED;
inline constexpr auto MAX_Y_SPEED = 16;
inline constexpr auto MIN_Y_SPEED = -12;
inline constexpr auto X_ACCEL = 2;
inline constexpr auto Y_ACCEL = 1;
inline constexpr auto Z_ACCEL = 2;
inline constexpr auto MAX_Z_SPEED = 2;
inline constexpr auto MIN_Z_SPEED = -MAX_Z_SPEED;
inline constexpr auto Z_FRICTION = 1;

/**************************************************************************/
TankWheel_o::TankWheel_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    anim.reset();

    images.resize(6, nullptr);

    for (i = 0; i < 3; i++) {
        sprintf(filename, "boss/v/band%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 3; i < 6; i++) {
        sprintf(filename, "boss/h/band%d.pcx", i - 2);
        images[i] = icache.GetImage(filename, pal);
    }
    current_image = 0;

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    //stand on feet
    y -= images[0]->h;
    energy = 10;
    strength = 10;
    speed_x = 0;
    speed_y = 0;
    speed_z = 0;
    direction = RIGHT_DIR;
    state = STATE_NONE;
    counter = 0;
    score = 0;
    friends = ~FRIEND_PLAYER;
    enemies = ENEMY_PLAYER;
    me = 0;
}

auto TankWheel_o::update() -> int
{
    if (energy <= 0) {
        return -1;
    }

    if (counter != 0) {
        counter--;
    }

    if (counter == 0) {
    }

    // Fall or Stop
    if (engine->check_floor(x, y + height, z) || engine->check_floor(x + width, y + height, z)) {
        speed_y = 0;
    } else if (speed_y < MAX_Y_SPEED) {
        speed_y += Y_ACCEL;
        speed_z = 0;
    }

    x += speed_x;
    y += speed_y;
    z += speed_z;

    if (x < 0) {
        x = 0;
    }

    if (z > MIN_Z) {
        z = MIN_Z;
        speed_z = 0;
        //                state = STATE_STOP;
        counter = 0;
    }
    if (z < MAX_Z) {
        z = MAX_Z;
        speed_z = 0;
        //                state = STATE_STOP;
        counter = 0;
    }

    layer = z / TILE_TOP_HEIGHT;

    return 0;
}

void TankWheel_o::DestroyTank()
{
    energy = 0;
}

void TankWheel_o::MoveLeft(int X)
{
    x = X;
    current_image = anim.next_frame(2, 4);
}

void TankWheel_o::MoveRight(int X)
{
    x = X;
    current_image = 2 - anim.next_frame(2, 4);
}

void TankWheel_o::Collision(Object* /*o*/)
{
    //	int i;

    //        if (!energy)
    //        	return;

    //        Object::Collision(o);

    /*	if ((o->GetStrength() > 0) && (!(o->GetFriends() & me))) {
       		if (direction == RIGHT_DIR) {
               		if (energy <= 0) {
                        	if (o->GetWho() & ENEMY_EXPLOSION)
                                	DEATH_BY_EXPLOSION
				else if (o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM))
					engine->create_effect(new BurningBody_o(x, y + height, z));
				else if (o->GetWho() & ENEMY_ICEBALL)
					engine->create_object(new FrosenBody_o(x, y + height, z, direction));
				else
					engine->create_effect(new Animation_o(x, y + height, z, "specf2/spech/dead", 4, 3));
			}
                        if (!(o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)))
				for (i = 0;i < 3;i++)
				        engine->create_effect(new blood_o(x + width / 2, y + random() % height, z, - 2 + random() % 4));
//				        engine->create_effect(new blood_o(x + width, y + random() % height, z, 1 + random() % 4));
		} else {
       	        	if (energy <= 0) {
                        	if (o->GetWho() & ENEMY_EXPLOSION)
                                	DEATH_BY_EXPLOSION
				else if (o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM))
					engine->create_effect(new BurningBody_o(x, y + height, z));
				else if (o->GetWho() & ENEMY_ICEBALL)
					engine->create_object(new FrosenBody_o(x, y + height, z, direction));
				else
					engine->create_effect(new Animation_o(x, y + height, z, "specf2/specv/dead", 4, 3));
			}
                        if (!(o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)))
				for (i = 0;i < 3;i++)
				        engine->create_effect(new blood_o(x + width / 2, y + random() % height, z, - 2 + random() % 4));
		}
                if (!(energy % 4))
	                state = STATE_HIT;
	}*/
}

/**************************************************************************/
TankWheel_o::~TankWheel_o()
    = default;
/**************************************************************************/
