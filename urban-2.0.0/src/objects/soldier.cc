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
#include "death.h"

#define PROB_WALK_START 0
#define PROB_WALK_END   40
#define PROB_STOP_START 41
#define PROB_STOP_END   70
#define PROB_TURN_START 71
#define PROB_TURN_END   100
#define PROB_NUMBER	100

#define MAX_NUM			15
#define STATE_NONE		0
#define STATE_STOP		1
#define STATE_WALK		2
#define STATE_TURN		3
#define STATE_HIT		4
#define STATE_HIT_COUNTDOWN 	5
#define STATE_FIRE		6
#define STATE_FIRING		7
#define STATE_ALARM		8

#define FIRE_RANGE		100

#define MAX_X_SPEED	2 //4
#define X_FRICTION	1
#define MIN_X_SPEED	-MAX_X_SPEED //-4
#define MAX_Y_SPEED	16
#define MIN_Y_SPEED	-12
#define X_ACCEL		2 //2
#define Y_ACCEL		1

#define FRAME_DELAY 2
#define FIRE_DELAY 4
#define NUM_ROUNDS 6
#define ALARM_DELAY 120

/**************************************************************************/
soldier_o::soldier_o(int X, int Y, int Z) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];
        int i;

        anim.reset();
	images = (BITMAP **)malloc(30 * sizeof(BITMAP *));

        for (i = 0;i < 5;i++) {
		sprintf(filename, "soldier/soldh/%d.pcx", i + 1);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 5;i < 8;i++) {
		sprintf(filename, "soldier/soldh/shot%d.pcx", i - 4);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}

        for (i = 8;i < 13;i++) {
		sprintf(filename, "soldier/soldv/%d.pcx", i - 7);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}

        for (i = 13;i < 16;i++) {
		sprintf(filename, "soldier/soldv/shot%d.pcx", i - 12);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}

        current_image = 4;

        height = images[0]->h;
        width = images[0]->w;
        coll_x = 20;
        coll_y = 0;
        coll_width = 30;
        coll_height = height;

        //stå med fötterna
        y -= images[0]->h;
        energy = 5;
        strength = 3;
        speed_x = 0;
        speed_y = 0;
	direction = RIGHT_DIR;
        state = STATE_NONE;
        counter = 0;
        score = 300;

	friends = ~FRIEND_PLAYER;
        enemies = ENEMY_PLAYER;
        me = FRIEND_SOLDIER;
}
/**************************************************************************/
int soldier_o::update() {
	int r;
        if (counter)
        	counter--;
	if (speed_x < 0)
        	speed_x += X_FRICTION;
	if (speed_x > 0)
        	speed_x -= X_FRICTION;

	if (counter == 0) {

        	if (state == STATE_FIRE || state == STATE_FIRING)
                	state = STATE_ALARM;
		else if (state == STATE_HIT) {
                	direction = PLAYER->GetX() < x ? LEFT_DIR : RIGHT_DIR;
			state = STATE_FIRE;
		} else {
                	r = random() % PROB_NUMBER;
			if (r >= PROB_WALK_START && r <= PROB_WALK_END)
				state = STATE_WALK;
			else if ((r >= PROB_STOP_START && r <= PROB_STOP_END) || state == STATE_STOP)
				state = STATE_STOP;
			else if (r >= PROB_TURN_START && r <= PROB_TURN_END)
				state = STATE_TURN;
			counter = random() % MAX_NUM + 15;
                }
        }

	if (state != STATE_FIRE && state != STATE_ALARM && state != STATE_FIRING) {
	        if (PLAYER->GetX() < x && PLAYER->GetX() > x - FIRE_RANGE
        		&& direction == LEFT_DIR && PLAYER->GetLayer() == layer) {
	        	state = STATE_FIRE;
		} else if(PLAYER->GetX() > x && PLAYER->GetX() < x + FIRE_RANGE
			&& direction == RIGHT_DIR && PLAYER->GetLayer() == layer) {
        		state = STATE_FIRE;
		}
	}

	switch (state) {
        	case STATE_FIRE:
                	counter = NUM_ROUNDS * FIRE_DELAY * 3;
                        state = STATE_FIRING;
                        anim.reset();
                        speed_x = 0;
                        counter2 = 0;
                        /* fallthrough */
		case STATE_FIRING:
                	current_image = (direction == RIGHT_DIR ? 5 : 13) + (r = anim.next_frame(2, FIRE_DELAY));
                        if (r == 1 && !counter2) {

                               	ENGINE.create_object(new HighSpeed_Bullet_o(x, y + 24, z , direction, ~FRIEND_PLAYER));
                                counter2 = 1;
			}
                        if (r == 2 && counter2 == 1) {
				if (direction == RIGHT_DIR) {
                                	ENGINE.create_effect(new shells_o(x + 36, y + 24, z, "soldier/hylsa", 1, -(random() % 5), -(random() % 8), 0));
                	        } else {
	                              	ENGINE.create_effect(new shells_o(x + 28, y + 24, z, "soldier/hylsa", 1, random() % 5, -(random() % 8), 0));
                        	}
                                counter2 = 0;
			}
                        break;
                case STATE_ALARM:
                	if (counter == 0) {
	                	current_image = (direction == RIGHT_DIR ? 5 : 13);
        	                counter = ALARM_DELAY;
			}
                        break;
		case STATE_NONE:
                	current_image = (direction == RIGHT_DIR ? 4 : 12);
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
					current_image = 8 + anim.next_frame(4, 5);
                                        break;
			};
                	break;
                case STATE_TURN:
                        direction = (direction == RIGHT_DIR ? LEFT_DIR : RIGHT_DIR);
                        speed_x = 0;
                        counter = 0;
                        break;
                case STATE_STOP:
                	speed_x = 0;
                	break;
		case STATE_HIT:
                	break;
		case STATE_HIT_COUNTDOWN:
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

	if (!speed_x && state != STATE_FIRE && state != STATE_FIRING
		&& state != STATE_ALARM)
		current_image = (direction == RIGHT_DIR ? 4 : 12);

	if (energy <= 0)
        	return -1;

	return 0;
}

