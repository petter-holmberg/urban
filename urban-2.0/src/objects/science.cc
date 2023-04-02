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
#include "allegro.h"
#include "death.h"
#include "engine.h"
#include "object.h"

inline constexpr auto PROB_WALK_START = 0;
inline constexpr auto PROB_WALK_END = 40;
inline constexpr auto PROB_STOP_START = 41;
inline constexpr auto PROB_STOP_END = 80;
inline constexpr auto PROB_TURN_START = 81;
inline constexpr auto PROB_TURN_END = 100;
inline constexpr auto PROB_NUMBER = 100;

inline constexpr auto MAX_NUM = 15;
inline constexpr auto STATE_NONE = 0;
inline constexpr auto STATE_STOP = 1;
inline constexpr auto STATE_WALK = 2;
inline constexpr auto STATE_TURN = 3;
inline constexpr auto STATE_HIT = 4;
inline constexpr auto STATE_HIT_COUNTDOWN = 5;
inline constexpr auto STATE_COLLAPS = 6;
inline constexpr auto STATE_DYING = 7;
inline constexpr auto STATE_WAITING = 8;
inline constexpr auto STATE_DEAD = 9;
inline constexpr auto STATE_WRITE = 10;
inline constexpr auto STATE_WRITING = 11;

inline constexpr auto MAX_X_SPEED = 2;
inline constexpr auto X_FRICTION = 1;
inline constexpr auto MIN_X_SPEED = (-MAX_X_SPEED);
inline constexpr auto MAX_Y_SPEED = 16;
inline constexpr auto MIN_Y_SPEED = -12;
inline constexpr auto X_ACCEL = 2;
inline constexpr auto Y_ACCEL = 1;

inline constexpr auto FRAME_DELAY = 2;
inline constexpr auto WRITE_DELAY = 30;

