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
#include "ai.h"
#include "death.h"
#include "engine.h"
#include "object.h"
#include "object2.h"
#include <allegro.h>
#include <stdlib.h>
#include <string.h>

#define PROB_WALK_START 0
#define PROB_WALK_END 50
#define PROB_STOP_START 51
#define PROB_STOP_END 60
#define PROB_TURN_START 61
#define PROB_TURN_END 100
#define PROB_NUMBER 100

#define MAX_NUM 15
#define STATE_NONE 0
#define STATE_STOP 1
#define STATE_WALK 2
#define STATE_TURN 3
#define STATE_HIT 4
#define STATE_HIT_COUNTDOWN 5
#define STATE_FIRE 6
#define STATE_FIRING 7
#define STATE_ALARM 8
#define STATE_INTERLAYER 9

#define FIRE_RANGE 150
#define FIRE_STRENGTH 5
#define MAX_X_SPEED 2 //4
#define X_FRICTION 1
#define MIN_X_SPEED -MAX_X_SPEED //-4
#define MAX_Y_SPEED 16
#define MIN_Y_SPEED -12
#define X_ACCEL 2 //2
#define Y_ACCEL 1
#define Z_ACCEL 2
#define MAX_Z_SPEED 2
#define MIN_Z_SPEED -MAX_Z_SPEED
#define Z_FRICTION 1
//#define FRAME_DELAY 2
//#define FIRE_DELAY 30
//#define NUM_ROUNDS 6
//#define ALARM_DELAY 120

#define ALARM_DELAY 30
#define FRAME_DELAY 2
#define FIRE_DELAY 60
#define SHELL_DELAY 4
#define FIRING_DELAY 5
#define HIT_DELAY 10

#define COLL_X (coll_x + coll_width)
#define COLL_Y (coll_y + coll_height)

/*
20,0-42,57

*/
#define FOOT_LEFT 20
#define FOOT_WIDTH 22
#define FOOT_RIGHT (FOOT_LEFT + FOOT_WIDTH)

//#define NO_BOX_DISTANCE	20
//#define BOX_DISTANCE	50

