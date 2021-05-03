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
#include "object.h"
#include "death.h"

#define PROB_WALK_START 0
#define PROB_WALK_END   60
#define PROB_STOP_START 61
#define PROB_STOP_END   80
#define PROB_TURN_START 81
#define PROB_TURN_END   100
#define PROB_NUMBER	100

#define MAX_NUM			15
#define STATE_NONE		0
#define STATE_STOP		1
#define STATE_WALK		2
#define STATE_TURN		3
#define STATE_HIT		4
#define STATE_HIT_COUNTDOWN 	5
#define STATE_COLLAPS		6
#define STATE_DYING		7
#define STATE_WAITING		8
#define STATE_DEAD		9
#define STATE_WRITE		10
#define STATE_WRITING		11

#define MAX_X_SPEED	2 //4
#define X_FRICTION	1
#define MIN_X_SPEED	-MAX_X_SPEED //-4
#define MAX_Y_SPEED	16
#define MIN_Y_SPEED	-12
#define X_ACCEL		2 //2
#define Y_ACCEL		1

#define FRAME_DELAY 	2
#define WRITE_DELAY 	30


/**************************************************************************/
Einstein_o::Einstein_o(int X, int Y, int Z) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];
        int i;

        anim.reset();
//	images = (BITMAP **)malloc(30 * sizeof(BITMAP *));
	images = new BITMAP*[30];

        for (i = 0;i < 5;i++) {
		sprintf(filename, "prof2/prof2h/%d.pcx", i + 1);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 5;i < 10;i++) {
		sprintf(filename, "prof2/prof2v/%d.pcx", i - 4);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 10;i < 17;i++) {
        	sprintf(filename, "prof2/prof2h/dead%d.pcx", i - 9);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
        }
        for (i = 17;i < 24;i++) {
        	sprintf(filename, "prof2/prof2v/dead%d.pcx", i - 16);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
        }
        for (i = 24;i < 27;i++) {
        	sprintf(filename, "prof2/prof2h/write%d.pcx", i - 23);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
        }
        for (i = 27;i < 30;i++) {
        	sprintf(filename, "prof2/prof2v/write%d.pcx", i - 26);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
        }


        current_image = 5;

        height = images[0]->h;
        width = images[0]->w;
        coll_x = 0;
        coll_y = 0;
        coll_width = width;
        coll_height = height;

        //st� med f�tterna
        y -= images[0]->h;
        energy = 2;
        strength = 0;
        speed_x = 0;
        speed_y = 0;
        speed_z = 0;
	direction = LEFT_DIR;
        state = STATE_NONE;
        counter = 0;
        score = 100;
//        evil = 1;
	friends = ~FRIEND_PLAYER;
        enemies = ENEMY_PLAYER;
        me = FRIEND_SCIENTIST;
}
/**************************************************************************/
int Einstein_o::update() {
	int r;

        if (state == STATE_DEAD) {
        	SOUND.PlaySFX("samples/scream.wav");
        	return -1;
	}

        if (counter)
        	counter--;
	if (speed_x < 0)
        	speed_x += X_FRICTION;
	if (speed_x > 0)
        	speed_x -= X_FRICTION;

	if (counter == 0 && state != STATE_COLLAPS && state != STATE_WAITING && state != STATE_DYING) {
                	r = random() % PROB_NUMBER;
			if (r >= PROB_WALK_START && r <= PROB_WALK_END)
				state = STATE_WALK;
			else if ((r >= PROB_STOP_START && r <= PROB_STOP_END) || state == STATE_STOP)
				state = STATE_STOP;
			else if (r >= PROB_TURN_START && r <= PROB_TURN_END)
				state = STATE_TURN;
			counter = random() % MAX_NUM + 15;
        }

	switch (state) {
        	case STATE_WRITE:
                	state = STATE_WRITING;
                        anim.reset();
                        counter = 3 * WRITE_DELAY;
        	case STATE_WRITING:
	                current_image = direction == RIGHT_DIR ? 24 : 27 + anim.next_frame(2, WRITE_DELAY);
                        break;
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
					current_image = anim.next_frame(4, 5);
                                	break;
				case LEFT_DIR:
                                	if (speed_x > MIN_X_SPEED)
                                        	speed_x -= X_ACCEL;
					while (ENGINE.check_wall(x + speed_x, y + height, z) && speed_x)
	        	        		speed_x++;
					current_image = 5 + anim.next_frame(4, 5);
                                        break;
			};
                	break;
                case STATE_TURN:
                        direction = (direction == RIGHT_DIR ? LEFT_DIR : RIGHT_DIR);
                        speed_x = 0;
                        counter = 0;
                        state = STATE_WALK;
                        break;
                case STATE_STOP:
                	anim.reset();
                	speed_x = 0;
                	break;
/*		case STATE_HIT:
                	break;
		case STATE_HIT_COUNTDOWN:
                	break;*/
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

	if (!speed_x && state != STATE_COLLAPS && state != STATE_WAITING && state != STATE_DYING &&
		state != STATE_WRITE && state != STATE_WRITING) {
		current_image = direction == RIGHT_DIR ? 4 : 9;
                state = STATE_WRITE;
	}

	return 0;
}

void Einstein_o::Collision(Object *o) {
	int i;

	if (state == STATE_DEAD)
        	return;

        Object::Collision(o);

	if ((o->GetStrength() > 0) && (!(o->GetFriends() & me)) && state != STATE_DEAD && state != STATE_DYING) {
        	if (o->GetWho() & ENEMY_EXPLOSION) {
                	state = STATE_DEAD;
                        DEATH_BY_EXPLOSION;
                        score += 50;
                        return;
		} else if (o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM)) {
			ENGINE.create_effect(new BurningBody_o(x, y + height, z));
                        state = STATE_DEAD;
		} else if (o->GetWho() & ENEMY_ICEBALL) {
			ENGINE.create_object(new FrosenBody_o(x, y + height, z, direction));
                	state = STATE_DEAD;
		} else if (o->GetWho() & (ENEMY_HS_BULLET | ENEMY_PLASMA)) {
                	if (direction == RIGHT_DIR) {
				ENGINE.create_effect(new Animation_o(x, y + height, z, "prof2/prof2h/dead", 7, 6));
				for (i = 0;i < 7;i++)
				        ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z, - 2 + random() % 4));
			} else {
				ENGINE.create_effect(new Animation_o(x, y + height, z, "prof2/prof2v/dead", 7, 6));
				for (i = 0;i < 7;i++)
				        ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z, 2 + random() % 4));
			}
                        state = STATE_DEAD;
		}
	        counter = 0;
	} else {
//        	state = STATE_TURN;
	}
}

/**************************************************************************/
Einstein_o::~Einstein_o() {
}
/**************************************************************************/

/**************************************************************************/
Boss_Einstein_o::Boss_Einstein_o(int X, int Y, int Z) : Einstein_o(X, Y, Z) {
}


Boss_Einstein_o::~Boss_Einstein_o() {
}

int Boss_Einstein_o::update() {
	int ret;
	if ((ret = Einstein_o::update()) != 0)
        	ENGINE.create_object(new card_o(x, y, z));
	return ret;
}

