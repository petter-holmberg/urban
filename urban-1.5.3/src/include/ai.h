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

//olika define's för ai:n i spelet
#ifndef __AI_H__
#define __AI_H__

/* Kollar hål för fötterna och stannar om hålet är för djupt */

#define INIT_HOLECHECK \
switch (direction) {

#define HOLE_LEFT(new_state) \
case LEFT_DIR: \
	if (!ENGINE.check_floor(x + FOOT_LEFT + speed_x, y + height, z) && \
		!ENGINE.check_floor(x + FOOT_LEFT + speed_x, y + height + TILE_SIDE_HEIGHT, z)) { \
		speed_x = 0; \
		state = new_state; \
	} \
	break;

#define HOLE_RIGHT(new_state) \
case RIGHT_DIR: \
	if (!ENGINE.check_floor(x + FOOT_LEFT + speed_x, y + height, z) && \
		!ENGINE.check_floor(x + FOOT_LEFT + speed_x, y + height + TILE_SIDE_HEIGHT, z)) { \
		speed_x = 0; \
		state = new_state; \
	} \
	break;


#define HOLE_UP(new_state) \
case UP_DIR: \
	if (!ENGINE.check_floor(x + FOOT_RIGHT, y + height, z + speed_z) && \
		!ENGINE.check_floor(x + FOOT_RIGHT, y + height + TILE_SIDE_HEIGHT, z + speed_z)) { \
		speed_z = 0; \
		state = new_state; \
	} \
	break;

#define HOLE_DOWN(new_state) \
case DOWN_DIR: \
	if (!ENGINE.check_floor(x + FOOT_RIGHT, y + height, z + speed_z) && \
		!ENGINE.check_floor(x + FOOT_RIGHT, y + height + TILE_SIDE_HEIGHT, z + speed_z)) { \
		speed_z = 0; \
		state = new_state; \
	} \
	break;


#define END_HOLECHECK \
	default: \
       		break; \
}


/* går igenom state-saken*/

#define INIT_DO_STATE \
switch (state) {

#define INIT_STATE_WALK \
case STATE_WALK: \
switch (direction) {

#define DO_WALK_LEFT \
case LEFT_DIR: \
	if (speed_x > MIN_X_SPEED) \
		speed_x -= X_ACCEL; \
                                    \
	while (ENGINE.check_wall(x + coll_x + speed_x, y + COLL_Y, z) && speed_x) \
		speed_x++; \
                           \
	current_image = anim.next_frame(4, 5); \
	speed_z = 0; \
	break;

#define DO_WALK_RIGHT \
case RIGHT_DIR: \
	if (speed_x < MAX_X_SPEED) \
		speed_x += X_ACCEL; \
                                     \
	while (ENGINE.check_wall(x + COLL_X + speed_x, y + COLL_Y, z) && speed_x) \
		speed_x--; \
                            \
	current_image = 5 + anim.next_frame(4, 5); \
	speed_z = 0; \
	break;

#define DO_WALK_UP \
case UP_DIR: \
	if (speed_z > MIN_Z_SPEED) \
		speed_z -= Z_ACCEL; \
	while ((ENGINE.check_wall(x + coll_x, y + COLL_Y, z + speed_z) || \
		ENGINE.check_wall(x + COLL_X, y + COLL_Y, z + speed_z)) && \
		speed_z) \
		speed_z++; \
	current_image = 15 + anim.next_frame(4, 5); \
	speed_x = 0; \
	break;

#define DO_WALK_DOWN \
case DOWN_DIR: \
	if (speed_z < MAX_Z_SPEED) \
		speed_z += Z_ACCEL; \
	while ((ENGINE.check_wall(x + coll_x, y + COLL_Y, z + speed_z) || \
		ENGINE.check_wall(x + COLL_X, y + COLL_Y, z + speed_z)) && \
		speed_z) \
		speed_z--; \
	current_image = 10 + anim.next_frame(4, 5); \
	speed_x = 0; \
	break;


#define END_STATE_WALK \
}

#define DO_STOP \
case STATE_STOP: \
	speed_x = 0; \
	speed_z = 0; \
	break;

#define DO_TURN \
case STATE_TURN: \
	if (direction == RIGHT_DIR || direction == LEFT_DIR) { \
		direction = (direction == RIGHT_DIR ? LEFT_DIR : RIGHT_DIR); \
		speed_x = 0; \
		counter = random() % MAX_NUM + 15; \
		state = STATE_WALK; \
	} \
	break;

#define DO_NONE \
case STATE_NONE: \
	if (direction == RIGHT_DIR || direction == LEFT_DIR) \
		current_image = (direction == RIGHT_DIR ? 4 : 9); \
	else \
		current_image = (direction == UP_DIR ? 14 : 19); \
	counter = 0; \
	anim.reset(); \
	break;


#define END_DO_STATE \
	default: \
        	break; \
}

#endif
