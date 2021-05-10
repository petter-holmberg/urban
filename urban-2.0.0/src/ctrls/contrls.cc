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
#include "ctrls.h"

Controllers::Controllers()
{
    //	controller_type = CONTROLLER_NONE;
}

Controllers::~Controllers()
    = default;

auto Controllers::up() -> bool
{
    return 0;
}

auto Controllers::down() -> bool
{
    return 0;
}

auto Controllers::left() -> bool
{
    return 0;
}

auto Controllers::right() -> bool
{
    return 0;
}

auto Controllers::jump() -> bool
{
    return 0;
}

auto Controllers::fire() -> bool
{
    return 0;
}

auto Controllers::weapon1() -> bool
{
    return 0;
}

auto Controllers::weapon2() -> bool
{
    return 0;
}

auto Controllers::weapon3() -> bool
{
    return 0;
}

auto Controllers::weapon4() -> bool
{
    return 0;
}

auto Controllers::weapon5() -> bool
{
    return 0;
}

auto Controllers::weapon6() -> bool
{
    return 0;
}

auto Controllers::weapon7() -> bool
{
    return 0;
}

auto Controllers::weapon8() -> bool
{
    return 0;
}

auto Controllers::weapon9() -> bool
{
    return 0;
}

auto Controllers::next_weapon() -> bool
{
    return 0;
}

auto Controllers::prev_weapon() -> bool
{
    return 0;
}

auto Controllers::update() -> int
{
    return 0;
}
