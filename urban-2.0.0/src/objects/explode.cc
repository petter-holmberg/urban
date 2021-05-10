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
#include "object.h"
#include <vector>

inline constexpr auto EXPLODE_SAMPLE = "samples/ex12.wav";
inline constexpr auto EXPLODE_SPEED = 4;
inline constexpr auto EXPLODE_DAMAGE = 10;
inline constexpr auto EXPLODE_FRAMES = 2;
explosion_o::explosion_o(int X, int Y, int Z, int /*Speed_X*/, int /*Speed_Y*/, int /*Speed_Z*/, int Friends)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    anim.reset();
    images.resize(10, nullptr);

    for (i = 0; i < 10; i++) {
        sprintf(filename, "expl/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }
    height = images[0]->h;
    width = images[0]->w;

    current_image = 0;
    y -= (height / 2);

    x -= (width / 2);
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    energy = 1;
    strength = EXPLODE_DAMAGE;
    speed_x = 0;
    speed_y = 0;
    score = 0;
    friends = FRIEND_EXPLOSION | Friends;
    enemies = ~friends;
    counter = EXPLODE_FRAMES;
    me = FRIEND_EXPLOSION;
    ENGINE.EnableEarthquake();
    SOUND.PlaySFX(EXPLODE_SAMPLE);
}

explosion_o::~explosion_o()
    = default;

auto explosion_o::update() -> int
{
    //        enemies = 0;

    if (counter != 0) {
        counter--;
    }

    if (counter == 0) {
        strength = 0;
    }
    /*
        if(counter2 == 0) {

        	counter2 = 30;
                strength = 9;
        } else {

        	strength = 0;
                counter2--;
        }*/
    current_image = anim.next_frame(10, EXPLODE_SPEED);

    /*        if (current_image > 5)
                strength = 0;*/
    if (current_image >= 10) {
        return -1;
    }
    return 0;
}

void explosion_o::Collision(Object* o)
{
    /*	if (((o->GetWho() & enemies))) {

        	counter2 = 100;
        }*/
}
