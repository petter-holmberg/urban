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

inline constexpr auto FRAME_DELAY = 5;
/****************************************************************************/

/****************************************************************************/
Door_o::Door_o(int X, int Y, int /*Z*/, Cardtype Card)
    : Object(X, Y, 0)
{
    PALETTE pal;
    char filename[512];
    int i = 0;

    images.resize(5, nullptr);
    card = Card;

    for (i = 0; i < 5; i++) {
        sprintf(filename, "new-door/%d.pcx", i + 1);
        images[i] = icache.GetImage(filename, pal);
    }

    current_image = 0;

    height = images[0]->h;
    width = images[0]->w;
    x += TILE_WIDTH / 2;
    x -= width / 2;
    y -= height - 2; //hehe
    coll_x = 0;
    coll_y = 0;
    coll_width = width;
    coll_height = height;

    energy = 1;
    strength = 0;
    speed_x = 0;
    // Fall to the floor
    speed_y = 5;
    speed_z = 0;
    counter = 0;

    friends = FRIEND_PLAYER;
    enemies = ~friends;
    me = FRIEND_DOOR;
    counter2 = 0;
    counter3 = 0;
    anim.reset();
}
/****************************************************************************/
Door_o::~Door_o()
    = default;

/****************************************************************************/
auto Door_o::update() -> int
{

    /*	if (counter3)
        	counter3--;
	if (counter)
        	counter--;

	if (!counter) {
        	current_image = current_image == 0 ? 1 : 0;
                counter = 10;
	}*/
    // Fall or Stop
    if (engine->check_floor(x, y + height, z) || engine->check_floor(x + width, y + height, z)) {
        speed_y = 0;
    } else {
        y += speed_y;
    }

    while (engine->check_floor(x + width / 2, y + height - 2, z)) {
        y--;
    }

    if (counter2 == 0 || counter2 == 2) {
        return 0;
    }

    current_image = anim.next_frame(5, FRAME_DELAY);

    if (current_image == 5) {
        current_image = 4;
        counter2 = 2;
        if (card == extra_level) {
            engine->PlayExtraLevel();
        } else {
            engine->ClearLevel();
        }
    }

    // Delete if already used
    if (energy == 0) {
        return -1;
    }
    return 0;
}

void Door_o::Collision(Object* o)
{
    char msg[512];
    if (energy == 0) {
        return;
    }

    if ((friends & o->GetWho()) == 0U) {
        return;
    }

    if (((dynamic_cast<player_o*>(o))->HaveCard(card) != 0) && (counter2 == 0)) {
        engine->PushMessage("Level Clear");
        engine->sound.PlaySFX("samples/fire4.wav");
        counter2 = 1;
        counter3 = 1;
    } else if (counter3 == 0) {
        switch (card) {
        case green:
            sprintf(msg, "Green access denied");
            break;
        case blue:
            sprintf(msg, "Blue access denied");
            break;
        case red:
            sprintf(msg, "Red access denied");
            break;
        case extra_level:
            engine->PushMessage("Extra level found");
            engine->sound.PlaySFX("samples/fire4.wav");
            counter2 = 1;
            return;
            break;
        case none:
        default:
            break;
        }
        engine->PushMessage(msg);
        //                counter3 = 240;
    }

    //	energy = 0;

    //        ((player_o *)o)->GiveCard(card);

    //        engine->ClearLevel();
}
