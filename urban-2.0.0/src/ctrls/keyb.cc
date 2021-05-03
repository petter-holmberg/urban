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
#include "config.h"
#include "ctrls.h"
#include <allegro.h>

extern Config* config;

#define __USE_CONFIG_KEYS

Keyboard_ctrl::Keyboard_ctrl()
    : Controllers()
{
//	controller_type = CONTROLLER_KEYBOARD;
#ifdef __USE_CONFIG_KEYS
    key_up = config->keyconf.key_up;
    key_down = config->keyconf.key_down;
    key_left = config->keyconf.key_left;
    key_right = config->keyconf.key_right;
    key_jump = config->keyconf.key_jump;
    key_fire = config->keyconf.key_fire;
    key_next = config->keyconf.key_prevweapon;
    key_prev = config->keyconf.key_nextweapon;
    if (!key_next)
        key_next = KEY_INSERT;
    if (!key_prev)
        key_prev = KEY_DEL;
#else
    key_up = KEY_UP;
    key_down = KEY_DOWN;
    key_right = KEY_RIGHT;
    key_left = KEY_LEFT;
    key_jump = KEY_SPACE;
    key_fire = KEY_LCONTROL;
    key_next = KEY_INSERT;
    key_prev = KEY_DEL;
#endif
}

Keyboard_ctrl::~Keyboard_ctrl()
{
}

int Keyboard_ctrl::up()
{
    return key[key_up];
}

int Keyboard_ctrl::down()
{
    return key[key_down];
}

int Keyboard_ctrl::left()
{
    return key[key_left];
}

int Keyboard_ctrl::right()
{
    return key[key_right];
}

int Keyboard_ctrl::jump()
{
    return key[key_jump];
}

int Keyboard_ctrl::fire()
{
    return key[key_fire];
}

int Keyboard_ctrl::next_weapon()
{
    static int pressed = 0;

    if (!key[key_next])
        pressed = 0;

    if (key[key_next])
        if (pressed)
            return 0;
        else
            pressed = 1;

    return key[key_next];
}

int Keyboard_ctrl::prev_weapon()
{
    static int pressed = 0;

    if (!key[key_prev])
        pressed = 0;

    if (key[key_prev])
        if (pressed)
            return 0;
        else
            pressed = 1;

    return key[key_prev];
}
