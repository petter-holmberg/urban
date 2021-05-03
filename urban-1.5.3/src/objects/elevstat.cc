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
#include <string.h>
#include <stdlib.h>
#include <allegro.h>
#include "engine.h"
#include "object2.h"


#define STATE_NONE	0x00
#define STATE_ACTIVATE	0x01
#define STATE_ACTIVATED	0x02

#define STATE_STOP	0x03
#define STATE_SEND	0x04

#define STATE_LOCATESTATION	0x05

#define SEND_DELAY	100
#define FRAME_DELAY	2

/****************************************************************************/
ElevatorStation_o::ElevatorStation_o(int X, int Y, int Z, int dir, Cardtype Card) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];

	images = new BITMAP*[10];

        for (int i = 0;i < 10;i++) {
        	sprintf(filename, "elepan%d.pcx", i);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        /*
        switch (dir) {
        	case UP_DIR:
                	sprintf(filename, "elepan1.pcx");
                        break;
                case DOWN_DIR:
                	sprintf(filename, "elepan1.pcx");
                        break;
                case LEFT_DIR:
                case RIGHT_DIR:
                	break;
	}
        images[0] = icache.GetImage(filename, pal);
        if (images[0])
        	num_images++;
	*/
	current_image = 0;

        height = images[0]->h;
        width = images[0]->w;
//	x += TILE_WIDTH / 2;
//	x -= width / 2;
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
        elevator = NULL;
        wire_up = wire_down = NULL;
        elev_stat = NULL;
}
/****************************************************************************/
ElevatorStation_o::~ElevatorStation_o() {
}

/****************************************************************************/
int ElevatorStation_o::update() {

	if (counter)
        	counter--;

        switch (state) {
        	case STATE_LOCATESTATION:
                	if (direction == DOWN_DIR) {
                        	if (wire_down) {
                                	((ElevatorWire_o *)wire_down)->FindElevatorStation(this, this);
					state = STATE_NONE;
				}
                        } else
                        	state = STATE_NONE;
                        break;
        	case STATE_SEND:
                	if (elevator) {
                        	((Elevator_o *)elevator)->Activate(direction_to_send);
                                state = STATE_NONE;
				ENGINE.PushMessage("Elevator on its way");
			}
                        break;
        	case STATE_ACTIVATE:
                	if (elevator) {
                        	((Elevator_o *)elevator)->Activate(direction);
                                state = STATE_ACTIVATED;
			} else {
                        	if (elev_stat)
	                        	((ElevatorStation_o *)elev_stat)->SendElevator(direction == UP_DIR ? DOWN_DIR : UP_DIR);
				state = STATE_STOP;
                        }
			break;
                case STATE_ACTIVATED:
                	if (elevator == NULL)
                        	state = STATE_NONE;
                	break;
                case STATE_NONE:
                	if (elevator == NULL)
                        	state = STATE_STOP;
			break;
		case STATE_STOP:
                	if (elevator)
	                	if (direction == UP_DIR && elevator->GetY() > (y + height - 9)) {
                                	state = STATE_NONE;
                                        ((Elevator_o *)elevator)->DeActivate();
				} else if (direction == DOWN_DIR && elevator->GetY() < (y + height - 7)) {
                                	state = STATE_NONE;
                                        ((Elevator_o *)elevator)->DeActivate();
                                }
/*                	if (elevator && elevator->GetY() > (y + height - 8)) {
                        	state = STATE_NONE;
                                ((Elevator_o *)elevator)->DeActivate();
			}*/
			break;
                default:
                	break;
	}

        elevator = NULL;

	current_image = anim.next_frame(num_images - 1, FRAME_DELAY);

	return 0;
}

int ElevatorStation_o::StartElevator() {
	if (counter)
        	return 0;

        counter = SEND_DELAY;

	if (state == STATE_NONE || state == STATE_STOP)
		if (card == none || ((player_o *)PLAYER)->HaveCard(card))
        		state = STATE_ACTIVATE;
		else if(card != none) {
			char msg[80];
			
	        	switch (card) {
	                	case green:
	                        	strcpy(msg, "Green access denied");
	                                break;
	                        case blue:
	                        	strcpy(msg, "Blue access denied");
	                                break;
	                        case red:
	                        	strcpy(msg, "Red access denied");
	                                break;
	                        case yellow:
	                        	strcpy(msg, "Yellow access denied");
	                                break;
				default:
					strcpy(msg, "elevstat.cc: unknown card");
					break;
			}
			ENGINE.PushMessage(msg);
		}
	return state == STATE_ACTIVATE ? 1 : 0;
}

void ElevatorStation_o::SetElevatorStation(Object *w) {
	if (elev_stat == NULL) {
		elev_stat = w;
                ((ElevatorStation_o *)elev_stat)->SetElevatorStation(this);
	}
}

void ElevatorStation_o::SendElevator(int dir) {
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


void ElevatorStation_o::Collision(Object *o) {
	if (o->GetWho() == FRIEND_ELEVATOR)
        	elevator = o;

	if (o->GetWho() == FRIEND_ELEVWIRE) {
        	if (direction == DOWN_DIR)
                	wire_down = o;
		else
                	wire_up = o;
	}

}
