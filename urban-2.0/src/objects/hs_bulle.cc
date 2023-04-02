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
    Copyright (C) 1999  Thomas Nyberg and Jonas Borgstr�m

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
    Thomas Nyberg					Jonas Borgstr�m
    Br�nnbergsv�gen 21b					Bondev�gen 6
    92136 Lycksele					92332 Storuman
    Sweden						Sweden

    or email:

    thomas.nyberg@usa.net				jonas_b@bitsmart.com
*****************************************************************************/
#include "allegro.h"
#include "engine.h"
#include "object.h"
#include <vector>
/***************************************************************************/
inline constexpr auto HS_SPEED = 7;
inline constexpr auto FIRE_SAMPLE = "samples/gun2.wav";
/***************************************************************************/
HighSpeed_Bullet_o::HighSpeed_Bullet_o(int X, int Y, int Z, int Dir, int friendship, int Damage)
    : Object(X, Y, Z)
{
    char filename[512];
    PALETTE pal;

    width = 1;
    height = 1;
    strength = Damage;
    energy = 1;
    direction = Dir;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;
    counter = 0;

    images.resize(1, nullptr);

    sprintf(filename, "hsbullet.pcx");
    images[0] = icache.GetImage(filename, pal);

    current_image = 0;
    me = FRIEND_HS_BULLET;
    friends = (friendship | me | FRIEND_DOOR | FRIEND_FIREBALL | FRIEND_ICEBALL | FRIEND_DEKOR | FRIEND_GRENADE | FRIEND_MINE) & ~ENEMY_SCIENTIST;
    enemies = ~friends;
    speed_x = (direction == RIGHT_DIR) ? HS_SPEED : -HS_SPEED;

    engine->sound.PlaySFX(FIRE_SAMPLE);
}
/****************************************************************************/
auto HighSpeed_Bullet_o::update() -> int
{

    if (energy == 0) {
        return -1;
    }
    /*        if (direction == LEFT_DIR)
        	while (engine->check_wall(x + 1, y, z) && speed_x)
                	x++;
	else if (direction == RIGHT_DIR)
        	while (engine->check_wall(x + coll_width - 1, y, z) && speed_x)
                	x--;
*/
    /*	if (direction == LEFT_DIR)
	        while (engine->check_wall(x + speed_x, y, z) && speed_x < -1)
        	       	speed_x++;
	else if (direction == RIGHT_DIR)
        	while (engine->check_wall(x + coll_width + speed_x, y, z) && speed_x > 1)
                	speed_x--;
*/

    x += speed_x;
    counter += HS_SPEED;

    if (counter > 600) {
        return -1;
    }

    /*	if (!speed_x) {
        	engine->create_effect(new gnistor_o(x, y + height, z, direction));
		energy = 0;
        }*/
    // Check collision with walls
    if (engine->check_wall((direction == LEFT_DIR) ? x : x + coll_width, y, z)) {
        engine->create_effect(new gnistor_o(x, y + height, z, direction));
        if ((random() % 3) == 0) {
            engine->sound.PlaySFX("samples/ricochet.wav");
        }
        energy = 0;
    }

    return 0;
}
/****************************************************************************/
void HighSpeed_Bullet_o::Collision(Object* o)
{

    if ((o->GetWho() & enemies) != 0U) {
        energy = 0;
    }
}
/****************************************************************************/
HighSpeed_Bullet_o::~HighSpeed_Bullet_o()
    = default;
/****************************************************************************/
