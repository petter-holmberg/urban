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
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include "object.h"
#include "engine.h"

//#define X_FRICTION	1
//#define Z_FRICTION	X_FRICTION
//#define MAX_X_SPEED	2 //4
//#define MIN_X_SPEED	-MAX_X_SPEED //-4
#define MAX_Y_SPEED	8
//#define MIN_Y_SPEED	-12
//#define	MAX_Z_SPEED	MAX_X_SPEED
//#define MIN_Z_SPEED	-MAX_Z_SPEED
//#define X_ACCEL		2 //2
#define Y_ACCEL		1
//#define Z_ACCEL         2

blood_o::blood_o(int X, int Y, int Z, int Speed_X, int Speed_Y, int Speed_Z) : Object(X, Y, Z) {
	int i;
        RGB pal[256];
	char filename[512];

        images = (BITMAP **)malloc(3 * sizeof(BITMAP *));

        for (i = 0;i < 3;i++) {
	        sprintf(filename, "dead/blod%d.pcx", i + 1);
        	images[i] = icache.GetImage(filename, pal);
		if (images[i])
        		num_images++;
	}

	coll_x = 0;
        coll_y = 0;
        coll_width = width = images[0]->w;
        coll_height = height = images[0]->h;
        x = X;
        y = Y;
        z = Z;
        speed_x = Speed_X;
        speed_y = Speed_Y;
        speed_z = Speed_Z;

        if (Speed_X)
	        current_image = 0;
	else
        	current_image = random() % 2 + 1;
        direction = Speed_X > 0 ? RIGHT_DIR : LEFT_DIR;
        energy = 0;
        strength = 0;
        me = 1;
}

blood_o::~blood_o() {
}

int blood_o::update() {


//	x += speed_x;

        if (ENGINE.check_floor(x, y + height, z)
        	|| ENGINE.check_floor(x + width, y + height, z))
                	speed_y = 0;
	else if (speed_y < MAX_Y_SPEED)
        	speed_y += Y_ACCEL;



//	if (speed_y)
	x += speed_x;
        y += speed_y;

	if (ENGINE.check_floor(x, y + height, z)
		&& ENGINE.check_floor(x + images[1]->w, y + height, z)) {
        	ENGINE.create_dekoration(new blood_o(x, y, z, 0, 0, 0));
        	return -1;
	}
	return 0;
}


