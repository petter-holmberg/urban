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
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "ctrls.h"
#include "keyinfo.h"

inline constexpr auto QUALITY_NORMAL = 1;
inline constexpr auto QUALITY_HIGH = 2;

struct KeyConf {
    scan_code key_up = scan_code::KEY_UP;
    scan_code key_down = scan_code::KEY_DOWN;
    scan_code key_left = scan_code::KEY_LEFT;
    scan_code key_right = scan_code::KEY_RIGHT;
    scan_code key_jump = scan_code::KEY_SPACE;
    scan_code key_fire = scan_code::KEY_LCONTROL;
    scan_code key_prevweapon = scan_code::KEY_LSHIFT;
    scan_code key_nextweapon = scan_code::KEY_RSHIFT;
};

class Config {
public:
    Config();
    void Start();
    KeyConf keyconf;
    int ctrl_type = CONTROLLER_KEYBOARD;
    int music_vol = 90;
    int sfx_vol = 128;
    int gfx_quality = QUALITY_NORMAL;

private:
    void StartControls();
    void ChangeGFXQuality();
    void ChangeMusicVol();
    void ChangeSFXVol();
    static scan_code GetKey(const char* Label);
    int GetButton(const char* Label);
    void ConfigureKeyboard();
    void ConfigureGamepad();
    void Load();
    void Save();
};

#endif
