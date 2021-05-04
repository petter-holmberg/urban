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
#include "engine.h"
#include "object2.h"
#include <allegro.h>
#include <cstdlib>
#include <cstring>

inline constexpr auto STATE_NONE = 0x00;
inline constexpr auto STATE_MOVE = 0x01;
inline constexpr auto STATE_STOP = 0x02;
inline constexpr auto STATE_RETURN = 0x03;
/****************************************************************************/
Elevator_o::Elevator_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];

    images = new BITMAP*;
    sprintf(filename, "elevator.pcx");
    images[0] = icache.GetImage(filename, pal);
    if (images[0] != nullptr) {
        num_images++;
    }

    current_image = 0;

    height = images[0]->h;
    width = images[0]->w;
    //	x += TILE_WIDTH / 2;
    //	x -= width / 2;
    x += TILE_WIDTH;
    y -= height;
    y += 8; //OBS! hack
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;
    energy = 1;
    strength = 0;

    speed_x = 0;
    speed_y = 0;
    speed_z = 0;
    counter = 0;

    me = FRIEND_ELEVATOR;
    friends = 0;
    enemies = 0;

    anim.reset();

    counter2 = 0;
    counter3 = 0;
    state = STATE_NONE;
    passenger = nullptr;
    direction = UP_DIR;
    counter = 0;
}
/****************************************************************************/
Elevator_o::~Elevator_o()
    = default;

/****************************************************************************/
auto Elevator_o::update() -> int
{

    switch (state) {
    case STATE_MOVE:
        switch (direction) {
        case UP_DIR:
            speed_y = -1;
            break;
        case DOWN_DIR:
            speed_y = 1;
            break;
        case LEFT_DIR:
            speed_x = -1;
            break;
        case RIGHT_DIR:
            speed_x = 1;
            break;
        }
        counter++;
        break;
        //		case STATE_RETURN:
        //                	if (counter--)
        //				break;
        //			state = STATE_STOP;
    case STATE_STOP:
    case STATE_NONE:
    default:
        speed_y = 0;
        speed_x = 0;
        counter = 0;
        break;
    }

    //        if (passenger == NULL && speed_y)
    //		counter++;

    if ((passenger != nullptr) && state == STATE_MOVE) {
        passenger->SetY(passenger->GetY() + speed_y);
        passenger->SetX(passenger->GetX() + speed_x);
    } else if (passenger == nullptr && state == STATE_MOVE) {
        /*        	if (direction == UP_DIR) {
	               	direction = DOWN_DIR;
                        state = STATE_RETURN;
                        speed_y = -speed_y;
                        speed_x = -speed_x;
		}*/
    }

    y += speed_y;
    x += speed_x;

    passenger = nullptr;

    return 0;
}

void Elevator_o::Activate(int dir)
{
    direction = dir;
    state = STATE_MOVE;
}

void Elevator_o::DeActivate()
{
    state = STATE_STOP;
}

void Elevator_o::Collision(Object* o)
{
    switch (o->GetWho()) {
    case FRIEND_ELEVSTAT:
        //                	state = STATE_STOP;
        break;
    default:
        if (o == PLAYER) {
            passenger = o;
        }
        break;
    }
}
