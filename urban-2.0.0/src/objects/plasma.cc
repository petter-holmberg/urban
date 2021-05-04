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
#include <cstring>
/***************************************************************************/
inline constexpr auto PLASMA_SPEED = 10;

/***************************************************************************/
Plasma_o::Plasma_o(int X, int Y, int Z, int Dir, int friendship, int Damage)
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

    images = new BITMAP*;

    switch (Dir) {
    case LEFT_DIR:
        sprintf(filename, "plasmal/skottv.pcx");
        break;
    case RIGHT_DIR:
    default:
        sprintf(filename, "plasmal/skotth.pcx");
        break;
    }
    images[0] = icache.GetImage(filename, pal);
    if (images[0] != nullptr) {
        num_images++;
    }

    //	rect(images[0], 0, 0, images[0]->w - 1, images[0]->h - 1, 15);

    current_image = 0;
    me = FRIEND_PLASMA;
    friends = (friendship | me | FRIEND_DOOR | FRIEND_FIREBALL | FRIEND_ICEBALL | FRIEND_DEKOR) & ~ENEMY_SCIENTIST;
    enemies = ~friends;
    speed_x = (direction == RIGHT_DIR) ? PLASMA_SPEED : -PLASMA_SPEED;

    //        SOUND.PlaySFX(FIRE_SAMPLE);
}
/****************************************************************************/
auto Plasma_o::update() -> int
{

    if (energy == 0) {
        return -1;
    }
    /*        if (direction == LEFT_DIR)
        	while (ENGINE.check_wall(x + 1, y, z) && speed_x)
                	x++;
	else if (direction == RIGHT_DIR)
        	while (ENGINE.check_wall(x + coll_width - 1, y, z) && speed_x)
                	x--;
*/
    /*	if (direction == LEFT_DIR)
	        while (ENGINE.check_wall(x + speed_x, y, z) && speed_x < -1)
        	       	speed_x++;
	else if (direction == RIGHT_DIR)
        	while (ENGINE.check_wall(x + coll_width + speed_x, y, z) && speed_x > 1)
                	speed_x--;
*/

    x += speed_x;
    counter += PLASMA_SPEED;

    if (counter > 600) {
        return -1;
    }

    /*	if (!speed_x) {
        	ENGINE.create_effect(new gnistor_o(x, y + height, z, direction));
		energy = 0;
        }*/
    // Check collision with walls
    if (ENGINE.check_wall((direction == LEFT_DIR) ? x : x + coll_width, y, z)) {
        //	       	ENGINE.create_effect(new gnistor_o(x, y + height, z, direction));
        energy = 0;
    }

    return 0;
}
/****************************************************************************/
void Plasma_o::Collision(Object* o)
{

    if ((o->GetWho() & enemies) != 0U) {
        energy = 0;
    }
}
/****************************************************************************/
Plasma_o::~Plasma_o()
    = default;
/****************************************************************************/
