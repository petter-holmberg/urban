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
#include "object.h"
#include "engine.h"
#include "icache.h"
#include <allegro.h>

Object::Object(int X, int Y, int Z, int Mode)
{
    images = nullptr;
    num_images = current_image = 0;
    speed_z = speed_x = speed_y = 0;
    direction = 0;
    layer = (Z / TILE_TOP_HEIGHT);
    x = X;
    y = Y;
    z = Z;
    score = 0;
    mode = Mode;
}

Object::~Object()
{
    int i = 0;
    for (i = 0; i < num_images; i++) {
        if (images[i] != nullptr) {
            icache.FreeImage(images[i]);
        }
    }
}

auto Object::GetImage() -> BITMAP*
{
    if (current_image < num_images) {
        return images[current_image];
    }
    {
        return images[0];
    }
}

auto Object::update() -> int
{
    return 0;
}

void Object::Collision(Object* o)
{

    if ((o->GetEnemies() & me) != 0U) {
        energy -= o->GetStrength();
    }

    if (energy < 0) {
        energy = 0;
    }
}
