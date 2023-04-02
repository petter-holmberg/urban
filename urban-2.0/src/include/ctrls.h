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
#ifndef __CTRLS_H__
#define __CTRLS_H__

#include "allegro.h"
#include "keyinfo.h"

inline constexpr auto CONTROLLER_NONE = 0x00;
inline constexpr auto CONTROLLER_FILE = 0x01;
inline constexpr auto CONTROLLER_KEYBOARD = 0x02; // Do not change
inline constexpr auto CONTROLLER_JOYSTICK = 0x03;
inline constexpr auto CONTROLLER_SAVEDEMO = 0x04;
inline constexpr auto CONTROLLER_PLAYDEMO = 0x05;
inline constexpr auto CONTROLLER_GAMEPAD2 = 0x06;
inline constexpr auto CONTROLLER_GAMEPAD4 = 0x07;
inline constexpr auto CONTROLLER_GAMEPAD6 = 0x08;
inline constexpr auto CONTROLLER_GAMEPAD8 = 0x09;

inline constexpr auto JOYSTICK_NONE = 0x00;
inline constexpr auto JOYSTICK_UP = 0x01;
inline constexpr auto JOYSTICK_DOWN = 0x02;
inline constexpr auto JOYSTICK_LEFT = 0x03;
inline constexpr auto JOYSTICK_RIGHT = 0x04;
inline constexpr auto JOYSTICK_B1 = 0x05;
inline constexpr auto JOYSTICK_B2 = 0x06;
inline constexpr auto JOYSTICK_B3 = 0x07;
inline constexpr auto JOYSTICK_B4 = 0x08;
inline constexpr auto JOYSTICK_B5 = 0x09;
inline constexpr auto JOYSTICK_B6 = 0x0A;
inline constexpr auto JOYSTICK_B7 = 0x0B;
inline constexpr auto JOYSTICK_B8 = 0x0C;

class Controllers {
public:
    Controllers();
    virtual ~Controllers();

    virtual bool up();
    virtual bool down();
    virtual bool left();
    virtual bool right();
    virtual bool fire();
    virtual bool jump();

    virtual bool weapon1();
    virtual bool weapon2();
    virtual bool weapon3();
    virtual bool weapon4();
    virtual bool weapon5();
    virtual bool weapon6();
    virtual bool weapon7();
    virtual bool weapon8();
    virtual bool weapon9();
    virtual bool next_weapon();
    virtual bool prev_weapon();
    virtual int update();
    //	protected:
    //        	int controller_type;
};

class Keyboard_ctrl : public Controllers {
public:
    Keyboard_ctrl();
    ~Keyboard_ctrl();
    bool up();
    bool down();
    bool left();
    bool right();
    bool jump();
    bool fire();
    bool weapon1() { return key[static_cast<size_t>(scan_code::KEY_1)]; }
    bool weapon2() { return key[static_cast<size_t>(scan_code::KEY_2)]; }
    bool weapon3() { return key[static_cast<size_t>(scan_code::KEY_3)]; }
    bool weapon4() { return key[static_cast<size_t>(scan_code::KEY_4)]; }
    bool weapon5() { return key[static_cast<size_t>(scan_code::KEY_5)]; }
    bool weapon6() { return key[static_cast<size_t>(scan_code::KEY_6)]; }
    bool weapon7() { return key[static_cast<size_t>(scan_code::KEY_7)]; }
    bool weapon8() { return key[static_cast<size_t>(scan_code::KEY_8)]; }
    bool weapon9() { return key[static_cast<size_t>(scan_code::KEY_9)]; }
    bool next_weapon();
    bool prev_weapon();

private:
    scan_code key_up {scan_code::INVALID};
    scan_code key_down {scan_code::INVALID};
    scan_code key_left {scan_code::INVALID};
    scan_code key_right {scan_code::INVALID};
    scan_code key_jump {scan_code::INVALID};
    scan_code key_fire {scan_code::INVALID};
    scan_code key_next {scan_code::INVALID};
    scan_code key_prev {scan_code::INVALID};
};

class Gamepad_ctrl : public Controllers {
public:
    Gamepad_ctrl();
    ~Gamepad_ctrl();
    Gamepad_ctrl(const Gamepad_ctrl&) = delete;
    Gamepad_ctrl& operator=(const Gamepad_ctrl&) = delete;
    bool up();
    bool down();
    bool left();
    bool right();
    bool jump();
    bool fire();
    bool next_weapon();
    bool prev_weapon();
    bool weapon1() { return key[static_cast<size_t>(scan_code::KEY_1)]; }
    bool weapon2() { return key[static_cast<size_t>(scan_code::KEY_2)]; }
    bool weapon3() { return key[static_cast<size_t>(scan_code::KEY_3)]; }
    bool weapon4() { return key[static_cast<size_t>(scan_code::KEY_4)]; }
    bool weapon5() { return key[static_cast<size_t>(scan_code::KEY_5)]; }
    bool weapon6() { return key[static_cast<size_t>(scan_code::KEY_6)]; }
    bool weapon7() { return key[static_cast<size_t>(scan_code::KEY_7)]; }
    bool weapon8() { return key[static_cast<size_t>(scan_code::KEY_8)]; }
    bool weapon9() { return key[static_cast<size_t>(scan_code::KEY_9)]; }

private:
    void set_key(int, int**);
    int* key_up;
    int* key_down;
    int* key_left;
    int* key_right;
    int* key_jump;
    int* key_fire;
    int* key_nextw;
    int* key_prevw;
};
#endif
