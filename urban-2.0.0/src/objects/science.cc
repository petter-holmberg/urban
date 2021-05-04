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
#include "death.h"
#include "engine.h"
#include "object.h"
#include <allegro.h>
#include <stdlib.h>
#include <string.h>

#define PROB_WALK_START 0
#define PROB_WALK_END 40
#define PROB_STOP_START 41
#define PROB_STOP_END 80
#define PROB_TURN_START 81
#define PROB_TURN_END 100
#define PROB_NUMBER 100

#define MAX_NUM 15
#define STATE_NONE 0
#define STATE_STOP 1
#define STATE_WALK 2
#define STATE_TURN 3
#define STATE_HIT 4
#define STATE_HIT_COUNTDOWN 5
#define STATE_COLLAPS 6
#define STATE_DYING 7
#define STATE_WAITING 8
#define STATE_DEAD 9
#define STATE_WRITE 10
#define STATE_WRITING 11

#define MAX_X_SPEED 2 //4
#define X_FRICTION 1
#define MIN_X_SPEED -MAX_X_SPEED //-4
#define MAX_Y_SPEED 16
#define MIN_Y_SPEED -12
#define X_ACCEL 2 //2
#define Y_ACCEL 1

#define FRAME_DELAY 2
#define WRITE_DELAY 30
#define DEATH_BY_EXPLOSION2                                                                                                 \
    {                                                                                                                       \
        int r = random() % 3;                                                                                               \
        int i;                                                                                                              \
        for (i = 0; i < 10; i++)                                                                                            \
            ENGINE.create_effect(new blood_o(x + width / 2, y, z, random() % 8 + 2, -(random() % 4) - 2));                  \
        for (i = 0; i < 10; i++)                                                                                            \
            ENGINE.create_effect(new blood_o(x + width / 2, y, z, -(random() % 8) - 2, -(random() % 4) - 2));               \
        switch (r) {                                                                                                        \
        case 0:                                                                                                             \
            ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/arm", 5, -4, -6, 0));                                       \
            ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/arm", 5, 4, -6, 0));                                        \
            ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/head", 5, 2, -4, 0));                                       \
            ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/leg", 5, -1, -2, 0));                                       \
            ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/leg", 5, 3, -4, 0));                                        \
            ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/ribs", 5, 0, 0, 0));                                        \
            for (i = 0; i < 5; i++)                                                                                         \
                ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/meat", 3, random() % 5, -(random() % 6), 0));           \
            break;                                                                                                          \
        case 1:                                                                                                             \
            ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/head", 5, 2, -4, 0));                                       \
            ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/ribs", 5, 0, 0, 0));                                        \
            for (i = 0; i < 5; i++)                                                                                         \
                ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/meat", 3, random() % 4, -(random() % 4) - 2, 0));       \
            for (i = 0; i < 5; i++)                                                                                         \
                ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/meat", 3, -(random() % 4), -(random() % 4) - 2, 0));    \
            break;                                                                                                          \
        case 2:                                                                                                             \
            for (i = 0; i < 3; i++)                                                                                         \
                ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/bigmeat", 5, random() % 5, -(random() % 4) - 3, 0));    \
            for (i = 0; i < 3; i++)                                                                                         \
                ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/bigmeat", 5, -(random() % 5), -(random() % 4) - 3, 0)); \
            break;                                                                                                          \
        case 3:                                                                                                             \
        default:                                                                                                            \
            break;                                                                                                          \
        }                                                                                                                   \
    }

/*
#define DEATH_BY_EXPLOSION \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/arm", 5, -4, -6, 0)); \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/arm", 5, 4, -6, 0)); \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/head", 5, 2, -4, 0)); \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/leg", 5, -1, -2, 0));  \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/leg", 5, 3, -4, 0));    \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/ribs", 5, 0, 0, 0));     \
for (i = 0;i < 5;i++)                                                         \
	ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/meat", 3, random() % 5, -(random() % 6), 0));
*/

