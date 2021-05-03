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
#include <string.h>
#include <stdlib.h>
#include <allegro.h>
#include "engine.h"
#include "object2.h"


Dekor_o::Dekor_o(int X, int Y, int Z, char *filename) : Object(X, Y, Z) {
	RGB pal[256];

        images = new BITMAP *;


        images[0] = icache.GetImage(filename, pal);

        if (images[0])
        	num_images++;

	current_image = 0;

        height = images[0]->h;
        width = images[0]->w;
	x += TILE_WIDTH / 2;
	x -= width / 2;
        y -= height;
        y += (TILE_TOP_HEIGHT);
        coll_x = 0;
        coll_y = 0;
        coll_width = width;
        coll_height = height;

        energy = 1;
        strength = 0;
        speed_x = 0;

        speed_y = 5;
        speed_z = 0;

        me = FRIEND_DEKOR;
	friends = 0;
        enemies = 0;
}
/****************************************************************************/
Dekor_o::~Dekor_o() {
}

/****************************************************************************/
int Dekor_o::update() {
       // Fall or Stop
//	while (ENGINE.check_floor(x, y + height + speed_y, z) && speed_y)
//		speed_y--;

//	y += speed_y;

	return 0;
}

void Dekor_o::Collision(Object *o) {
}
