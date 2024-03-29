/******************************************************************************
    URBAN 2.0
    Copyright (C) 2021  Petter Holmberg (petter.holmberg@usa.net)

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
*****************************************************************************/
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
#include "allegro.h"
#include "death.h"
#include "engine.h"
#include "object.h"
#include <vector>

inline constexpr auto PROB_WALK_START = 0;
inline constexpr auto PROB_WALK_END = 40;
inline constexpr auto PROB_STOP_START = 41;
inline constexpr auto PROB_STOP_END = 70;
inline constexpr auto PROB_TURN_START = 71;
inline constexpr auto PROB_TURN_END = 100;
inline constexpr auto PROB_NUMBER = 100;

inline constexpr auto MAX_NUM = 15;
inline constexpr auto STATE_NONE = 0;
inline constexpr auto STATE_STOP = 1;
inline constexpr auto STATE_WALK = 2;
inline constexpr auto STATE_TURN = 3;
inline constexpr auto STATE_HIT = 4;
inline constexpr auto STATE_HIT_COUNTDOWN = 5;
inline constexpr auto STATE_FIRE = 6;
inline constexpr auto STATE_FIRING = 7;
inline constexpr auto STATE_ALARM = 8;

inline constexpr auto FIRE_RANGE = 100;

inline constexpr auto MAX_X_SPEED = 2;
inline constexpr auto X_FRICTION = 1;
inline constexpr auto MIN_X_SPEED = -MAX_X_SPEED;
inline constexpr auto MAX_Y_SPEED = 16;
inline constexpr auto MIN_Y_SPEED = -12;
inline constexpr auto X_ACCEL = 2;
inline constexpr auto Y_ACCEL = 1;

inline constexpr auto FRAME_DELAY = 2;
inline constexpr auto FIRE_DELAY = 4;
inline constexpr auto NUM_ROUNDS = 6;
inline constexpr auto ALARM_DELAY = 120;

