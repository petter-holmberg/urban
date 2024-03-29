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
#include "engine.h"
#include "object.h"
#include <vector>

/****************************************************************************/
inline constexpr auto STATE_NONE = 0x00;
inline constexpr auto STATE_FIRE = 0x01;
inline constexpr auto STATE_FIRING = 0x02;
inline constexpr auto FIRE_STRENGTH = 3;

/****************************************************************************/
FloorGun_o::FloorGun_o(int X, int Y, int Z)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];

    images.resize(2, nullptr);
    sprintf(filename, "auto-f/shot1.pcx");
    images[0] = icache.GetImage(filename, pal);
    sprintf(filename, "auto-f/shot2.pcx");
    images[1] = icache.GetImage(filename, pal);

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
    spy = nullptr;
    energy = 1;
    strength = 0;
    speed_x = 0;
    // Fall to the floor
    speed_y = 5;
    speed_z = 0;
    counter = 0;
    //	evil = 1;
    me = FRIEND_MINE;
    friends = 0;
    enemies = 0;
    anim.reset();
    counter2 = 0;
    counter3 = 0;
    state = STATE_NONE;
}
/****************************************************************************/
FloorGun_o::~FloorGun_o()
    = default;

/****************************************************************************/
auto FloorGun_o::update() -> int
{

    if (counter != 0) {
        counter--;
    }

    if ((counter == 0) && (counter3 == 0) && state == STATE_FIRING) {
        state = STATE_NONE;
    }

    if (counter3 != 0) {
        counter3--;
    }

    switch (state) {
    case STATE_NONE:
        if ((spy != nullptr) && (spy->GetSpy() != 0) && (counter3 == 0)) {
            engine->sound.PlaySFX("samples/hydraul1.wav");
            state = STATE_FIRE;
            spy->ResetSpy();
            counter3 = 30;
            counter = counter3 + 1;
        }
        current_image = 0;
        break;
    case STATE_FIRE:
        if (counter3 == 0) {
            state = STATE_FIRING;
            counter = 100;
            anim.reset();
            current_image = 0;
        } else {
            break;
        }
    /* fallthrough */
    case STATE_FIRING:
        if (current_image == 0) {

            if ((current_image = anim.next_frame(1, 3)) != 0) {
                engine->create_object(new HighSpeed_Bullet_o(x, y + 3, z, LEFT_DIR, 0, FIRE_STRENGTH));
            }
        } else {
            current_image = anim.next_frame(1, 3);
        }
        break;
    }

    // Fall or Stop
    if (engine->check_floor(x, y + height, z) || engine->check_floor(x + width, y + height, z)) {
        speed_y = 0;
        if (counter2 == 0) {
            engine->create_object((spy = new Spy_o(x - 150, y, z)));
            counter2 = 1;
        }
    } else {
        y += speed_y;
    }
    return 0;
}
void FloorGun_o::Collision(Object* /*o*/)
{
}
