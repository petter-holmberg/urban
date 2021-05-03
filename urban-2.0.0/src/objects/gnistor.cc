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
#include <allegro.h>
#include "object.h"
#include "engine.h"

gnistor_o::gnistor_o(int X, int Y, int Z, int Speed_X, int Speed_Y, int Speed_Z, int Direction) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];
        int i;

        anim.reset();
	images = new BITMAP*[3];

        for (i = 0;i < 3;i++) {
		sprintf(filename, "gnist/%s/%d.pcx", Direction == LEFT_DIR ? "v":"h", i + 1);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        height = images[0]->h;
        width = images[0]->w;

        current_image = 0;
        //st� med f�tterna
        y -= (height / 2);

        x -= (width / 2);
        coll_x = 0;
        coll_y = 0;
        coll_width = width;
        coll_height = height;

        energy = 0;
        strength = 10;
        speed_x = 0;
        speed_y = 0;
        score = 0;
        friends = ~0;
	enemies = 0;
        counter = 0;
	me = 3;
}

gnistor_o::~gnistor_o() {
}

int gnistor_o::update() {
	counter++;
	current_image = anim.next_frame(3, 3);

        if (current_image >= 3)
        	return -1;
	return 0;
}

void gnistor_o::Collision(Object *) {
}