/**************************************************************************/
soldier_o::soldier_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    anim.reset();
    images.resize(30, nullptr);

    for (i = 0; i < 5; i++) {
        sprintf(filename, "soldier/soldh/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 5; i < 8; i++) {
        sprintf(filename, "soldier/soldh/shot%d.pcx", i - 4);
        images[i] = icache.GetImage(filename, pal);
    }

    for (i = 8; i < 13; i++) {
        sprintf(filename, "soldier/soldv/%d.pcx", i - 7);
        images[i] = icache.GetImage(filename, pal);
    }

    for (i = 13; i < 16; i++) {
        sprintf(filename, "soldier/soldv/shot%d.pcx", i - 12);
        images[i] = icache.GetImage(filename, pal);
    }

    current_image = 4;

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 20;
    coll_y = 0;
    coll_width = 30;
    coll_height = height;

    //st� med f�tterna
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
auto soldier_o::update() -> int
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

        if (state == STATE_FIRE || state == STATE_FIRING) {
            state = STATE_ALARM;
        } else if (state == STATE_HIT) {
            direction = engine->player->GetX() < x ? LEFT_DIR : RIGHT_DIR;
            state = STATE_FIRE;
        } else {
            r = random() % PROB_NUMBER;
            if (r >= PROB_WALK_START && r <= PROB_WALK_END) {
                state = STATE_WALK;
            } else if ((r >= PROB_STOP_START && r <= PROB_STOP_END) || state == STATE_STOP) {
                state = STATE_STOP;
            } else if (r >= PROB_TURN_START && r <= PROB_TURN_END) {
                state = STATE_TURN;
            }
            counter = random() % MAX_NUM + 15;
        }
    }

    if (state != STATE_FIRE && state != STATE_ALARM && state != STATE_FIRING) {
        if (engine->player->GetX() < x && engine->player->GetX() > x - FIRE_RANGE
            && direction == LEFT_DIR && engine->player->GetLayer() == layer) {
            state = STATE_FIRE;
        } else if (engine->player->GetX() > x && engine->player->GetX() < x + FIRE_RANGE
            && direction == RIGHT_DIR && engine->player->GetLayer() == layer) {
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
        if (r == 1 && (counter2 == 0)) {

            engine->create_object(new HighSpeed_Bullet_o(x, y + 24, z, direction, ~FRIEND_PLAYER));
            counter2 = 1;
        }
        if (r == 2 && counter2 == 1) {
            if (direction == RIGHT_DIR) {
                engine->create_effect(new shells_o(x + 36, y + 24, z, "soldier/hylsa", 1, -(random() % 5), -(random() % 8), 0));
            } else {
                engine->create_effect(new shells_o(x + 28, y + 24, z, "soldier/hylsa", 1, random() % 5, -(random() % 8), 0));
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
            if (speed_x < MAX_X_SPEED) {
                speed_x += X_ACCEL;
            }
            while (engine->check_wall(x + width + speed_x, y + height, z) && (speed_x != 0)) {
                speed_x--;
            }
            current_image = anim.next_frame(4, 5);
            break;
        case LEFT_DIR:
            if (speed_x > MIN_X_SPEED) {
                speed_x -= X_ACCEL;
            }
            while (engine->check_wall(x + speed_x, y + height, z) && (speed_x != 0)) {
                speed_x++;
            }
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

    if (speed_y == 0) {
        if (!engine->check_floor(x + speed_x, y + height, z) || !engine->check_floor(x + width + speed_x, y + height, z)) {
            speed_x = 0;
        }
    }

    // Fall or Stop
    if (engine->check_floor(x + 14, y + height, z) || engine->check_floor(x + width - 14, y + height, z)) {
        speed_y = 0;
    } else if (speed_y < MAX_Y_SPEED) {
        speed_y += Y_ACCEL;
    }

    x += speed_x;
    y += speed_y;
    if (x < 0) {
        x = 0;
    }

    if ((speed_x == 0) && state != STATE_FIRE && state != STATE_FIRING
        && state != STATE_ALARM) {
        current_image = (direction == RIGHT_DIR ? 4 : 12);
    }

    if (energy <= 0) {
        return -1;
    }

    return 0;
}

void soldier_o::Collision(Object* o)
{
    int i = 0;

    if (energy == 0) {
        return;
    }

    Object::Collision(o);

    if ((o->GetStrength() > 0) && ((o->GetFriends() & me) == 0U)) {
        if (direction == RIGHT_DIR) {
            if (energy <= 0) {
                if ((o->GetWho() & ENEMY_EXPLOSION) != 0) {
                    DEATH_BY_EXPLOSION(x, width, y, height, z);
                } else if ((o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM)) != 0) {
                    engine->create_effect(new BurningBody_o(x, y + height, z));
                } else if ((o->GetWho() & ENEMY_ICEBALL) != 0) {
                    engine->create_object(new FrosenBody_o(x, y + height, z, direction));
                } else {
                    engine->create_effect(new Animation_o(x, y + height, z, "soldier/deadh/dead", 4, 3));
                }
            }
            if ((o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)) == 0) {
                for (i = 0; i < 3; i++) {
                    engine->create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
                }
            }
        } else {
            if (energy <= 0) {
                if ((o->GetWho() & ENEMY_EXPLOSION) != 0) {
                    DEATH_BY_EXPLOSION(x, width, y, height, z);
                } else if ((o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM)) != 0) {
                    engine->create_effect(new BurningBody_o(x, y + height, z));
                } else if ((o->GetWho() & ENEMY_ICEBALL) != 0) {
                    engine->create_object(new FrosenBody_o(x, y + height, z, direction));
                } else {
                    engine->create_effect(new Animation_o(x, y + height, z, "soldier/deadv/dead", 4, 3));
                }
            }
            if ((o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)) == 0) {
                for (i = 0; i < 3; i++) {
                    engine->create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
                }
            }
        }
        state = STATE_HIT;
        counter = 0;
    }
}

/**************************************************************************/
soldier_o::~soldier_o()
    = default;
/**************************************************************************/

/**************************************************************************/
//Boss-soldat
/**************************************************************************/
Boss_soldier_o::Boss_soldier_o(int X, int Y, int Z)
    : soldier_o(X, Y, Z)
{
}

Boss_soldier_o::~Boss_soldier_o()
    = default;

auto Boss_soldier_o::update() -> int
{
    int ret = 0;
    if ((ret = soldier_o::update()) == -1) {
        engine->create_object(new card_o(x, y, z));
    }
    return ret;
}