/**************************************************************************/
soldier2_o::soldier2_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    RGB pal[256];
    char filename[512];
    int i;

    anim.reset();
    //	images = (BITMAP **)malloc(30 * sizeof(BITMAP *));
    images = new BITMAP*[26];

    for (i = 0; i < 5; i++) {
        sprintf(filename, "soldier2/v/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 5; i < 10; i++) {
        sprintf(filename, "soldier2/h/%d.pcx", i + 1 - 5);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 10; i < 15; i++) {
        sprintf(filename, "soldier2/f/%d.pcx", i + 1 - 10);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 15; i < 20; i++) {
        sprintf(filename, "soldier2/b/%d.pcx", i + 1 - 15);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 20; i < 22; i++) {
        sprintf(filename, "soldier2/v/shot%d.pcx", i + 1 - 20);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 22; i < 24; i++) {
        sprintf(filename, "soldier2/h/shot%d.pcx", i + 1 - 22);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    sprintf(filename, "soldier2/h/hit.pcx");
    images[24] = icache.GetImage(filename, pal);
    if (images[24])
        num_images++;

    sprintf(filename, "soldier2/v/hit.pcx");
    images[25] = icache.GetImage(filename, pal);
    if (images[25])
        num_images++;

    current_image = 1;

    height = images[0]->h;
    width = images[0]->w;
    /*        coll_x = 0;
        coll_y = 0;
        coll_width = width;
        coll_height = height;*/
    coll_x = 20;
    coll_y = 0;
    coll_width = 45;
    coll_height = height - 1;
    //        for (i = 0;i < num_images;i++)
    //		rect(images[i], coll_x, coll_y, COLL_X, COLL_Y, 15);

    /*

coll_x = 33
coll_y = 0
coll_width = 64
coll_height = height
*/

    //stå med fötterna
    y -= images[0]->h;
    energy = 20;
    strength = 10;
    speed_x = 0;
    speed_y = 0;
    speed_z = 0;
    direction = RIGHT_DIR;
    state = STATE_NONE;
    counter = 0;
    score = 500;
    friends = ~FRIEND_PLAYER;
    enemies = ENEMY_PLAYER;
    me = FRIEND_SOLDIER2;
}

int soldier2_o::update()
{
    int r;

    if (energy <= 0)
        return -1;

    if (counter)
        counter--;

    if (!counter && state != STATE_FIRING) {
        r = random() % PROB_NUMBER;
        if (r >= PROB_WALK_START && r <= PROB_WALK_END) {
            state = STATE_WALK;
            direction = random() % 4; //lotta en riktning
        } else if ((r >= PROB_STOP_START && r <= PROB_STOP_END) || state == STATE_STOP || state == STATE_TURN)
            state = STATE_STOP;
        else if (r >= PROB_TURN_START && r <= PROB_TURN_END)
            state = STATE_TURN;
        counter = random() % MAX_NUM + 15;
    }

    if ((state != STATE_FIRE && state != STATE_FIRING && state != STATE_TURN && state != STATE_ALARM && state != STATE_HIT && state != STATE_HIT_COUNTDOWN))
        if (PLAYER->GetX() < x && PLAYER->GetX() > x - FIRE_RANGE && (direction == LEFT_DIR || direction == UP_DIR || direction == DOWN_DIR) && PLAYER->GetLayer() == layer) {
            state = STATE_FIRE;
        } else if (PLAYER->GetX() > x && PLAYER->GetX() < x + FIRE_RANGE && (direction == RIGHT_DIR || direction == UP_DIR || direction == DOWN_DIR) && PLAYER->GetLayer() == layer) {
            state = STATE_FIRE;
        }

    if (!speed_y)
        switch (state) {
        case STATE_WALK:
            switch (direction) {
            case RIGHT_DIR:
                if (speed_x < MAX_X_SPEED)
                    speed_x += X_ACCEL;

                while (ENGINE.check_wall(x + COLL_X + speed_x, y + COLL_Y, z) && speed_x)
                    speed_x--;

                current_image = 5 + anim.next_frame(4, 5);
                speed_z = 0;
                break;
            case LEFT_DIR:
                if (speed_x > MIN_X_SPEED)
                    speed_x -= X_ACCEL;

                while (ENGINE.check_wall(x + coll_x + speed_x, y + COLL_Y, z) && speed_x)
                    speed_x++;

                current_image = anim.next_frame(4, 5);
                speed_z = 0;
                break;
            case UP_DIR:
                if (speed_z > MIN_Z_SPEED)
                    speed_z -= Z_ACCEL;
                while ((ENGINE.check_wall(x + coll_x, y + COLL_Y, z + speed_z - 5) || ENGINE.check_wall(x + COLL_X, y + COLL_Y, z + speed_z - 5)) && speed_z)
                    speed_z++;
                current_image = 15 + anim.next_frame(4, 5);
                speed_x = 0;
                break;
            case DOWN_DIR:
                if (speed_z < MAX_Z_SPEED)
                    speed_z += Z_ACCEL;
                while ((ENGINE.check_wall(x + coll_x, y + COLL_Y, z + speed_z + 5) || ENGINE.check_wall(x + COLL_X, y + COLL_Y, z + speed_z + 5)) && speed_z)
                    speed_z--;
                current_image = 10 + anim.next_frame(4, 5);
                speed_x = 0;
                break;
            default:
                direction = LEFT_DIR;
                break;
            }
            break;
        case STATE_STOP:
            speed_x = 0;
            speed_z = 0;
            break;
        case STATE_NONE:
            if (direction == RIGHT_DIR || direction == LEFT_DIR)
                current_image = (direction == RIGHT_DIR ? 4 : 9);
            else
                current_image = (direction == UP_DIR ? 14 : 19);
            counter = 0;
            anim.reset();
            break;
        case STATE_ALARM:
            /*				if (current_image != 22 && current_image != 20) {
		                	counter = ALARM_DELAY;
        		                current_image = direction == RIGHT_DIR ? 22 : 20;
                		        speed_x = 0;
                                        speed_z = 0;
				}*/
            break;
        case STATE_FIRE:
            if (PLAYER->GetX() < x)
                direction = LEFT_DIR;
            else
                direction = RIGHT_DIR;
            counter = FIRE_DELAY;
            state = STATE_FIRING;
            anim.reset();
            speed_x = 0;
            speed_z = 0;
            counter2 = 0;
            /* fallthrough */
        case STATE_FIRING:
            switch (counter2) {
            case 0:
                current_image = (direction == RIGHT_DIR ? 22 : 20) + (r = anim.next_frame(1, ALARM_DELAY));
                if (r == 0)
                    break;
                counter2 = 1;
                anim.reset();
            case 1:
            case 2:
                current_image = (direction == RIGHT_DIR ? 23 : 21) + (r = anim.next_frame(1, FIRING_DELAY));
                if (r == 0 && counter2 == 1) {
                    if (direction == RIGHT_DIR)
                        ENGINE.create_object(new grenade_o(x + width + 5, y - 10, z, 4, -(2 + random() % 5)));
                    else
                        ENGINE.create_object(new grenade_o(x - 5, y - 10, z, -4, -(2 + random() % 5)));

                    ENGINE.create_object(new shells_o(x, y, z, "soldier2/", 1));
                    counter2 = 2;
                }
                if (r == 0)
                    break;
                counter2 = 3;
                anim.reset();
            case 3:
                current_image = (direction == RIGHT_DIR ? 22 : 20) + (r = anim.next_frame(1, ALARM_DELAY));
                if (r == 0)
                    break;
            default:
                state = STATE_TURN;
            }
            if (state != STATE_TURN)
                break;
        case STATE_TURN:
            if (direction == RIGHT_DIR || direction == LEFT_DIR) {
                direction = (direction == RIGHT_DIR ? LEFT_DIR : RIGHT_DIR);
                speed_x = 0;
                counter = random() % MAX_NUM + 15;
                state = STATE_WALK;
            }
            break;
        case STATE_HIT:
            if (direction == UP_DIR || direction == DOWN_DIR)
                direction = PLAYER->GetX() < x ? LEFT_DIR : RIGHT_DIR;
            state = STATE_HIT_COUNTDOWN;
            counter = HIT_DELAY;
            /* fall through */
        case STATE_HIT_COUNTDOWN:
            current_image = direction == RIGHT_DIR ? 24 : 25;
            break;
        default:
            break;
        }

    if (!speed_y)
        switch (direction) {
        case LEFT_DIR:
            if (!ENGINE.check_floor(x + FOOT_LEFT + speed_x, y + height, z) && !ENGINE.check_floor(x + FOOT_LEFT + speed_x, y + height + TILE_SIDE_HEIGHT, z)) {
                speed_x = 0;
                state = STATE_TURN;
            }
            break;
        case RIGHT_DIR:
            if (!ENGINE.check_floor(x + FOOT_RIGHT + speed_x, y + height, z) && !ENGINE.check_floor(x + FOOT_RIGHT + speed_x, y + height + TILE_SIDE_HEIGHT, z)) {
                speed_x = 0;
                state = STATE_TURN;
            }
            break;
        case UP_DIR:
            if (!ENGINE.check_floor(x + FOOT_RIGHT, y + height, z + speed_z) && !ENGINE.check_floor(x + FOOT_RIGHT, y + height + TILE_SIDE_HEIGHT, z + speed_z)) {
                speed_z = 0;
                state = STATE_TURN;
            }
            break;
        case DOWN_DIR:
            if (!ENGINE.check_floor(x + FOOT_RIGHT, y + height, z + speed_z) && !ENGINE.check_floor(x + FOOT_RIGHT, y + height + TILE_SIDE_HEIGHT, z + speed_z)) {
                speed_z = 0;
                state = STATE_TURN;
            }
            break;
        default:
            break;
        }

    // Fall or Stop
    if (ENGINE.check_floor(x + FOOT_LEFT, y + height, z) || ENGINE.check_floor(x + FOOT_RIGHT, y + height, z)) {
        speed_y = 0;
    } else if (speed_y < MAX_Y_SPEED) {
        speed_y += Y_ACCEL;
        speed_z = 0;
    }

    x += speed_x;
    y += speed_y;
    z += speed_z;

    if (x < 0)
        x = 0;

    if (z > MIN_Z) {
        z = MIN_Z;
        speed_z = 0;
        state = STATE_STOP;
        counter = 0;
    }
    if (z < MAX_Z) {
        z = MAX_Z;
        speed_z = 0;
        state = STATE_STOP;
        counter = 0;
    }

    layer = z / TILE_TOP_HEIGHT;

    if (!speed_x && !speed_z && state != STATE_HIT_COUNTDOWN && state != STATE_FIRING && state != STATE_ALARM) {
        if (direction == LEFT_DIR || direction == RIGHT_DIR)
            current_image = (direction == RIGHT_DIR ? 4 : 9);
        else if (direction == UP_DIR || direction == DOWN_DIR)
            current_image = (direction == UP_DIR ? 14 : 19);
    }
    return 0;
}

void soldier2_o::Collision(Object* o)
{
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
                    ENGINE.create_effect(new Animation_o(x, y + height, z, "soldier2/h/dead", 4, 3));
            }
            if (!(o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)))
                for (i = 0; i < 3; i++)
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
                    ENGINE.create_effect(new Animation_o(x, y + height, z, "soldier2/v/dead", 4, 3));
            }
            if (!(o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)))
                for (i = 0; i < 3; i++)
                    ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
        }
        //                state = STATE_HIT;
        //                direction = o->GetDirection();
    }
}

/**************************************************************************/
soldier2_o::~soldier2_o()
{
}
/**************************************************************************/

/**************************************************************************/
Boss_soldier2_o::Boss_soldier2_o(int X, int Y, int Z)
    : soldier2_o(X, Y, Z)
{
    score = 1500;
    energy = 30;
}

Boss_soldier2_o::~Boss_soldier2_o()
{
}

int Boss_soldier2_o::update()
{
    int ret;
    if ((ret = soldier2_o::update()) == -1)
        ENGINE.create_object(new card_o(x, y, z));
    return ret;
}
