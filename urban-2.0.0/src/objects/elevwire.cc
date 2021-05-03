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
#define STATE_FIND	0x01
#define STATE_DESTROY	0x02
/****************************************************************************/
ElevatorWire_o::ElevatorWire_o(int X, int Y, int Z) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];

	images = new BITMAP*;
        sprintf(filename, "connect.pcx");
        images[0] = icache.GetImage(filename, pal);
        if (images[0])
        	num_images++;

	current_image = 0;

        height = images[0]->h;
        width = images[0]->w;
//	x += TILE_WIDTH / 2;
//	x -= width / 2;
	x += TILE_WIDTH;
        y -= height;
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

        me = FRIEND_ELEVWIRE;
	friends = 0;
        enemies = 0;

	anim.reset();

	counter2 = 0;
	counter3 = 0;
        state = STATE_NONE;
	elev_controller = NULL;
        wire_up = wire_down = NULL;
}

/****************************************************************************/
ElevatorWire_o::~ElevatorWire_o() {
}

/****************************************************************************/
int ElevatorWire_o::update() {
	if (state == STATE_DESTROY)
        	return -1;

	return 0;
}

#if 0
void ElevatorWire_o::FindElevator(int dir, Object *w) {

/*
	if (wire_left && wire_left != w)
		((ElevatorWire_o *)wire_left)->FindElevator(-1, this);

	if (wire_right && wire_right != w)
		((ElevatorWire_o *)wire_right)->FindElevator(-1, this);

	if (elev_controller && elev_controller != w)
        	((ElevatorStation_o *)elev_controller)->SendElevator(dir);

	if (wire_down && wire_down != w)
        	((ElevatorWire_o *)wire_down)->FindElevator(UP_DIR, this);

	if (wire_up && wire_up != w)
        	((ElevatorWire_o *)wire_up)->FindElevator(DOWN_DIR, this);
*/
/*
        if (dir == UP_DIR && wire_down)
        	((ElevatorWire_o *)wire_down)->FindElevator(dir);
        else if (dir == DOWN_DIR


        if (dir == UP_DIR) {
       		if (elev_controller && elev_controller->GetY() > y)
               		((ElevatorStation_o *)elev_controller)->SendElevator(dir);
		else if (wire_down)
       	        	((ElevatorWire_o *)wire_down)->FindElevator(dir);
	} else {
       		if (elev_controller && elev_controller->GetY() < y)
               		((ElevatorStation_o *)elev_controller)->SendElevator(dir);
		else if (wire_up)
       	        	((ElevatorWire_o *)wire_up)->FindElevator(dir);
	}
*/
}
#endif

void ElevatorWire_o::FindElevatorStation(Object *w, Object *sender) {
	if (elev_controller && elev_controller != sender)
        	((ElevatorStation_o *)elev_controller)->SetElevatorStation(w);
	else if (wire_down)
        	((ElevatorWire_o *)wire_down)->FindElevatorStation(w, this);

	state = STATE_DESTROY;
}

void ElevatorWire_o::Collision(Object *o) {
	if (o->GetWho() == FRIEND_ELEVSTAT)
        	elev_controller = o;

	if (o->GetWho() == FRIEND_ELEVWIRE) {
        	if (o->GetY() > y)
                	wire_down = o;
		else if (o->GetY() < y)
                	wire_up = o;
	}
}