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
inline constexpr auto PROB_WALK_END = 50;
inline constexpr auto PROB_STOP_START = 51;
inline constexpr auto PROB_STOP_END = 60;
inline constexpr auto PROB_TURN_START = 61;
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
inline constexpr auto STATE_WAIT = 8;
inline constexpr auto STATE_WAITING = 9;
inline constexpr auto STATE_DEAD = 10;
inline constexpr auto STATE_COLLAPS = 11;
inline constexpr auto STATE_DYING = 12;

inline constexpr auto FIRE_RANGE = 150;
inline constexpr auto FIRE_STRENGTH = 20;
inline constexpr auto MAX_X_SPEED = 2;
inline constexpr auto X_FRICTION = 1;
inline constexpr auto MIN_X_SPEED = -MAX_X_SPEED;
inline constexpr auto MAX_Y_SPEED = 16;
inline constexpr auto MIN_Y_SPEED = -12;
inline constexpr auto X_ACCEL = 2;
inline constexpr auto Y_ACCEL = 1;
inline constexpr auto Z_ACCEL = 2;
inline constexpr auto MAX_Z_SPEED = 2;
inline constexpr auto MIN_Z_SPEED = -MAX_Z_SPEED;
inline constexpr auto Z_FRICTION = 1;

inline constexpr auto ALARM_DELAY = 5;
inline constexpr auto FRAME_DELAY = 2;
inline constexpr auto FIRE_DELAY = 60;
inline constexpr auto SHELL_DELAY = 4;
inline constexpr auto FIRING_DELAY = 3;
inline constexpr auto HIT_DELAY = 10;
inline constexpr auto JUMP_DELAY = 20;
inline constexpr auto LAND_DELAY = 10;

inline constexpr auto FOOT_LEFT = 39;
inline constexpr auto FOOT_WIDTH = 17;
inline constexpr auto FOOT_RIGHT = FOOT_LEFT + FOOT_WIDTH;

inline constexpr auto NO_BOX_DISTANCE = 20;
inline constexpr auto BOX_DISTANCE = 50;

/**************************************************************************/
Crycheck_o::Crycheck_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    anim.reset();
    images.resize(42, nullptr);

    for (i = 0; i < 5; i++) {
        sprintf(filename, "inspecto/inspv/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 5; i < 10; i++) {
        sprintf(filename, "inspecto/insph/%d.pcx", i + 1 - 5);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 10; i < 14; i++) {
        sprintf(filename, "inspecto/inspv/shot%d.pcx", i + 1 - 10);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 14; i < 18; i++) {
        sprintf(filename, "inspecto/insph/shot%d.pcx", i + 1 - 14);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 18; i < 30; i++) {
        sprintf(filename, "inspecto/insph/dead%d.pcx", i + 1 - 18);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 30; i < 42; i++) {
        sprintf(filename, "inspecto/inspv/dead%d.pcx", i + 1 - 30);
        images[i] = icache.GetImage(filename, pal);
    }

    current_image = 1;

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
    speed_z = 0;
    direction = RIGHT_DIR;
    state = STATE_NONE;
    counter = 0;
    score = 500;
    friends = ~FRIEND_PLAYER;
    enemies = ENEMY_PLAYER;
    me = FRIEND_AGENT;
}

