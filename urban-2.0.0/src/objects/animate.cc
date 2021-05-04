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
#include "object.h"
#include <allegro.h>

#define MAX_Y_SPEED 4
#define Y_ACCEL 1
/****************************************************************************/
Animation_o::Animation_o(int X, int Y, int Z, const char* name, int num_pics, int dek_frame, int Speed_X, int Speed_Y, int Speed_Z)
    : Object(X, Y, Z)
{
    int i;
    RGB pal[256];
    char filename[512];

    images = new BITMAP*[num_pics];

    for (i = 0; i < num_pics; i++) {
        sprintf(filename, "%s%d.pcx", name, i + 1);
        images[i] = icache.GetImage(filename, pal);
        if (images[i])
            num_images++;
    }

    coll_x = 0;
    coll_y = 0;
    coll_width = width = images[0]->w;
    coll_height = height = images[0]->h;

    x = X;
    y = Y - height;
    z = Z;
    speed_x = Speed_X;
    speed_y = Speed_Y;
    speed_z = Speed_Z;
    DekorationFrame = dek_frame;
    anim.reset();

    direction = Speed_X > 0 ? RIGHT_DIR : LEFT_DIR;
    energy = 0;
    strength = 0;
    state = 0;
}
/****************************************************************************/
Animation_o::~Animation_o()
{
}
/****************************************************************************/
int Animation_o::update()
{

    if (state)
        return REMOVE_ME;
    // Fall or stop
    if (ENGINE.check_floor(x, y + height, z) || ENGINE.check_floor(x + width, y + height, z))
        speed_y = 0;
    else {

        y += speed_y;
        speed_y += Y_ACCEL;
        if (speed_y > MAX_Y_SPEED)
            speed_y = MAX_Y_SPEED;
    }

    current_image = anim.next_frame(num_images, 8);
    if (current_image == num_images) {
        if (DekorationFrame != -1) {
            current_image = DekorationFrame;
            ENGINE.create_dekoration(this);
            state = 1; /* avoid a blip - if I return REMOVE_ME one frame wont be drawn*/
        } else
            return REMOVE_ME;
    }
    return 0;
}
/****************************************************************************/
