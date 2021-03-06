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

#define PROB_WALK_START 0
#define PROB_WALK_END   80
#define PROB_TURN_START 81
#define PROB_TURN_END   100
#define PROB_NUMBER	100
#define MAX_NUM		10

#define MAX_X_SPEED	1 //4
#define X_FRICTION	1
#define MIN_X_SPEED	-MAX_X_SPEED //-4
#define MAX_Y_SPEED	16
#define MIN_Y_SPEED	-12
#define X_ACCEL		2 //2
#define Y_ACCEL		1

#define STATE_WALK	0
#define STATE_TURN	1
#define STATE_DEAD	2
#define STATE_NONE	3
#define STATE_BURN	4
#define STATE_BURNING	5

#define FRAME_DELAY 2
#define BURN_DELAY	300

/**************************************************************************/
BurningBody_o::BurningBody_o(int X, int Y, int Z, int Move) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];
        int i;

        anim.reset();
	images = new BITMAP*[12];

        for (i = 0;i < 4;i++) {
		sprintf(filename, "burning/burnh/%d.pcx", i + 1);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 4;i < 8;i++) {
		sprintf(filename, "burning/burnv/%d.pcx", i - 3);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 8;i < 11;i++) {
		sprintf(filename, "burning/%d.pcx", i - 7);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}

        current_image = Move ? 0 : 10;

	coll_x = 0;
        coll_y = 0;
        coll_width = width = images[0]->w;
        coll_height = height = images[0]->h;

        //stå med fötterna
        y -= images[0]->h;
        friends = ~0;
        enemies = 0;
        energy = 1;
        strength = 0;
        speed_x = 0;
        speed_y = 0;
	direction = RIGHT_DIR;
        state = STATE_NONE;
        counter = 0;
        counter2 = BURN_DELAY;
        counter3 = 0;
}
/**************************************************************************/
int BurningBody_o::update() {
	int r;

        if (counter)
        	counter--;

	if (counter2)
        	counter2--;

	if (speed_x < 0)
        	speed_x += X_FRICTION;

	if (speed_x > 0)
        	speed_x -= X_FRICTION;

	if (counter == 0) {
        	if (state == STATE_BURNING)
                	state = STATE_DEAD;
		else {
	               	r = random() % PROB_NUMBER;
			if (r >= PROB_WALK_START && r <= PROB_WALK_END)
				state = STATE_WALK;
			else if (r >= PROB_TURN_START && r <= PROB_TURN_END)
				state = STATE_TURN;
			counter = random() % MAX_NUM + 15;
		}
        }

        if (!counter2 && state != STATE_DEAD && state != STATE_BURNING) {
        	state = STATE_BURN;
	}

	switch (state) {
		case STATE_NONE:
                	current_image = (direction == RIGHT_DIR ? 5 : 0);
                        counter = 0;
                        anim.reset();
                	break;
                case STATE_WALK:
                	switch (direction) {
                        	case RIGHT_DIR:
                                	if (speed_x < MAX_X_SPEED)
                                        	speed_x += X_ACCEL;
					while (ENGINE.check_wall(x + width + speed_x, y + height, z) && speed_x)
        		        		speed_x--;
					current_image = anim.next_frame(3, 5);
                                	break;
				case LEFT_DIR:
                                	if (speed_x > MIN_X_SPEED)
                                        	speed_x -= X_ACCEL;
					while (ENGINE.check_wall(x + speed_x, y + height, z) && speed_x)
	        	        		speed_x++;
					current_image = 4 + anim.next_frame(3, 5);
                                        break;
			};
                	break;
                case STATE_TURN:
                        direction = (direction == RIGHT_DIR ? LEFT_DIR : RIGHT_DIR);
                        speed_x = 0;
                        counter = 0;
                        state = STATE_WALK;
                        break;
		case STATE_BURN:
                       	counter = 150;
                        state = STATE_BURNING;
                        anim.reset();
		/* fall through*/
		case STATE_BURNING:
                	current_image = 8 + anim.next_frame(1, 5);
                        break;
		case STATE_DEAD:
                	current_image = 10;
                        ENGINE.create_dekoration(this);
			return REMOVE_ME;
                	break;
		default:
                	break;
       	};

        if (!speed_y)
	        if (!ENGINE.check_floor(x + speed_x, y + height, z) || !ENGINE.check_floor(x + width + speed_x, y + height, z)) {
			speed_x = 0;
        	}

        // Fall or Stop
	if (ENGINE.check_floor(x + 14, y + height, z) || ENGINE.check_floor(x + width - 14, y + height, z)) {
        	speed_y = 0;
        } else if (speed_y < MAX_Y_SPEED) {
        	speed_y += Y_ACCEL;
        }


        x += speed_x;
        y += speed_y;
        if (x < 0)
        	x = 0;

	if (!speed_x && state != STATE_BURNING)
        	state = STATE_TURN;

	if (energy <= 0)
        	return -1;

	return 0;
}

/**************************************************************************/
BurningBody_o::~BurningBody_o() {
}
/**************************************************************************/

