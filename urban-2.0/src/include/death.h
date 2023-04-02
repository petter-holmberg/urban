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
#ifndef __DEATH_H__
#define __DEATH_H__

#include "engine.h"

inline constexpr auto SPLAT_SOUND = "samples/gutsplat.wav";

template <typename T>
constexpr auto _X_(T x, T width) {
    return x + width / 2;
}

template <typename T>
constexpr auto _Y_(T y, T height) {
    return y + height / 2;
}

inline void DEATH_BY_EXPLOSION(int x, int width, int y, int height, int z) {
int r = random() % 3;
int i = 0;
for(i = 0;i < 10;i++)
	engine->create_effect(new blood_o(_X_(x, width), y, z, random() % 8 + 2, -(random() % 4) - 2));
for(i = 0;i < 10;i++)
	engine->create_effect(new blood_o(_X_(x, width), y, z, -(random() % 8) - 2, -(random() % 4) - 2));
switch (r) {
	case 0:
		engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/arm", 5, -4, -6, 0));
		engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/arm", 5, 4, -6, 0));
		engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/head", 5, 2, -4, 0));
		engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/leg", 5, -1, -2, 0));
		engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/leg", 5, 3, -4, 0));
		engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/ribs", 5, 0, 0, 0));
		for (i = 0;i < 5;i++)                                                         \
			engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/meat", 3, random() % 5, -(random() % 6), 0));
	        engine->sound.PlaySFX(SPLAT_SOUND);
        	break;
        case 1:
		engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/head", 5, 2, -4, 0));
		engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/ribs", 5, 0, 0, 0));
		for (i = 0;i < 5;i++)                                                         \
			engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/meat", 3, random() % 4, -(random() % 4) - 2, 0));
		for (i = 0;i < 5;i++)                                                         \
			engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/meat", 3, -(random() % 4), -(random() % 4) - 2, 0));
	        engine->sound.PlaySFX(SPLAT_SOUND);
        	break;
        case 2:
        	for (i = 0;i < 3;i++)
			engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/bigmeat", 5, random() % 5, -(random() % 4) - 3, 0));
        	for (i = 0;i < 3;i++)
			engine->create_effect(new bodyparts_o(_X_(x, width), _Y_(y, height), z, "meat/bigmeat", 5, -(random() % 5), -(random() % 4) - 3, 0));
	        engine->sound.PlaySFX(SPLAT_SOUND);
        	break;
        case 3:
        default:
        	break;
}
}

#endif
