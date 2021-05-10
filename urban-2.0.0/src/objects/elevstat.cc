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
#include "engine.h"
#include "object2.h"
#include <string>
#include <vector>

inline constexpr auto STATE_NONE = 0x00;
inline constexpr auto STATE_ACTIVATE = 0x01;
inline constexpr auto STATE_ACTIVATED = 0x02;

inline constexpr auto STATE_STOP = 0x03;
inline constexpr auto STATE_SEND = 0x04;

inline constexpr auto STATE_LOCATESTATION = 0x05;

inline constexpr auto SEND_DELAY = 100;
inline constexpr auto FRAME_DELAY = 2;

/****************************************************************************/
ElevatorStation_o::ElevatorStation_o(int X, int Y, int Z, int dir, Cardtype Card)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];

    images.resize(10, nullptr);

    for (int i = 0; i < 10; i++) {
        sprintf(filename, "elepan%d.pcx", i);
        images[i] = icache.GetImage(filename, pal);
    }
    current_image = 0;

    height = images[0]->h;
    width = images[0]->w;
    x += TILE_WIDTH;
    y -= height;
    coll_x = -TILE_WIDTH;
    coll_y = 0;
    coll_width = width + TILE_WIDTH;
    coll_height = height;
    energy = 1;
    strength = 0;

    speed_x = 0;
    speed_y = 0;
    speed_z = 0;
    counter = 0;

    me = FRIEND_ELEVSTAT;
    friends = 0;
    enemies = 0;

    anim.reset();

    counter2 = 0;
    counter3 = 0;
    //        state = STATE_NONE;
    state = STATE_LOCATESTATION;
    direction = dir;
    card = Card;
    elevator = nullptr;
    wire_up = wire_down = nullptr;
    elev_stat = nullptr;
}
/****************************************************************************/
ElevatorStation_o::~ElevatorStation_o()
    = default;

/****************************************************************************/
auto ElevatorStation_o::update() -> int
{

    if (counter != 0) {
        counter--;
    }

    switch (state) {
    case STATE_LOCATESTATION:
        if (direction == DOWN_DIR) {
            if (wire_down != nullptr) {
                (dynamic_cast<ElevatorWire_o*>(wire_down))->FindElevatorStation(this, this);
                state = STATE_NONE;
            }
        } else {
            state = STATE_NONE;
        }
        break;
    case STATE_SEND:
        if (elevator != nullptr) {
            (dynamic_cast<Elevator_o*>(elevator))->Activate(direction_to_send);
            state = STATE_NONE;
            ENGINE.PushMessage("Elevator on its way");
        }
        break;
    case STATE_ACTIVATE:
        if (elevator != nullptr) {
            (dynamic_cast<Elevator_o*>(elevator))->Activate(direction);
            state = STATE_ACTIVATED;
        } else {
            if (elev_stat != nullptr) {
                (dynamic_cast<ElevatorStation_o*>(elev_stat))->SendElevator(direction == UP_DIR ? DOWN_DIR : UP_DIR);
            }
            state = STATE_STOP;
        }
        break;
    case STATE_ACTIVATED:
        if (elevator == nullptr) {
            state = STATE_NONE;
        }
        break;
    case STATE_NONE:
        if (elevator == nullptr) {
            state = STATE_STOP;
        }
        break;
    case STATE_STOP:
        if (elevator != nullptr) {
            if (direction == UP_DIR && elevator->GetY() > (y + height - 9)) {
                state = STATE_NONE;
                (dynamic_cast<Elevator_o*>(elevator))->DeActivate();
            } else if (direction == DOWN_DIR && elevator->GetY() < (y + height - 7)) {
                state = STATE_NONE;
                (dynamic_cast<Elevator_o*>(elevator))->DeActivate();
            }
        }
        break;
    default:
        break;
    }

    elevator = nullptr;

    current_image = anim.next_frame(images.size() - 1, FRAME_DELAY);

    return 0;
}

auto ElevatorStation_o::StartElevator() -> int
{
    if (counter != 0) {
        return 0;
    }

    counter = SEND_DELAY;

    if (state == STATE_NONE || state == STATE_STOP) {
        if (card == none || ((dynamic_cast<player_o*>(PLAYER))->HaveCard(card) != 0)) {
            state = STATE_ACTIVATE;
        } else if (card != none) {
            std::string msg;

            switch (card) {
            case green:
                msg = "Green access denied";
                break;
            case blue:
                msg = "Blue access denied";
                break;
            case red:
                msg = "Red access denied";
                break;
            case yellow:
                msg = "Yellow access denied";
                break;
            default:
                msg = "elevstat.cc: unknown card";
                break;
            }
            ENGINE.PushMessage(msg.c_str());
        }
    }
    return state == STATE_ACTIVATE ? 1 : 0;
}

void ElevatorStation_o::SetElevatorStation(Object* w)
{
    if (elev_stat == nullptr) {
        elev_stat = w;
        (dynamic_cast<ElevatorStation_o*>(elev_stat))->SetElevatorStation(this);
    }
}

void ElevatorStation_o::SendElevator(int dir)
{
    direction_to_send = dir;
    state = STATE_SEND;
    /*	if (dir == -1) {
        	state = STATE_NONE;
		if (direction_to_send == UP_DIR) {
                	if (wire_down)
				((ElevatorWire_o *)wire_down)->FindElevator(-1, this);
		} else {
                	if (wire_up)
				((ElevatorWire_o *)wire_up)->FindElevator(-1, this);
		}
	} else {
		state = STATE_SEND;
		direction_to_send = dir;
	}*/
}

void ElevatorStation_o::Collision(Object* o)
{
    if (o->GetWho() == FRIEND_ELEVATOR) {
        elevator = o;
    }

    if (o->GetWho() == FRIEND_ELEVWIRE) {
        if (direction == DOWN_DIR) {
            wire_down = o;
        } else {
            wire_up = o;
        }
    }
}
