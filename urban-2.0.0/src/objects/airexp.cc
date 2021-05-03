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
#include "object2.h"
#include "engine.h"


#define EXPLODE_SAMPLE	"samples/ex12.wav"
#define EXPLODE_SPEED	4
#define EXPLODE_DAMAGE	30
#define EXPLODE_FRAMES	4

AirExplosion_o::AirExplosion_o(int X, int Y, int Z, int Speed_X, int Speed_Y, int Speed_Z) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];
        int i;

        anim.reset();
        images = new BITMAP*[10];

        for (i = 0;i < 10;i++) {
		sprintf(filename, "expl/%d.pcx", i + 1);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        height = images[0]->h;
        width = images[0]->w;

        current_image = 0;
        y -= (height / 2);

        x -= (width / 2);
        coll_x = 0;
        coll_y = 0;
        coll_width = width;
        coll_height = height;

        energy = 1;
        strength = EXPLODE_DAMAGE;
        speed_x = 0;
        speed_y = 0;
        score = 0;
        friends = FRIEND_EXPLOSION;
	enemies = ~friends;
        counter = EXPLODE_FRAMES;
//        counter2 = random() % 10;
	me = FRIEND_EXPLOSION;
	ENGINE.EnableEarthquake();
//        SOUND.PlaySFX(EXPLODE_SAMPLE);
}

AirExplosion_o::~AirExplosion_o() {
}

int AirExplosion_o::update() {
//        enemies = 0;

	if (counter)
		counter--;

        if (!counter)
        	strength = 0;
/*
        if(counter2 == 0) {

        	counter2 = 30;
                strength = 9;
        } else {

        	strength = 0;
                counter2--;
        }*/
	current_image = anim.next_frame(10, EXPLODE_SPEED);


/*        if (current_image > 5)
                strength = 0;*/
        if (current_image >= 10)
        	return -1;
	return 0;
}

void AirExplosion_o::Collision(Object *o) {
/*	if (((o->GetWho() & enemies))) {

        	counter2 = 100;
        }*/
}