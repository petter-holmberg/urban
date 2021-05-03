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

#include <allegro.h>
#include <stdio.h>

#define CONTROLLER_NONE 0x00
#define CONTROLLER_FILE 0x01
#define CONTROLLER_KEYBOARD 0x02 // Ändra ej
#define CONTROLLER_JOYSTICK 0x03
#define CONTROLLER_SAVEDEMO 0x04
#define CONTROLLER_PLAYDEMO 0x05
#define CONTROLLER_GAMEPAD2 0x06
#define CONTROLLER_GAMEPAD4 0x07
#define CONTROLLER_GAMEPAD6 0x08
#define CONTROLLER_GAMEPAD8 0x09

#define JOYSTICK_NONE 0x00
#define JOYSTICK_UP 0x01
#define JOYSTICK_DOWN 0x02
#define JOYSTICK_LEFT 0x03
#define JOYSTICK_RIGHT 0x04
#define JOYSTICK_B1 0x05
#define JOYSTICK_B2 0x06
#define JOYSTICK_B3 0x07
#define JOYSTICK_B4 0x08
#define JOYSTICK_B5 0x09
#define JOYSTICK_B6 0x0A
#define JOYSTICK_B7 0x0B
#define JOYSTICK_B8 0x0C

class Controllers {
public:
    Controllers();
    virtual ~Controllers();

    virtual int up();
    virtual int down();
    virtual int left();
    virtual int right();
    virtual int fire();
    virtual int jump();

    virtual int weapon1();
    virtual int weapon2();
    virtual int weapon3();
    virtual int weapon4();
    virtual int weapon5();
    virtual int weapon6();
    virtual int weapon7();
    virtual int weapon8();
    virtual int weapon9();
    virtual int next_weapon();
    virtual int prev_weapon();
    virtual int update();
    //	protected:
    //        	int controller_type;
};

class Keyboard_ctrl : public Controllers {
public:
    Keyboard_ctrl();
    ~Keyboard_ctrl();
    int up();
    int down();
    int left();
    int right();
    int jump();
    int fire();
    int weapon1() { return key[KEY_1]; }
    int weapon2() { return key[KEY_2]; }
    int weapon3() { return key[KEY_3]; }
    int weapon4() { return key[KEY_4]; }
    int weapon5() { return key[KEY_5]; }
    int weapon6() { return key[KEY_6]; }
    int weapon7() { return key[KEY_7]; }
    int weapon8() { return key[KEY_8]; }
    int weapon9() { return key[KEY_9]; }
    int next_weapon();
    int prev_weapon();

private:
    volatile int key_up;
    volatile int key_down;
    volatile int key_left;
    volatile int key_right;
    volatile int key_jump;
    volatile int key_fire;
    volatile int key_next;
    volatile int key_prev;
};

class Gamepad_ctrl : public Controllers {
public:
    Gamepad_ctrl();
    ~Gamepad_ctrl();
    int up();
    int down();
    int left();
    int right();
    int jump();
    int fire();
    int next_weapon();
    int prev_weapon();
    int weapon1() { return key[KEY_1]; }
    int weapon2() { return key[KEY_2]; }
    int weapon3() { return key[KEY_3]; }
    int weapon4() { return key[KEY_4]; }
    int weapon5() { return key[KEY_5]; }
    int weapon6() { return key[KEY_6]; }
    int weapon7() { return key[KEY_7]; }
    int weapon8() { return key[KEY_8]; }
    int weapon9() { return key[KEY_9]; }

private:
    void set_key(int, volatile int**);
    volatile int* key_up;
    volatile int* key_down;
    volatile int* key_left;
    volatile int* key_right;
    volatile int* key_jump;
    volatile int* key_fire;
    volatile int* key_nextw;
    volatile int* key_prevw;
};
#endif
