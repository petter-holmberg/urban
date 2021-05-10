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
card_o::card_o(int X, int Y, int Z, Cardtype Card)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];

    images.resize(2, nullptr);
    card = Card;
    switch (Card) {
    case blue:
        sprintf(filename, "items/bluecard.pcx");
        break;
    case green:
        sprintf(filename, "items/greencar.pcx");
        break;
    case red:
        sprintf(filename, "items/redcard.pcx");
        break;

    default:
        fprintf(stderr, "Unknown Card\n");
        exit(1);
    }
    images[0] = icache.GetImage(filename, pal);
    sprintf(filename, "mine2.pcx");
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

    energy = 1;
    strength = 0;
    speed_x = 0;
    // Fall to the floor
    speed_y = 5;
    speed_z = 0;
    counter = 0;

    friends = FRIEND_PLAYER;
    enemies = ~friends;
    me = FRIEND_CARD;
}
/****************************************************************************/
card_o::~card_o()
    = default;

/****************************************************************************/
auto card_o::update() -> int
{
    if (counter != 0) {
        counter--;
    }
    if (counter == 0) {
        current_image = current_image == 0 ? 1 : 0;
        counter = 10;
    }
    // Fall or Stop
    if (ENGINE.check_floor(x, y + height, z) || ENGINE.check_floor(x + width, y + height, z)) {
        speed_y = 0;
    } else {
        y += speed_y;
    }

    // Delete if already used
    if (energy == 0) {
        return -1;
    }
    return 0;
}

void card_o::Collision(Object* o)
{
    if (energy == 0) {
        return;
    }

    if ((friends & o->GetWho()) == 0U) {
        return;
    }
    energy = 0;

    (dynamic_cast<player_o*>(o))->GiveCard(card);
    switch (card) {
    case green:
        ENGINE.PushMessage("Green access granted");
        break;
    case red:
        ENGINE.PushMessage("Red access granted");
        break;
    case blue:
        ENGINE.PushMessage("Blue access granted");
        break;
    case none:
    default:
        break;
    }

    //        ENGINE.ClearLevel();
}
