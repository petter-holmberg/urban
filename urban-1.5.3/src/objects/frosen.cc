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
#include <string.h>
#include <stdlib.h>
#include <allegro.h>
#include "engine.h"
#include "object.h"

#define STATE_FROSEN	0
#define STATE_CRACKING	1
#define STATE_CRUSHED	2

#define FRAME_DELAY 2

/**************************************************************************/
FrosenBody_o::FrosenBody_o(int X, int Y, int Z, int dir) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];
//        int i;


        images = new BITMAP*;

        if (dir == LEFT_DIR)
	        sprintf(filename, "iced/v/6.pcx");
	else
        	sprintf(filename, "iced/h/6.pcx");

	images[0] = icache.GetImage(filename, pal);
        if (images[0])
        	num_images++;

	current_image = 0;

	coll_x = 0;
        coll_y = 0;
        coll_width = width = images[0]->w;
        coll_height = height = images[0]->h;

        //stå med fötterna
        y -= images[0]->h;
        friends = 0;
        enemies = ~friends;
        energy = 1;
        strength = 0;
        speed_x = 0;
        speed_y = 0;
	direction = dir;
        me = FRIEND_FROZENBODY;
        state = STATE_FROSEN;
        score = 200;
}
/**************************************************************************/
int FrosenBody_o::update() {
	if (energy <= 0) {
        	if (direction == LEFT_DIR)
	        	ENGINE.create_effect(new Animation_o(x, y + height, z, "iced/v/", 5, 4));
                else
	        	ENGINE.create_effect(new Animation_o(x, y + height, z, "iced/h/", 5, 4));
//Animation_o(int X, int Y, int Z, char *name, int num_pics, int dek_frame = -1, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0);
//		else
        	return -1;
        }
//ENGINE.create_effect(new Animation_o(x, y + height, z, "soldier2\\v\\dead", 4, 3));
//	if (energy <= 0)
//        	return -1;

	return 0;
}

/**************************************************************************/
FrosenBody_o::~FrosenBody_o() {
}
/**************************************************************************/

void FrosenBody_o::Collision(Object *o) {
	if ((o->GetWho() == ENEMY_HS_BULLET) ||
		(o->GetWho() == ENEMY_GRENADE))
        	energy = 0;
}