void soldier_o::Collision(Object *o) {
	int i;

        if (!energy)
        	return;

        Object::Collision(o);

	if ((o->GetStrength() > 0) && (!(o->GetFriends() & me))) {
       		if (direction == RIGHT_DIR) {
               		if (energy <= 0) {
		               	if (o->GetWho() & ENEMY_EXPLOSION)
					DEATH_BY_EXPLOSION
				else if (o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM))
					ENGINE.create_effect(new BurningBody_o(x, y + height, z));
				else if (o->GetWho() & ENEMY_ICEBALL)
					ENGINE.create_object(new FrosenBody_o(x, y + height, z, direction));
				else
					ENGINE.create_effect(new Animation_o(x, y + height, z, "soldier/deadh/dead", 4, 3));
			}
                        if (!(o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)))
				for (i = 0;i < 3;i++)
				        ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
		} else {
       	        	if (energy <= 0) {
		               	if (o->GetWho() & ENEMY_EXPLOSION)
					DEATH_BY_EXPLOSION
	                        else if (o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM))
					ENGINE.create_effect(new BurningBody_o(x, y + height, z));
				else if (o->GetWho() & ENEMY_ICEBALL)
					ENGINE.create_object(new FrosenBody_o(x, y + height, z, direction));
				else
					ENGINE.create_effect(new Animation_o(x, y + height, z, "soldier/deadv/dead", 4, 3));
			}
                        if (!(o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)))
				for (i = 0;i < 3;i++)
				        ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
		}
		state = STATE_HIT;
	        counter = 0;
	}
}

/**************************************************************************/
soldier_o::~soldier_o() {
}
/**************************************************************************/


/**************************************************************************/
//Boss-soldat
/**************************************************************************/
Boss_soldier_o::Boss_soldier_o(int X, int Y, int Z) : soldier_o(X, Y, Z) {
}


Boss_soldier_o::~Boss_soldier_o() {
}

int Boss_soldier_o::update() {
	int ret;
	if ((ret = soldier_o::update()) == -1)
        	ENGINE.create_object(new card_o(x, y, z));
	return ret;
}