auto Crycheck_o::update() -> int
{
    int r = 0;

    if (state == STATE_DEAD) {
        return -1;
    }

    if (counter != 0) {
        counter--;
    }

    if ((counter == 0) && state != STATE_COLLAPS && state != STATE_WAITING && state != STATE_DYING && state != STATE_FIRING) {
        r = random() % PROB_NUMBER;
        if (r >= PROB_WALK_START && r <= PROB_WALK_END) {
            state = STATE_WALK;
            direction = random() % 4; //lotta en riktning
        } else if ((r >= PROB_STOP_START && r <= PROB_STOP_END) || state == STATE_STOP || state == STATE_TURN) {
            state = STATE_STOP;
        } else if (r >= PROB_TURN_START && r <= PROB_TURN_END) {
            state = STATE_TURN;
        }
        counter = random() % MAX_NUM + 15;
    }

    if ((state != STATE_FIRE && state != STATE_FIRING && state != STATE_TURN && state != STATE_COLLAPS && state != STATE_DYING && state != STATE_WAITING)) {
        if (engine->player->GetX() < x && engine->player->GetX() > x - FIRE_RANGE && (direction == LEFT_DIR || direction == UP_DIR || direction == DOWN_DIR) && engine->player->GetLayer() == layer) {
            state = STATE_FIRE;
        } else if (engine->player->GetX() > x && engine->player->GetX() < x + FIRE_RANGE && (direction == RIGHT_DIR || direction == UP_DIR || direction == DOWN_DIR) && engine->player->GetLayer() == layer) {
            state = STATE_FIRE;
        }
    }

    if (speed_y == 0) {
        switch (state) {
        case STATE_COLLAPS:
            direction = (engine->player->GetX() > x) ? RIGHT_DIR : LEFT_DIR;
            speed_x = speed_z = 0;
            current_image = (direction == RIGHT_DIR ? 18 : 30) + (r = anim.next_frame(4, 5));
            if (r == 4) {
                state = STATE_WAITING;
            } else {
                break;
            }
            anim.reset();
        case STATE_WAITING:
            speed_x = speed_z = 0;
            current_image = (direction == RIGHT_DIR ? 22 : 34) + anim.next_frame(1, 10);
            break;
        case STATE_DYING:
            if (direction == -100) {
                direction = (engine->player->GetX() > x) ? RIGHT_DIR : LEFT_DIR;
            }
            current_image = (direction == RIGHT_DIR ? 23 : 35) + (r = anim.next_frame(6, 5));
            if (r == 6) {
                engine->create_dekoration(this);
                return REMOVE_ME;
            }
            break;
        case STATE_WALK:
            switch (direction) {
            case RIGHT_DIR:
                if (speed_x < MAX_X_SPEED) {
                    speed_x += X_ACCEL;
                }

                while (engine->check_wall(x + coll_x + coll_width + speed_x, y + coll_y + coll_height, z) && (speed_x != 0)) {
                    speed_x--;
                }

                current_image = 5 + anim.next_frame(4, 5);
                speed_z = 0;
                break;
            case LEFT_DIR:
                if (speed_x > MIN_X_SPEED) {
                    speed_x -= X_ACCEL;
                }

                while (engine->check_wall(x + coll_x + speed_x, y + coll_y + coll_height, z) && (speed_x != 0)) {
                    speed_x++;
                }
                current_image = anim.next_frame(4, 5);
                speed_z = 0;
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
        case STATE_TURN:
            if (direction == RIGHT_DIR || direction == LEFT_DIR) {
                direction = (direction == RIGHT_DIR ? LEFT_DIR : RIGHT_DIR);
                speed_x = 0;
                counter = random() % MAX_NUM + 15;
                state = STATE_WALK;
            }
            break;
        case STATE_NONE:
            if (direction == RIGHT_DIR || direction == LEFT_DIR) {
                current_image = (direction == RIGHT_DIR ? 4 : 9);
            }
            counter = 0;
            anim.reset();
            break;
        case STATE_FIRE:
            if (engine->player->GetX() < x) {
                direction = LEFT_DIR;
            } else {
                direction = RIGHT_DIR;
            }
            counter = 2 * FIRE_DELAY;
            state = STATE_FIRING;
            anim.reset();
            speed_x = 0;
            speed_z = 0;
            counter2 = 0;
            /* fallthrough */
        case STATE_FIRING:
            switch (counter2) {
            case 0:
                current_image = (direction == RIGHT_DIR ? 14 : 10) + (r = anim.next_frame(2, 20));
                if (r < 2) {
                    break;
                }
                counter2 = 1;
                anim.reset();
                [[fallthrough]];
            case 1:
                [[fallthrough]];
            case 2:
                r = anim.next_frame(2, 10);
                if (r == 1 && counter2 == 1) {
                    counter2 = 2;
                    engine->create_object(new HighSpeed_Bullet_o(x + (direction == RIGHT_DIR ? 100 : 12), y + 13, z, direction, ~FRIEND_PLAYER, FIRE_STRENGTH));
                    if (direction == RIGHT_DIR) {
                        engine->create_effect(new shells_o(x + 90, y + 22, z, "soldier/hylsa", 1, -(random() % 5), -(random() % 8), 0));
                    } else {
                        engine->create_effect(new shells_o(x + 23, y + 22, z, "soldier/hylsa", 1, random() % 5, -(random() % 8), 0));
                    }
                } else if (r == 2) {
                    counter2 = 1;
                    if (direction == LEFT_DIR && engine->player->GetX() < x && engine->player->GetX() > x - FIRE_RANGE && engine->player->GetLayer() == layer) {
                        r = 0;
                    } else if (direction == RIGHT_DIR && engine->player->GetX() > x && engine->player->GetX() < x + FIRE_RANGE && engine->player->GetLayer() == layer) {
                        r = 0;
                    }
                }
                current_image = (direction == RIGHT_DIR ? 16 : 12) + r;
                if (r < 2) {
                    break;
                }
                counter2 = 3;
                anim.reset();
                [[fallthrough]];
            case 3:
                current_image = (direction == RIGHT_DIR ? 15 : 11) - (r = anim.next_frame(2, 20));
                if (r == 2) {
                    state = STATE_NONE;
                    current_image = (direction == RIGHT_DIR ? 4 : 9);
                }
                break;
            }
            //                               	current_image = (direction == RIGHT_DIR ? 14 : 10) + (r = anim.next_frame(3, FIRING_DELAY));
            //				if (r == 1 && counter2 == 0) {
            /*                        	current_image = (direction == RIGHT_DIR ? 22 : 20) + (r = anim.next_frame(1, FIRING_DELAY));
				if (r == 1 && counter2 == 0) {
                                	engine->create_object(new HighSpeed_Bullet_o(x, y + 22, z, direction, ~(FRIEND_PLAYER | FRIEND_SCIENTIST), FIRE_STRENGTH));
					counter2 = 1;
                                        if (direction == RIGHT_DIR)
                                        	engine->create_effect(new shells_o(x + 56, y + 22, z, "soldier/hylsa", 1, -(random() % 5), -(random() % 8), 0));
					else
						engine->create_effect(new shells_o(x + 44, y + 22, z, "soldier/hylsa", 1, random() % 5, -(random() % 8), 0));
				} else if (r == 0)
                                	counter2 = 0; */
            break;
        default:
            break;
        }
    }

    if (speed_y == 0) {
        switch (direction) {
        case LEFT_DIR:
            if (!engine->check_floor(x + FOOT_LEFT + speed_x, y + height, z) && !engine->check_floor(x + FOOT_LEFT + speed_x, y + height + TILE_SIDE_HEIGHT, z)) {
                speed_x = 0;
                state = STATE_TURN;
            }
            break;
        case RIGHT_DIR:
            if (!engine->check_floor(x + FOOT_RIGHT + speed_x, y + height, z) && !engine->check_floor(x + FOOT_RIGHT + speed_x, y + height + TILE_SIDE_HEIGHT, z)) {
                speed_x = 0;
                state = STATE_TURN;
            }
            break;
        default:
            break;
        }
    }

    // Fall or Stop
    if (engine->check_floor(x + FOOT_LEFT, y + height, z) || engine->check_floor(x + FOOT_RIGHT, y + height, z)) {
        speed_y = 0;
    } else if (speed_y < MAX_Y_SPEED) {
        speed_y += Y_ACCEL;
        speed_z = 0;
    }

    x += speed_x;
    y += speed_y;
    z += speed_z;

    if (x < 0) {
        x = 0;
    }

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

    if ((speed_x == 0) && (speed_z == 0) && state != STATE_FIRING && state != STATE_COLLAPS && state != STATE_WAITING && state != STATE_DYING) {
        if (direction == LEFT_DIR || direction == RIGHT_DIR) {
            current_image = (direction == RIGHT_DIR ? 4 : 9);
        }
    }
    return 0;
}

void Crycheck_o::Collision(Object* o)
{
    int i = 0;

    if (state == STATE_DEAD) {
        return;
    }

    Object::Collision(o);

    if ((o->GetStrength() > 0) && ((o->GetFriends() & me) == 0U) && state != STATE_DEAD && state != STATE_DYING) {
        if ((o->GetWho() & ENEMY_EXPLOSION) != 0) {
            state = STATE_DEAD;
            return;
        }
        if ((o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM)) != 0U) {
            engine->create_effect(new BurningBody_o(x, y + height, z));
            state = STATE_DEAD;
        } else if ((o->GetWho() & ENEMY_ICEBALL) != 0U) {
            engine->create_object(new FrosenBody_o(x, y + height, z, direction));
            state = STATE_DEAD;
        } else if ((o->GetWho() & (ENEMY_HS_BULLET | ENEMY_PLASMA)) != 0U) {
            if (state == STATE_COLLAPS) {
                state = STATE_DEAD;
                return;
            }
            if (state == STATE_WAITING) {
                direction = -100;
                state = STATE_DYING;
                anim.reset();
                return;
            }
            if (
                direction == RIGHT_DIR) {
                for (i = 0; i < 7; i++) {
                    engine->create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
                }
            } else {
                for (i = 0; i < 7; i++) {
                    engine->create_effect(new blood_o(x + width / 2, y + random() % height, z, 2 + random() % 4));
                }
            }
            if (state != STATE_COLLAPS && (energy == 0)) {
                state = STATE_COLLAPS;
                score += 50;
                anim.reset();
            }
        }
        counter = 0;
    }
}

/**************************************************************************/
Crycheck_o::~Crycheck_o()
    = default;
/**************************************************************************/

/**************************************************************************/
Boss_Crycheck_o::Boss_Crycheck_o(int X, int Y, int Z)
    : Crycheck_o(X, Y, Z)
{
    score = 1500;
    energy = 30;
}

Boss_Crycheck_o::~Boss_Crycheck_o()
    = default;

auto Boss_Crycheck_o::update() -> int
{
    int ret = 0;
    if ((ret = Crycheck_o::update()) != 0) {
        engine->create_object(new card_o(x, y, z));
    }
    return ret;
}
