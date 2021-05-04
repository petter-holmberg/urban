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
#include "engine.h"
#include "object.h"
#include <allegro.h>
#include <cstdlib>
#include <cstring>

#define PROB_WALK_START 0
#define PROB_WALK_END 80
#define PROB_TURN_START 81
#define PROB_TURN_END 100
#define PROB_NUMBER 100

#define MAX_NUM 15

#define STATE_NONE 0
#define STATE_WALK 1
#define STATE_TURN 2

#define MAX_X_SPEED 1 //4
#define X_FRICTION 1
#define MIN_X_SPEED (-MAX_X_SPEED) //-4
#define MAX_Y_SPEED 16
#define MIN_Y_SPEED (-12)
#define X_ACCEL 2 //2
#define Y_ACCEL 1

#define FRAME_DELAY 5

/**************************************************************************/
Vaktm_o::Vaktm_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    RGB pal[256];
    char filename[512];
    int i = 0;

    anim.reset();
    images = new BITMAP*[8];

    for (i = 0; i < 4; i++) {
        sprintf(filename, "vaktm/v/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
        if (images[i] != nullptr) {
            num_images++;
        }
    }
    for (i = 4; i < 8; i++) {
        sprintf(filename, "vaktm/h/%d.pcx", i - 3);
        images[i] = icache.GetImage(filename, pal);
        if (images[i] != nullptr) {
            num_images++;
        }
    }

    current_image = 0;

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    //st� med f�tterna
    y -= images[0]->h;
    energy = 1;
    strength = 0;
    speed_x = 0;
    speed_y = 0;
    direction = LEFT_DIR;
    state = STATE_NONE;
    counter = 0;
    score = 50;
    friends = ~FRIEND_PLAYER;
    enemies = ENEMY_PLAYER;
    me = FRIEND_VAKTM;
}
/**************************************************************************/
auto Vaktm_o::update() -> int
{
    int r = 0;
    if (counter != 0) {
        counter--;
    }
    if (speed_x < 0) {
        speed_x += X_FRICTION;
    }
    if (speed_x > 0) {
        speed_x -= X_FRICTION;
    }

    if (counter == 0) {
        r = random() % PROB_NUMBER;
        if (r >= PROB_WALK_START && r <= PROB_WALK_END) {
            state = STATE_WALK;
        } else if (r >= PROB_TURN_START && r <= PROB_TURN_END) {
            state = STATE_TURN;
        }
        counter = random() % MAX_NUM + 15;
    }

    switch (state) {
    case STATE_NONE:
        current_image = (direction == RIGHT_DIR ? 4 : 0);
        counter = 0;
        anim.reset();
        break;
    case STATE_WALK:
        switch (direction) {
        case RIGHT_DIR:
            if (speed_x < MAX_X_SPEED) {
                speed_x += X_ACCEL;
            }
            while (ENGINE.check_wall(x + width + speed_x, y + height, z) && (speed_x != 0)) {
                speed_x--;
            }
            current_image = 4 + anim.next_frame(3, 5);
            break;
        case LEFT_DIR:
            if (speed_x > MIN_X_SPEED) {
                speed_x -= X_ACCEL;
            }
            while (ENGINE.check_wall(x + speed_x, y + height, z) && (speed_x != 0)) {
                speed_x++;
            }
            current_image = anim.next_frame(3, 5);
            break;
        };
        break;
    case STATE_TURN:
        direction = (direction == RIGHT_DIR ? LEFT_DIR : RIGHT_DIR);
        speed_x = 0;
        counter = 0;
        state = STATE_WALK;
        break;
    default:
        break;
    };

    if (speed_y == 0) {
        if (!ENGINE.check_floor(x + speed_x, y + height, z) || !ENGINE.check_floor(x + width + speed_x, y + height, z)) {
            speed_x = 0;
        }
    }

    // Fall or Stop
    if (ENGINE.check_floor(x + 14, y + height, z) || ENGINE.check_floor(x + width - 14, y + height, z)) {
        speed_y = 0;
    } else if (speed_y < MAX_Y_SPEED) {
        speed_y += Y_ACCEL;
    }

    x += speed_x;
    y += speed_y;
    if (x < 0) {
        x = 0;
    }

    if (energy <= 0) {
        return -1;
    }

    return 0;
}

void Vaktm_o::Collision(Object* o)
{
    if (energy == 0) {
        return;
    }

    Object::Collision(o);

    if ((o->GetStrength() > 0) && ((o->GetFriends() & me) == 0U)) {
        ENGINE.create_object(new explosion_o(x, y + height - 75, z));
        ENGINE.create_object(new explosion_o(x + 15, y + height - 50, z));
        ENGINE.create_object(new explosion_o(x - 15, y + height - 50, z));
        ENGINE.create_effect(new BurningBody_o(x, y + height, z));
        counter = 0;
        energy = 0;
    }
}

/**************************************************************************/
Vaktm_o::~Vaktm_o()
    = default;
/**************************************************************************/
