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
#include <cstring>

/****************************************************************************/
inline constexpr auto POWERUP_STRENGTH = 100;
/****************************************************************************/
powerup_o::powerup_o(int X, int Y, int Z, int Type)
    : Object(X, Y, Z)
{
    PALETTE pal;
    char filename[512];

    type = Type;
    images = new BITMAP*;
    msg[0] = '\0';
    switch (type) {
    case POWERUP_ENERGY:
        sprintf(filename, "items/medipak.pcx");
        strength = -POWERUP_STRENGTH;
        sprintf(msg, "%d%% health", POWERUP_STRENGTH / 3);
        break;

    case POWERUP_EXTRA_LIFE:
        sprintf(filename, "items/xlife.pcx");
        strength = 0;
        sprintf(msg, "Extra life");
        break;

    case POWERUP_WEAPON_FLAME_THROWER:
        sprintf(filename, "items/flame.pcx");
        strength = 0;
        sprintf(msg, "Flamethrower\nLet's Burn!");
        break;

    case POWERUP_WEAPON_ICEMAKER:
        sprintf(filename, "items/ice.pcx");
        strength = 0;
        sprintf(msg, "IceMaker (tm)\nOrder your own at 555-URBAN");
        break;

    case POWERUP_WEAPON_PLASMA:
        sprintf(filename, "items/plasmal.pcx");
        strength = 0;
        sprintf(msg, "Plasma Gun\nHightech stuff");
        break;

    case POWERUP_WEAPON_MINIGUN:
        sprintf(filename, "items/minigun.pcx");
        strength = 0;
        sprintf(msg, "BIG MiniGun");
        break;

    case POWERUP_WEAPON_ELECTRIC:
        sprintf(filename, "items/electric.pcx");
        strength = 0;
        sprintf(msg, "Electrifier");
        break;

    case POWERUP_WEAPON_GRENADE_LAUNCHER:
        sprintf(filename, "items/granadel.pcx");
        strength = 0;
        sprintf(msg, "Rocket Launcher");
        break;

    case POWERUP_AMMO_FLAME_THROWER:
        sprintf(filename, "items/flamea.pcx");
        strength = 0;
        sprintf(msg, "More Ammo");
        break;
    case POWERUP_AMMO_ICEMAKER:
        sprintf(filename, "items/icea.pcx");
        strength = 0;
        sprintf(msg, "More Ammo");
        break;

    case POWERUP_AMMO_GRENADE_LAUNCHER:
        sprintf(filename, "items/granade.pcx");
        strength = 0;
        sprintf(msg, "More Ammo");
        break;

    case POWERUP_AMMO_GRENADE_LAUNCHER6:
        sprintf(filename, "items/granadea.pcx");
        strength = 0;
        sprintf(msg, "More Ammo");
        break;

    case POWERUP_AMMO_PLASMA:
        sprintf(filename, "items/plasmaa.pcx");
        strength = 0;
        sprintf(msg, "More Ammo");
        break;

    case POWERUP_AMMO_MINIGUN:
        sprintf(filename, "items/miniguna.pcx");
        strength = 0;
        sprintf(msg, "More Ammo");
        break;

    case POWERUP_AMMO_ELECTRIC:
        sprintf(filename, "items/ela.pcx");
        strength = 0;
        sprintf(msg, "More Ammo");
        break;

    default:
        sprintf(filename, "oopsUnkonwnpowerup.pcx");
        strength = 0;
        break;
    }
    images[0] = icache.GetImage(filename, pal);
    if (images[0] != nullptr) {
        num_images++;
    }

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
    speed_x = 0;
    // Fall to the floor
    speed_y = 5;
    speed_z = 0;

    //	evil = 1;
    friends = ~FRIEND_PLAYER;
    enemies = FRIEND_PLAYER;
    me = FRIEND_POWERUP;
}
/****************************************************************************/
powerup_o::~powerup_o()
    = default;

/****************************************************************************/
auto powerup_o::update() -> int
{
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

void powerup_o::Collision(Object* o)
{
    if (energy == 0) {
        return;
    }

    if ((friends & o->GetWho()) != 0U) {
        return;
    }
    ENGINE.PushMessage(msg);

    if (o->GetWho() == FRIEND_PLAYER) {

        energy = 0;

        switch (type) {
        case POWERUP_EXTRA_LIFE:
            (dynamic_cast<player_o*>(PLAYER))->AddLife();
            break;
        case POWERUP_WEAPON_FLAME_THROWER:
            if ((dynamic_cast<player_o*>(PLAYER))->HaveWeapon(FLAME_THROWER) != 0) {
                energy = 1;
                break;
            }
            (dynamic_cast<player_o*>(PLAYER))->EnableWeapon(FLAME_THROWER);
            break;
        case POWERUP_WEAPON_ICEMAKER:
            if ((dynamic_cast<player_o*>(PLAYER))->HaveWeapon(ICEMAKER) != 0) {
                energy = 1;
                break;
            }
            (dynamic_cast<player_o*>(PLAYER))->EnableWeapon(ICEMAKER);
            break;

        case POWERUP_WEAPON_GRENADE_LAUNCHER:
            if ((dynamic_cast<player_o*>(PLAYER))->HaveWeapon(GRENADE_LAUNCHER) != 0) {
                energy = 1;
                break;
            }
            (dynamic_cast<player_o*>(PLAYER))->EnableWeapon(GRENADE_LAUNCHER);
            break;

        case POWERUP_WEAPON_PLASMA:
            if ((dynamic_cast<player_o*>(PLAYER))->HaveWeapon(PLASMA_GUN) != 0) {
                energy = 1;
                break;
            }
            (dynamic_cast<player_o*>(PLAYER))->EnableWeapon(PLASMA_GUN);
            break;

        case POWERUP_WEAPON_ELECTRIC:
            if ((dynamic_cast<player_o*>(PLAYER))->HaveWeapon(ELECTRIC) != 0) {
                energy = 1;
                break;
            }
            (dynamic_cast<player_o*>(PLAYER))->EnableWeapon(ELECTRIC);
            break;

        case POWERUP_WEAPON_MINIGUN:
            if ((dynamic_cast<player_o*>(PLAYER))->HaveWeapon(MINIGUN) != 0) {
                energy = 1;
                break;
            }
            (dynamic_cast<player_o*>(PLAYER))->EnableWeapon(MINIGUN);
            break;

        case POWERUP_AMMO_FLAME_THROWER:

            (dynamic_cast<player_o*>(PLAYER))->AddAmmo(FLAME_THROWER, 50);
            break;

        case POWERUP_AMMO_ICEMAKER:

            (dynamic_cast<player_o*>(PLAYER))->AddAmmo(ICEMAKER, 50);
            break;

        case POWERUP_AMMO_GRENADE_LAUNCHER:

            (dynamic_cast<player_o*>(PLAYER))->AddAmmo(GRENADE_LAUNCHER, 1);
            break;

        case POWERUP_AMMO_GRENADE_LAUNCHER6:

            (dynamic_cast<player_o*>(PLAYER))->AddAmmo(GRENADE_LAUNCHER, 6);
            break;

        case POWERUP_AMMO_PLASMA:

            (dynamic_cast<player_o*>(PLAYER))->AddAmmo(PLASMA_GUN, 20);
            break;

        case POWERUP_AMMO_MINIGUN:

            (dynamic_cast<player_o*>(PLAYER))->AddAmmo(MINIGUN, 100);
            break;

        case POWERUP_AMMO_ELECTRIC:

            (dynamic_cast<player_o*>(PLAYER))->AddAmmo(ELECTRIC, 20);
            break;

        default:
            break;
        }
    }
}
