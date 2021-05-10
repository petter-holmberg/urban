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
#include "object2.h"
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
inline constexpr auto STATE_ALARM = 8;
inline constexpr auto STATE_INTERLAYER = 9;

inline constexpr auto FIRE_RANGE = 300;
inline constexpr auto FIRE_STRENGTH = 5;
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
inline constexpr auto SHELL_DELAY = 4;
inline constexpr auto FIRING_DELAY = 5;
inline constexpr auto FIRE_DELAY = 5 * FIRING_DELAY;
inline constexpr auto HIT_DELAY = 10;
inline constexpr auto JUMP_DELAY = 20;
inline constexpr auto LAND_DELAY = 10;

inline constexpr auto FOOT_LEFT = 39;
inline constexpr auto FOOT_WIDTH = 17;
inline constexpr auto FOOT_RIGHT = FOOT_LEFT + FOOT_WIDTH;

inline constexpr auto NO_BOX_DISTANCE = 20;
inline constexpr auto BOX_DISTANCE = 50;

/**************************************************************************/
Seagal_o::Seagal_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    anim.reset();
    images.resize(30, nullptr);

    for (i = 0; i < 5; i++) {
        sprintf(filename, "specf4/specv/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 5; i < 10; i++) {
        sprintf(filename, "specf4/spech/%d.pcx", i + 1 - 5);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 10; i < 15; i++) {
        sprintf(filename, "specf4/specfram/%d.pcx", i + 1 - 10);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 15; i < 20; i++) {
        sprintf(filename, "specf4/specbak/%d.pcx", i + 1 - 15);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 20; i < 24; i++) {
        sprintf(filename, "specf4/specv/shot%d.pcx", i + 1 - 20);
        images[i] = icache.GetImage(filename, pal);
    }
    for (i = 24; i < 28; i++) {
        sprintf(filename, "specf4/spech/shot%d.pcx", i + 1 - 24);
        images[i] = icache.GetImage(filename, pal);
    }
    sprintf(filename, "specf4/spech/hit.pcx");
    images[28] = icache.GetImage(filename, pal);

    sprintf(filename, "specf4/specv/hit.pcx");
    images[29] = icache.GetImage(filename, pal);

    current_image = 1;

    height = images[0]->h;
    width = images[0]->w;
    coll_x = 33;
    coll_y = 0;
    coll_width = 31;
    coll_height = height - 1;

    //stand on feet
    y -= images[0]->h;
    energy = 30;
    strength = 10;
    speed_x = 0;
    speed_y = 0;
    speed_z = 0;
    direction = RIGHT_DIR;
    state = STATE_NONE;
    counter = 0;
    score = 900;
    friends = ~FRIEND_PLAYER;
    enemies = ENEMY_PLAYER;
    me = FRIEND_RAMBO;
}

