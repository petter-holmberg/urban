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
#include <stdlib.h>
#include <string.h>

/****************************************************************************/
#define STATE_NONE 0x00
#define STATE_WAIT 0x01
#define STATE_STRIKE 0x02
#define STATE_DONE 0x03

#define NUM_NUKES 90
#define NUKE_MULTI 10

#define AREA_START -300
#define AREA_END 300
#define AREA_WIDTH 600
#define AREA_MUL (AREA_WIDTH / NUM_NUKES)

#define STRIKE_DELAY 600

#define STRIKE_START -20
#define STRIKE_END 20

#define STRIKE_SAMPLE "samples/hellfire.wav"
#define WARNING_SAMPLE "samples/airnuke.wav"

/****************************************************************************/
Airstrike_o::Airstrike_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    RGB pal[256];
    char filename[512];

    /*
	images = new BITMAP*[2];
        sprintf(filename, "auto-f/shot1.pcx");
        images[0] = icache.GetImage(filename, pal);
        if (images[0])
        	num_images++;
        sprintf(filename, "auto-f/shot2.pcx");
        images[1] = icache.GetImage(filename, pal);
        if (images[1])
        	num_images++;
*/
    images = new BITMAP*;
    sprintf(filename, "mine2.pcx");
    images[0] = icache.GetImage(filename, pal);
    if (images[0])
        num_images++;

    current_image = 0;

    height = images[0]->h;
    width = images[0]->w;
    x += TILE_WIDTH / 2;
    x -= width / 2;
    y -= height;
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;
    energy = 1;
    strength = 0;
    speed_x = 0;
    // Fall to the floor
    speed_y = 0;
    speed_z = 0;
    counter = 0;
    //	evil = 1;
    me = FRIEND_DEKOR;
    friends = 0;
    enemies = 0;

    counter2 = 0;
    counter3 = 0;
    state = STATE_NONE;
}
/****************************************************************************/
Airstrike_o::~Airstrike_o()
{
}

/****************************************************************************/
int Airstrike_o::update()
{

    if (counter)
        counter--;

    if (counter2)
        counter2--;

    if (state == STATE_DONE)
        return -1;

    switch (state) {
    case STATE_NONE:
        if (PLAYER->GetX() > x + STRIKE_START && PLAYER->GetX() < x + STRIKE_END) {
            state = STATE_WAIT;
            counter = STRIKE_DELAY;
            ENGINE.PushMessage("Airstrike alert!");
            SOUND.PlaySFX_Critical(WARNING_SAMPLE);
        }
        break;
    case STATE_WAIT:
        if (counter == 0) {
            state = STATE_STRIKE;
        }
        break;
    case STATE_STRIKE:
        SOUND.PlaySFX_Critical(STRIKE_SAMPLE);
        for (int i = 0; i < NUM_NUKES; i++) {
            ENGINE.create_alwaysupdate(new AirGrenade_o(x + AREA_START + i * AREA_MUL, y - i * 15, (random() % 3) * TILE_TOP_HEIGHT, 2, 5, 0));
        }
    default:
        state = STATE_DONE;
        break;
    }

    return 0;
}

void Airstrike_o::Collision(Object* o)
{
}