/**************************************************************************/
scientist_o::scientist_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    anim.reset();
    images.resize(40, nullptr);

    for (i = 0; i < 5; i++) {
        sprintf(filename, "profh/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 5; i < 10; i++) {
        sprintf(filename, "profv/%d.pcx", i - 4);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 10; i < 22; i++) {
        sprintf(filename, "profh/dead%d.pcx", i - 9);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 22; i < 34; i++) {
        sprintf(filename, "profv/dead%d.pcx", i - 21);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 34; i < 37; i++) {
        sprintf(filename, "profh/write%d.pcx", i - 33);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 37; i < 40; i++) {
        sprintf(filename, "profv/write%d.pcx", i - 36);
        images[i] = icache.GetImage(filename, pal);
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
auto scientist_o::update() -> int
{
    int r = 0;

    if (state == STATE_DEAD) {
        engine->sound.PlaySFX("samples/scream.wav");
        return -1;
    }

    if (counter != 0) {
        counter--;
    }
    if (speed_x < 0) {
        speed_x += X_FRICTION;
    }
    if (speed_x > 0) {
        speed_x -= X_FRICTION;
    }

    if (counter == 0 && state != STATE_COLLAPS && state != STATE_WAITING && state != STATE_DYING) {
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

    switch (state) {
    case STATE_WRITE:
        state = STATE_WRITING;
        anim.reset();
        counter = 3 * WRITE_DELAY;
        [[fallthrough]];
    case STATE_WRITING:
        current_image = direction == RIGHT_DIR ? 34 : 37 + anim.next_frame(2, WRITE_DELAY);
        break;
    case STATE_COLLAPS:
        speed_x = speed_z = 0;
        direction = (engine->player->GetX() > x) ? RIGHT_DIR : LEFT_DIR;
        current_image = (direction == RIGHT_DIR ? 10 : 22) + (r = anim.next_frame(4, 5));
        //                	current_image = ((engine->player->GetX() > x) ? 10 : 22) + (r = anim.next_frame(4, 5));
        if (r == 4) {
            state = STATE_WAITING;
        } else {
            break;
        }
        anim.reset();
    case STATE_WAITING:
        speed_x = speed_z = 0;
        current_image = (direction == RIGHT_DIR ? 14 : 26) + anim.next_frame(1, 10);
        break;
    case STATE_DYING:
        if (direction == -100) {
            direction = (engine->player->GetX() > x) ? RIGHT_DIR : LEFT_DIR;
        }
        //                	current_image = ((engine->player->GetX() > x) ? 15 : 27) + (r = anim.next_frame(6, 5));
        current_image = (direction == RIGHT_DIR ? 15 : 27) + (r = anim.next_frame(6, 5));
        if (r == 6) {
            engine->create_dekoration(this);
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

    if ((speed_x == 0) && state != STATE_COLLAPS && state != STATE_WAITING && state != STATE_DYING && state != STATE_WRITE && state != STATE_WRITING) {
        current_image = direction == RIGHT_DIR ? 4 : 9;
        state = STATE_WRITE;
    }

    return 0;
}

void scientist_o::Collision(Object* o)
{
    int i = 0;

    //        if (!energy)
    //        	return;
    if (state == STATE_DEAD) {
        return;
    }

    Object::Collision(o);

    if ((o->GetStrength() > 0) && ((o->GetFriends() & me) == 0U) && state != STATE_DEAD && state != STATE_DYING) {
        if ((o->GetWho() & ENEMY_EXPLOSION) != 0) {
            state = STATE_DEAD;
            score += 50;
            return;
        }
        if ((o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM)) != 0) {
            engine->create_effect(new BurningBody_o(x, y + height, z));
            state = STATE_DEAD;
        } else if ((o->GetWho() & ENEMY_ICEBALL) != 0) {
            engine->create_object(new FrosenBody_o(x, y + height, z, direction));
            state = STATE_DEAD;
        } else if ((o->GetWho() & (ENEMY_HS_BULLET | ENEMY_PLASMA)) != 0) {
            if (state == STATE_COLLAPS) {
                state = STATE_DEAD;
                // DEATH_BY_EXPLOSION2
                {
                    int r = random() % 3;
                    int i = 0;
                    for (i = 0; i < 10; i++) {
                        engine->create_effect(new blood_o(x + width / 2, y, z, random() % 8 + 2, -(random() % 4) - 2));
                    }
                    for (i = 0; i < 10; i++) {
                        engine->create_effect(new blood_o(x + width / 2, y, z, -(random() % 8) - 2, -(random() % 4) - 2));
                    }
                    switch (r) {
                    case 0:
                        engine->create_effect(new bodyparts_o(x, y, z, "meat/arm", 5, -4, -6, 0));
                        engine->create_effect(new bodyparts_o(x, y, z, "meat/arm", 5, 4, -6, 0));
                        engine->create_effect(new bodyparts_o(x, y, z, "meat/head", 5, 2, -4, 0));
                        engine->create_effect(new bodyparts_o(x, y, z, "meat/leg", 5, -1, -2, 0));
                        engine->create_effect(new bodyparts_o(x, y, z, "meat/leg", 5, 3, -4, 0));
                        engine->create_effect(new bodyparts_o(x, y, z, "meat/ribs", 5, 0, 0, 0));
                        for (i = 0; i < 5; i++) {
                            engine->create_effect(new bodyparts_o(x, y, z, "meat/meat", 3, random() % 5, -(random() % 6), 0));
                        }
                        break;
                    case 1:
                        engine->create_effect(new bodyparts_o(x, y, z, "meat/head", 5, 2, -4, 0));
                        engine->create_effect(new bodyparts_o(x, y, z, "meat/ribs", 5, 0, 0, 0));
                        for (i = 0; i < 5; i++) {
                            engine->create_effect(new bodyparts_o(x, y, z, "meat/meat", 3, random() % 4, -(random() % 4) - 2, 0));
                        }
                        for (i = 0; i < 5; i++) {
                            engine->create_effect(new bodyparts_o(x, y, z, "meat/meat", 3, -(random() % 4), -(random() % 4) - 2, 0));
                        }
                        break;
                    case 2:
                        for (i = 0; i < 3; i++) {
                            engine->create_effect(new bodyparts_o(x, y, z, "meat/bigmeat", 5, random() % 5, -(random() % 4) - 3, 0));
                        }
                        for (i = 0; i < 3; i++) {
                            engine->create_effect(new bodyparts_o(x, y, z, "meat/bigmeat", 5, -(random() % 5), -(random() % 4) - 3, 0));
                        }
                        break;
                    case 3:
                    default:
                        break;
                    }
                }
                return;
            }
            if (state == STATE_WAITING) {
                direction = -100;
                state = STATE_DYING;
                anim.reset();
                score += 100;
                return;
            }
            if (direction == RIGHT_DIR) {
                for (i = 0; i < 7; i++) {
                    engine->create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
                }
            } else {
                for (i = 0; i < 7; i++) {
                    engine->create_effect(new blood_o(x + width / 2, y + random() % height, z, 2 + random() % 4));
                }
            }
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
    = default;
/**************************************************************************/

/**************************************************************************/
Boss_scientist_o::Boss_scientist_o(int X, int Y, int Z)
    : scientist_o(X, Y, Z)
{
}

Boss_scientist_o::~Boss_scientist_o()
    = default;

auto Boss_scientist_o::update() -> int
{
    int ret = 0;
    if ((ret = scientist_o::update()) != 0) {
        engine->create_object(new card_o(x, y, z));
    }
    return ret;
}