auto Seagal_o::update() -> int
{
    int r = 0;

    if (energy <= 0) {
        return -1;
    }

    if (counter != 0) {
        counter--;
    }

    if (counter == 0) {
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

    if ((state != STATE_FIRE && state != STATE_FIRING && state != STATE_TURN && state != STATE_ALARM && state != STATE_HIT && state != STATE_HIT_COUNTDOWN)) {
        if (PLAYER->GetX() < x && PLAYER->GetX() > x - FIRE_RANGE && (direction == LEFT_DIR || direction == UP_DIR || direction == DOWN_DIR)) {
            if (PLAYER->GetLayer() == layer) {
                state = STATE_FIRE;
            } else {
                state = STATE_WALK;
                if (PLAYER->GetLayer() > layer) {
                    direction = DOWN_DIR;
                } else {
                    direction = UP_DIR;
                }
                counter = 15;
            }
        } else if (PLAYER->GetX() > x && PLAYER->GetX() < x + FIRE_RANGE && (direction == RIGHT_DIR || direction == UP_DIR || direction == DOWN_DIR)) {
            if (PLAYER->GetLayer() == layer) {
                state = STATE_FIRE;
            } else {
                state = STATE_WALK;
                if (PLAYER->GetLayer() > layer) {
                    direction = DOWN_DIR;
                } else {
                    direction = UP_DIR;
                }
                counter = 15;
            }
        }

        /*                	if (PLAYER->GetX() < x && PLAYER->GetX() > x - FIRE_RANGE &&
                        	(direction == LEFT_DIR || direction == UP_DIR || direction == DOWN_DIR) &&
                                PLAYER->GetLayer() == layer) {
                                state = STATE_FIRE;
			} else if (PLAYER->GetX() > x && PLAYER->GetX() < x + FIRE_RANGE &&
                        	(direction == RIGHT_DIR || direction == UP_DIR || direction == DOWN_DIR) &&
                                PLAYER->GetLayer() == layer) {
                                state = STATE_FIRE;
			}*/
    }
    if (speed_y == 0) {
        switch (state) {
        case STATE_WALK:
            switch (direction) {
            case RIGHT_DIR:
                if (speed_x < MAX_X_SPEED) {
                    speed_x += X_ACCEL;
                }

                while (ENGINE.check_wall(x + coll_x + coll_width + speed_x, y + coll_y + coll_height, z) && (speed_x != 0)) {
                    speed_x--;
                }

                current_image = 5 + anim.next_frame(4, 5);
                speed_z = 0;
                break;
            case LEFT_DIR:
                if (speed_x > MIN_X_SPEED) {
                    speed_x -= X_ACCEL;
                }

                while (ENGINE.check_wall(x + coll_x + speed_x, y + coll_y + coll_height, z) && (speed_x != 0)) {
                    speed_x++;
                }
                current_image = anim.next_frame(4, 5);
                speed_z = 0;
                break;
            case UP_DIR:
                if (speed_z > MIN_Z_SPEED) {
                    speed_z -= Z_ACCEL;
                }
                while ((ENGINE.check_wall(x + coll_x, y + coll_y + coll_height, z + speed_z - 5) || ENGINE.check_wall(x + coll_x + coll_width, y + coll_y + coll_height, z + speed_z - 5)) && (speed_z != 0)) {
                    speed_z++;
                }
                current_image = 15 + anim.next_frame(4, 5);
                speed_x = 0;
                break;
            case DOWN_DIR:
                if (speed_z < MAX_Z_SPEED) {
                    speed_z += Z_ACCEL;
                }
                while ((ENGINE.check_wall(x + coll_x, y + coll_y + coll_height, z + speed_z + 5) || ENGINE.check_wall(x + coll_x + coll_width, y + coll_y + coll_height, z + speed_z + 5)) && (speed_z != 0)) {
                    speed_z--;
                }
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
            } else {
                current_image = (direction == UP_DIR ? 14 : 19);
            }
            counter = 0;
            anim.reset();
            break;
        case STATE_ALARM:
            if (current_image != 22 && current_image != 20) {
                counter = ALARM_DELAY;
                current_image = direction == RIGHT_DIR ? 22 : 20;
                speed_x = 0;
                speed_z = 0;
            }
            break;
        case STATE_FIRE:
            speed_x = 0;
            speed_z = 0;
            if (PLAYER->GetX() < x) {
                direction = LEFT_DIR;
            } else {
                direction = RIGHT_DIR;
            }
            counter = FIRE_DELAY;
            state = STATE_FIRING;
            anim.reset();
            speed_x = 0;
            speed_z = 0;
            counter2 = 0;
            /* fallthrough */
        case STATE_FIRING:
            current_image = (direction == RIGHT_DIR ? 24 : 20) + (r = anim.next_frame(5, FIRING_DELAY));
            if (r == 1 && counter2 == 0) {
                if (direction == RIGHT_DIR) {
                    ENGINE.create_object(new Missile_o(x + 70, y + 22, z, 10));
                } else {
                    ENGINE.create_object(new Missile_o(x, y + 22, z, -10));
                }
                counter2 = 1;
            } else if (r == 5) {
                state = STATE_STOP;
            }
            break;
        case STATE_HIT:
            if (direction == UP_DIR || direction == DOWN_DIR) {
                direction = PLAYER->GetX() < x ? LEFT_DIR : RIGHT_DIR;
            }
            state = STATE_HIT_COUNTDOWN;
            counter = HIT_DELAY;
            /* fall through */
        case STATE_HIT_COUNTDOWN:
            current_image = direction == RIGHT_DIR ? 28 : 29;
            break;
        default:
            break;
        }
    }

    if (speed_y == 0) {
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

    if ((speed_x == 0) && (speed_z == 0) && state != STATE_HIT_COUNTDOWN && state != STATE_FIRING && state != STATE_ALARM) {
        if (direction == LEFT_DIR || direction == RIGHT_DIR) {
            current_image = (direction == RIGHT_DIR ? 4 : 9);
        } else if (direction == UP_DIR || direction == DOWN_DIR) {
            current_image = (direction == UP_DIR ? 14 : 19);
        }
    }
    return 0;
}

void Seagal_o::Collision(Object* o)
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
                    ENGINE.create_effect(new BurningBody_o(x, y + height, z));
                } else if ((o->GetWho() & ENEMY_ICEBALL) != 0) {
                    ENGINE.create_object(new FrosenBody_o(x, y + height, z, direction));
                } else {
                    ENGINE.create_effect(new Animation_o(x, y + height, z, "specf4/spech/dead", 4, 3));
                }
            }
            if ((o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)) == 0) {
                for (i = 0; i < 3; i++) {
                    ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
                }
            }
            //				        ENGINE.create_effect(new blood_o(x + width, y + random() % height, z, 1 + random() % 4));
        } else {
            if (energy <= 0) {
                if ((o->GetWho() & ENEMY_EXPLOSION) != 0) {
                    DEATH_BY_EXPLOSION(x, width, y, height, z);
                } else if ((o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM)) != 0) {
                    ENGINE.create_effect(new BurningBody_o(x, y + height, z));
                } else if ((o->GetWho() & ENEMY_ICEBALL) != 0) {
                    ENGINE.create_object(new FrosenBody_o(x, y + height, z, direction));
                } else {
                    ENGINE.create_effect(new Animation_o(x, y + height, z, "specf4/specv/dead", 4, 3));
                }
            }
            if ((o->GetWho() & (ENEMY_FIREBALL | ENEMY_ICEBALL | ENEMY_BEAM)) == 0) {
                for (i = 0; i < 3; i++) {
                    ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z, -2 + random() % 4));
                }
            }
        }
        if ((energy % 4) == 0) {
            state = STATE_HIT;
        }
    }
}

/**************************************************************************/
Seagal_o::~Seagal_o()
    = default;
/**************************************************************************/

/**************************************************************************/
Boss_Seagal_o::Boss_Seagal_o(int X, int Y, int Z)
    : Seagal_o(X, Y, Z)
{
    score = 2000;
    energy = 40;
}

Boss_Seagal_o::~Boss_Seagal_o()
    = default;

auto Boss_Seagal_o::update() -> int
{
    int ret = 0;
    if ((ret = Seagal_o::update()) == -1) {
        ENGINE.create_object(new card_o(x, y, z));
    }
    return ret;
}
