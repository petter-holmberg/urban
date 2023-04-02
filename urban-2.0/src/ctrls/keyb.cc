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

inline constexpr auto USE_CONFIG_KEYS = true;

Keyboard_ctrl::Keyboard_ctrl()
{
    //	controller_type = CONTROLLER_KEYBOARD;
    if constexpr (USE_CONFIG_KEYS) {
        key_up = config->keyconf.key_up;
        key_down = config->keyconf.key_down;
        key_left = config->keyconf.key_left;
        key_right = config->keyconf.key_right;
        key_jump = config->keyconf.key_jump;
        key_fire = config->keyconf.key_fire;
        key_next = config->keyconf.key_prevweapon;
        key_prev = config->keyconf.key_nextweapon;
        if (key_next == scan_code::NONE) {
            key_next = scan_code::KEY_INSERT;
        }
        if (key_prev == scan_code::NONE) {
            key_prev = scan_code::KEY_DEL;
        }
    } else {
        key_up = scan_code::KEY_UP;
        key_down = scan_code::KEY_DOWN;
        key_right = scan_code::KEY_RIGHT;
        key_left = scan_code::KEY_LEFT;
        key_jump = scan_code::KEY_SPACE;
        key_fire = scan_code::KEY_LCONTROL;
        key_next = scan_code::KEY_INSERT;
        key_prev = scan_code::KEY_DEL;
    }
}

Keyboard_ctrl::~Keyboard_ctrl()
    = default;

auto Keyboard_ctrl::up() -> bool
{
    return key[static_cast<size_t>(key_up)];
}

auto Keyboard_ctrl::down() -> bool
{
    return key[static_cast<size_t>(key_down)];
}

auto Keyboard_ctrl::left() -> bool
{
    return key[static_cast<size_t>(key_left)];
}

auto Keyboard_ctrl::right() -> bool
{
    return key[static_cast<size_t>(key_right)];
}

auto Keyboard_ctrl::jump() -> bool
{
    return key[static_cast<size_t>(key_jump)];
}

auto Keyboard_ctrl::fire() -> bool
{
    return key[static_cast<size_t>(key_fire)];
}

auto Keyboard_ctrl::next_weapon() -> bool
{
    static int pressed = 0;

    if (!key[static_cast<size_t>(key_next)]) {
        pressed = 0;
    }

    if (!key[static_cast<size_t>(key_next)]) {
        if (pressed != 0) {
            return false;
        }
        pressed = 1;
    }

    return key[static_cast<size_t>(key_next)];
}

auto Keyboard_ctrl::prev_weapon() -> bool
{
    static int pressed = 0;

    if (!key[static_cast<size_t>(key_prev)]) {
        pressed = 0;
    }

    if (!key[static_cast<size_t>(key_prev)]) {
        if (pressed != 0) {
            return false;
        }
        pressed = 1;
    }

    return key[static_cast<size_t>(key_prev)];
}