/**************************************************************************/
scientist_o::scientist_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    RGB pal[256];
    char filename[512];
    int i;

    anim.reset();
    //	images = (BITMAP **)malloc(30 * sizeof(BITMAP *));
    images = new BITMAP*[40];

    for (i = 0; i < 5; i++) {
        sprintf(filename, "profh/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 5; i < 10; i++) {
        sprintf(filename, "profv/%d.pcx", i - 4);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 10; i < 22; i++) {
        sprintf(filename, "profh/dead%d.pcx", i - 9);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 22; i < 34; i++) {
        sprintf(filename, "profv/dead%d.pcx", i - 21);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 34; i < 37; i++) {
        sprintf(filename, "profh/write%d.pcx", i - 33);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }
    for (i = 37; i < 40; i++) {
        sprintf(filename, "profv/write%d.pcx", i - 36);
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

    //stå med fötterna
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
int scientist_o::update()
{
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
        current_image = direction == RIGHT_DIR ? 34 : 37 + anim.next_frame(2, WRITE_DELAY);
        break;
    case STATE_COLLAPS:
        speed_x = speed_z = 0;
        direction = (PLAYER->GetX() > x) ? RIGHT_DIR : LEFT_DIR;
        current_image = (direction == RIGHT_DIR ? 10 : 22) + (r = anim.next_frame(4, 5));
        //                	current_image = ((PLAYER->GetX() > x) ? 10 : 22) + (r = anim.next_frame(4, 5));
        if (r == 4)
            state = STATE_WAITING;
        else
            break;
        anim.reset();
    case STATE_WAITING:
        speed_x = speed_z = 0;
        current_image = (direction == RIGHT_DIR ? 14 : 26) + anim.next_frame(1, 10);
        break;
    case STATE_DYING:
        if (direction == -100)
            direction = (PLAYER->GetX() > x) ? RIGHT_DIR : LEFT_DIR;
        //                	current_image = ((PLAYER->GetX() > x) ? 15 : 27) + (r = anim.next_frame(6, 5));
        current_image = (direction == RIGHT_DIR ? 15 : 27) + (r = anim.next_frame(6, 5));
        if (r == 6) {
            ENGINE.create_dekoration(this);
            return REMOVE_ME;
        }
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

    if (!speed_x && state != STATE_COLLAPS && state != STATE_WAITING && state != STATE_DYING && state != STATE_WRITE && state != STATE_WRITING) {
        current_image = direction == RIGHT_DIR ? 4 : 9;
        state = STATE_WRITE;
    }

    return 0;
}

void scientist_o::Collision(Object* o)
{
    int i;

    //        if (!energy)
    //        	return;
    if (state == STATE_DEAD)
        return;

    Object::Collision(o);

    if ((o->GetStrength() > 0) && (!(o->GetFriends() & me)) && state != STATE_DEAD && state != STATE_DYING) {
        if (o->GetWho() & ENEMY_EXPLOSION) {
            state = STATE_DEAD;
            //                        DEATH_BY_EXPLOSION;
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
            if (state == STATE_COLLAPS) {
                state = STATE_DEAD;
                DEATH_BY_EXPLOSION2
                return;
            } else if (state == STATE_WAITING) {
                direction = -100;
                state = STATE_DYING;
                anim.reset();
                score += 100;
                return;
            }
            if (direction == RIGHT_DIR)
                for (i = 0; i < 7; i++)
                    ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
            else
                for (i = 0; i < 7; i++)
                    ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z, 2 + random() % 4));

            if (state != STATE_COLLAPS) {
                state = STATE_COLLAPS;
                score += 50;
                anim.reset();
            }
        }
        counter = 0;
    } else {
        //        	state = STATE_TURN;
    }
}

/**************************************************************************/
scientist_o::~scientist_o()
{
}
/**************************************************************************/

/**************************************************************************/
Boss_scientist_o::Boss_scientist_o(int X, int Y, int Z)
    : scientist_o(X, Y, Z)
{
}

Boss_scientist_o::~Boss_scientist_o()
{
}

int Boss_scientist_o::update()
{
    int ret;
    if ((ret = scientist_o::update()) != 0)
        ENGINE.create_object(new card_o(x, y, z));
    return ret;
}
