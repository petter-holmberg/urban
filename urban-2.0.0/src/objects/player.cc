/******************************************************************************

    URBAN
    Copyright (C) 1999  Thomas Nyberg and Jonas Borgstr�m

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
    Thomas Nyberg					Jonas Borgstr�m
    Br�nnbergsv�gen 21b					Bondev�gen 6
    92136 Lycksele					92332 Storuman
    Sweden						Sweden

    or email:

    thomas.nyberg@usa.net				jonas_b@bitsmart.com
*****************************************************************************/
#include <stdlib.h>
#include <allegro.h>
#include "object.h"
#include "object2.h"
#include "ctrls.h"
#include "engine.h"
#include "ctrls.h"
#include "config.h"
#include "death.h"
#include "cheat.h"

#define FIRE_SAMPLE "samples/flame.wav"
#define ELEVATOR(x) ((Elevator_o *)x)
#define ELEVATORSTATION(x) ((ElevatorStation_o *)x)

#define FALL_DAMAGE		20
#define FALL_DAMAGE_BEGIN	30

#define X_FRICTION	1
#define Z_FRICTION	X_FRICTION
#define MAX_X_SPEED	(mode == MODE_NORMAL ? 3 : 2) //4
#define MIN_X_SPEED	-MAX_X_SPEED //-4
#define MAX_Y_SPEED	(mode == MODE_NORMAL ? 16 : 8)
#define MIN_Y_SPEED 	(mode == MODE_NORMAL ? -12 : -6)
#define	MAX_Z_SPEED	(mode == MODE_NORMAL ? 2 : 1) //MAX_X_SPEED
#define MIN_Z_SPEED	-MAX_Z_SPEED
#define X_ACCEL		(mode == MODE_NORMAL ? 2 : 1) //2
#define Y_ACCEL		1
#define Z_ACCEL         (mode == MODE_NORMAL ? 2 : 1)
#define LIFE_IMMORTAL   0xfffffff
#define LEFT_FOOT_X	30
#define RIGHT_FOOT_X	32
#define FOOT_Y		66

#define DEATH_EXPLOSION	0x01
#define DEATH_ICEBALL	0x02
#define DEATH_FIREBALL	0x03
#define DEATH_BULLET	0x04

#define STATE_NONE	0x00
#define STATE_WALK	0x01
#define STATE_TURN	0x02
#define STATE_FIRE	0x04
#define STATE_FIRING	0x08
#define STATE_HIT	0x10
#define STATE_DEAD	0x20
#define STATE_FIREDELAY	0x40
#define STATE_IMMORTAL	0x80

#define FRAME_DELAY 	(mode == MODE_NORMAL ? 4 : 8)
#define IMMORTAL_DELAY	200
#define TOUCH_DELAY	10

#define EL_LENGTH	5

#define SAVE_DELAY	1000
int temp = 0;
int temp2 = 0;
int fall_counter = 0;

extern Config *config;
#define NUM_WEAPONS 	7 //OBS!!! �ndra i ENGINE.CC ocks�!!!
#define NUM_LIFES 	3

#if defined(ALLOW_Fx_CHEATING) && defined(__DISTRIBUTION__)
#	warning ALLOW_Fx_CHEATING defined and compiling for distribution
#endif

Weapon Weapons[NUM_WEAPONS] = {
        {{6, 7, 8, 9, 10, 11},
        {0, 1, 2, 3, 4, 5},
        {18, 19, 20, 21, 22, 23},
        {12, 13, 14, 15, 16, 17},
        {30, 31, 32, 33, 34, 35},
        {24, 25, 26, 27, 28, 29},
//        {5}, {25}, {-1}, {1}},
        {3}, {10}, {-1}, {1}, {1}, {-1}},
        /* Flame thrower*/
/*	int walk_left[6];
        int walk_right[6];
        int walk_up[6];
        int walk_down[6];
        int fire_left[6];
        int fire_right[6];
        int fire_rep;
	int fire_delay;*/
        {{54, 55, 56, 57, 58, 59},
        {48, 49, 50, 51, 52, 53},
        {18, 19, 20, 21, 22, 23},
        {60, 61, 62, 63, 64, 65},
//        {12, 13, 14, 15, 16, 17},
        {42, 43, 44, 45, 46, 47},
        {36, 37, 38, 39, 40, 41},
        {5}, {8}, {0}, {0}, {0}, {500}},
        /* IceMaker(tm)*/
        {{84, 85, 86, 87, 88, 89},
        {78, 79, 80, 81, 82, 83},
        {18, 19, 20, 21, 22, 23},
        {90, 91, 92, 93, 94, 95},
//        {12, 13, 14, 15, 16, 17},
        {72, 73, 74, 75, 76, 77},
        {66, 67, 68, 69, 70, 71},
        {5}, {8}, {0}, {0}, {0}, {500}},
	/* Grenade launcher */
        {{102, 103, 104, 105, 106, 107},
        {96, 97, 98, 99, 100, 101},
        {114, 115, 116, 117, 118, 119},
        {108, 109, 110, 111, 112, 113},
        {126, 127, 128, 129, 130, 131},
        {120, 121, 122, 123, 124, 125},
//        {5}, {25}, {-1}, {1}},
        {3}, {60}, {0}, {0}, {1}, {32}},
	/* Plasma Weapon */
        {{138, 139, 140, 141, 142, 143},
        {132, 133, 134, 135, 136, 137},
        {150, 151, 152, 153, 154, 155},
        {144, 145, 146, 147, 148, 149},
        {162, 163, 164, 165, 166, 167},
        {156, 157, 158, 159, 160, 161},
        {3}, {5}, {0}, {0}, {0}, {100}},
        /* Minigun */
        {{174, 175, 176, 177, 178, 179},
        {168, 169, 170, 171, 172, 173},
        {186, 187, 188, 189, 190, 191},
        {180, 181, 182, 183, 184, 185},
        {198, 199, 200, 201, 202, 203},
        {192, 193, 194, 195, 196, 197},
        {1}, {1}, {0}, {0}, {1}, {1000}},
        /* Electric */
        {{210, 211, 212, 213, 214, 215},
        {204, 205, 206, 207, 208, 209},
        {222, 223, 224, 225, 226, 227},
        {216, 217, 218, 219, 220, 221},
        {234, 235, 236, 237, 238, 239},
        {228, 229, 230, 231, 232, 233},
        {6}, {6}, {0}, {0}, {0}, {100}}
};

/*struct Weapon {
	int walk_left[6];
        int walk_right[6];
        int walk_up[6];
        int walk_down[6];
        int fire_left[6];
        int fire_right[6];
        int fire_rep;
	int fire_delay;
	long ammo;
        char availible;
};*/

#define CURRENT_WEAPON weapon[current_weapon]
#define WEAPON_AVAILIBLE (CURRENT_WEAPON.ammo && (mode == MODE_NORMAL ? 1 : CURRENT_WEAPON.waterproof))

#define MAX_ENERGY 300
static PALETTE pal;
/**************************************************************************/
player_o::player_o(int X, int Y, int Z, int controls) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];

        lives = NUM_LIFES;

        control_type = controls;

        switch(config->keyconf.ctrl_type) {
        	case CONTROLLER_KEYBOARD:
		        ctrl = new Keyboard_ctrl();
	                break;

#ifdef DJGPP
		case CONTROLLER_GAMEPAD2:
		case CONTROLLER_GAMEPAD4:
		case CONTROLLER_GAMEPAD6:
		case CONTROLLER_GAMEPAD8:
		case CONTROLLER_JOYSTICK:
                	ctrl = new Gamepad_ctrl();
                        break;
#endif
        };

//	images = (BITMAP **)malloc(205 * sizeof(BITMAP *));
	images = new BITMAP*[241];

        for(int i = 0;i<6;i++) {
        	sprintf(filename, "urban/h/%d.pcx", i + 1);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 6;i<12;i++) {
        	sprintf(filename, "urban/v/%d.pcx", i-5);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 12;i<18;i++) {
        	sprintf(filename, "urban/f/%d.pcx", i-11);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 18;i<24;i++) {
        	sprintf(filename, "urban/b/%d.pcx", i - 17);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
/* Skjutbilder */
//Hagelgev�r
        for(int i = 24;i<30;i++) {
        	sprintf(filename, "urban/h/%ds.pcx", i - 23);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 30;i<36;i++) {
        	sprintf(filename, "urban/v/%ds.pcx", i - 29);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
//Eldkastare
        for(int i = 36;i<42;i++) {
        	sprintf(filename, "urban/flame_h/%ds.pcx", i - 35);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 42;i<48;i++) {
        	sprintf(filename, "urban/flame_v/%ds.pcx", i - 41);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 48;i<54;i++) {
        	sprintf(filename, "urban/flame_h/%d.pcx", i - 47);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 54;i<60;i++) {
        	sprintf(filename, "urban/flame_v/%d.pcx", i - 53);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 60;i<66;i++) {
        	sprintf(filename, "urban/flame_f/%d.pcx", i - 59);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}

//IceMaker(tm)
        for(int i = 66;i<72;i++) {
        	sprintf(filename, "urban/ice/urbhoger/%ds.pcx", i - 65);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 72;i<78;i++) {
        	sprintf(filename, "urban/ice/urbvanst/%ds.pcx", i - 71);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 78;i<84;i++) {
        	sprintf(filename, "urban/ice/urbhoger/%d.pcx", i - 77);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 84;i<90;i++) {
        	sprintf(filename, "urban/ice/urbvanst/%d.pcx", i - 83);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 90;i<96;i++) {
        	sprintf(filename, "urban/ice/urbfram/%d.pcx", i - 89);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
//Grenade
        for(int i = 96;i<102;i++) {
        	sprintf(filename, "urban/granadel/urbhoger/%d.pcx", i - 95);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 102;i<108;i++) {
        	sprintf(filename, "urban/granadel/urbvanst/%d.pcx", i - 101);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 108;i<114;i++) {
        	sprintf(filename, "urban/granadel/urbfram/%d.pcx", i - 107);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 114;i<120;i++) {
        	sprintf(filename, "urban/granadel/urbbak/%d.pcx", i - 113);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 120;i<126;i++) {
        	sprintf(filename, "urban/granadel/urbhoger/%ds.pcx", i - 119);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 126;i<132;i++) {
        	sprintf(filename, "urban/granadel/urbvanst/%ds.pcx", i - 125);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}

//Plasma-weapon
        for(int i = 132;i<138;i++) {
        	sprintf(filename, "plasmal/urbhoger/%d.pcx", i - 131);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 138;i<144;i++) {
        	sprintf(filename, "plasmal/urbvanst/%d.pcx", i - 137);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 144;i<150;i++) {
        	sprintf(filename, "plasmal/urbfram/%d.pcx", i - 143);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 150;i<156;i++) {
        	sprintf(filename, "plasmal/urbbak/%d.pcx", i - 149);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 156;i<162;i++) {
        	sprintf(filename, "plasmal/urbhoger/%ds.pcx", i - 155);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 162;i<168;i++) {
        	sprintf(filename, "plasmal/urbvanst/%ds.pcx", i - 161);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
//Minigun
        for(int i = 168;i<174;i++) {
        	sprintf(filename, "urban/minigun/urbhoger/%d.pcx", i - 167);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 174;i<180;i++) {
        	sprintf(filename, "urban/minigun/urbvanst/%d.pcx", i - 173);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 180;i<186;i++) {
        	sprintf(filename, "urban/minigun/urbfram/%d.pcx", i - 179);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 186;i<192;i++) {
        	sprintf(filename, "urban/minigun/urbbak/%d.pcx", i - 185);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 192;i<198;i++) {
        	sprintf(filename, "urban/minigun/urbhoger/%ds.pcx", i - 191);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 198;i<204;i++) {
        	sprintf(filename, "urban/minigun/urbvanst/%ds.pcx", i - 197);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
//Electric
        for(int i = 204;i<210;i++) {
        	sprintf(filename, "urban/electric/urbhoger/%d.pcx", i - 203);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 210;i<216;i++) {
        	sprintf(filename, "urban/electric/urbvanst/%d.pcx", i - 209);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 216;i<222;i++) {
        	sprintf(filename, "urban/electric/urbfram/%d.pcx", i - 215);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 222;i<228;i++) {
        	sprintf(filename, "urban/electric/urbbak/%d.pcx", i - 221);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 228;i<234;i++) {
        	sprintf(filename, "urban/electric/urbhoger/%ds.pcx", i - 227);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
        for(int i = 234;i<240;i++) {
        	sprintf(filename, "urban/electric/urbvanst/%ds.pcx", i - 233);
                images[i] = icache.GetImage(filename, pal);
             	if(images[i])
			num_images++;
	}
       	sprintf(filename, "urban/empty.pcx");
        images[240] = icache.GetImage(filename, pal);
        if(images[240])
		num_images++;

/*	for (int i = 0;i < num_images;i++)
		rect(images[i], 0, 0, images[i]->w - 1, images[i]->h - 1, 15);*/

        weapon = new Weapon[NUM_WEAPONS];
        memcpy(weapon, Weapons, NUM_WEAPONS * sizeof(Weapon));


        current_weapon = SHOTGUN;

        current_image = 5;
        speed_y = 1;
	direction = RIGHT_DIR;
        height = images[5]->h;
        width = images[5]->w;
        coll_x = 31;
        coll_y = 0;
        coll_width = 20;
        coll_height = height;

        //st� med f�tterna
	y -= 66;
        energy = MAX_ENERGY;
        strength = 0;
        friends = FRIEND_PLAYER;
        enemies = ~FRIEND_PLAYER;
        me = FRIEND_PLAYER;
        anim.reset();
        state = STATE_NONE;
        counter = 0;
        counter2 = 0;
        counter3 = 0;
        counter4 = 0;
        counter5 = 0;	// Reborn counter
        save_x = x;
        save_y = y;
        save_z = z;
        card = 0;
        elevator = NULL;
        grav_counter = 0;
}

/**************************************************************************/
void player_o::MoveTo(int X, int Y, int Z) {
	x = X;
        y = Y;
	y -= 66;
        z = Z;
        save_x = x;
        save_y = y;
        save_z = z;
        card = 0;
        speed_x = 0;
        speed_y = 0;
        speed_z = 0;
}
/**************************************************************************/
int player_o::update() {
//7 & 9 �r avst�nden till f�tterna n�r gubben st�r stilla
/*
30,66 -32,66
32,66 -30,66
*/
	static int jumping = 0;
	static int jump_keypressed = 0;
        static int saved_mode = mode;

	if(counter5) {
        	counter5++;
	        // Fade in if the player has been reborn
	        if(counter5 == 20) {
	      		fade_in(ENGINE.map.GetPal(), 2);
	                counter5 = 0;
	        }
        }

        if(energy > MAX_ENERGY)
        	energy = MAX_ENERGY;

        if (counter)
        	counter--;
	if(state & STATE_DEAD) {
		if (!counter) {
			state |= STATE_IMMORTAL;
                	state &= ~STATE_DEAD;
	       		counter4 = IMMORTAL_DELAY;
		        counter2 = 0;
                	x = save_x;
	                y = save_y;
        	        z = save_z;
                	counter5 = 1;
	                get_palette(pal);
			fade_out(4);
	        }
                return 0;
	}
	if (counter2)
        	counter2--;
    // Fade to red if hurt
	if (counter4) {
/*    		RGB red;

           	red.g = 63 - ((counter4 - 1) * 2);
           	red.b = 63 - ((counter4 - 1) * 2);

            red.r = 63;
    		set_color(0, &red);*/
        	counter4--;
	}

//	if (!counter && state & STATE_FIRE)
//        	state &= ~STATE_FIRE;
        if(speed_x > 0)
        	speed_x -= X_FRICTION;
        else if(speed_x < 0)
        	speed_x += X_FRICTION;
	if (speed_z > 0)
        	speed_z -= Z_FRICTION;
	else if (speed_z < 0)
        	speed_z += Z_FRICTION;

	/* handle cheatcodes */
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_PLASMA)
		weapon[PLASMA_GUN].availible = 1;
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_FIRE)
		weapon[FLAME_THROWER].availible = 1;
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_ICE)
		weapon[ICEMAKER].availible = 1;
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_GRENADE)
		weapon[GRENADE_LAUNCHER].availible = 1;
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_MINIGUN)
		weapon[MINIGUN].availible = 1;
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_ELECTRIC)
		weapon[ELECTRIC].availible = 1;
	if (cheat_codes_active & CHEAT_MK_PLAYER_IMMORTAL) {
		state |= STATE_IMMORTAL;
       		counter4 = 5000;
	        SOUND.PlaySFX_Critical("samples/pathetic.wav");	
	}		
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_1K_OF_AMMO)
		CURRENT_WEAPON.ammo += 1000;
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_CARDS)
		card = (red | green | blue);				
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_1K_AMMO_2_ALL) {
		weapon[PLASMA_GUN].ammo += 1000;
		weapon[FLAME_THROWER].ammo += 1000;
		weapon[ICEMAKER].ammo += 1000;
		weapon[GRENADE_LAUNCHER].ammo += 1000;
		weapon[MINIGUN].ammo += 1000;
		weapon[ELECTRIC].ammo += 1000;		
	}		
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_MAX_AMMO) {
		weapon[PLASMA_GUN].ammo = weapon[PLASMA_GUN].max_ammo;
		weapon[FLAME_THROWER].ammo = weapon[FLAME_THROWER].max_ammo;
		weapon[ICEMAKER].ammo = weapon[ICEMAKER].max_ammo;
		weapon[GRENADE_LAUNCHER].ammo = weapon[GRENADE_LAUNCHER].max_ammo;
		weapon[MINIGUN].ammo = weapon[MINIGUN].max_ammo;
		weapon[ELECTRIC].ammo = weapon[ELECTRIC].max_ammo;		
	}			
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_HEALTH) {
	        energy = MAX_ENERGY;	
	}
	if (cheat_codes_active & CHEAT_GIVE_PLAYER_ALL_WEAPONS) {
		weapon[PLASMA_GUN].availible = 1;
		weapon[FLAME_THROWER].availible = 1;
		weapon[ICEMAKER].availible = 1;
		weapon[GRENADE_LAUNCHER].availible = 1;
		weapon[MINIGUN].availible = 1;
		weapon[ELECTRIC].availible = 1;
	}
	if (cheat_codes_active & CHEAT_SET_PLAYER_DEADLY)
		strength = 10;		
	if (cheat_codes_active & CHEAT_KILL_PLAYER) {
        	energy = 0;
                PerformDeath(DEATH_EXPLOSION);
		if (lives < 1)
			return -1;
		else			
			return 0;
	}	
			
#ifdef ALLOW_Fx_CHEATING
	if (key[KEY_F2])
        	weapon[PLASMA_GUN].availible = 1;
	if (key[KEY_F3])
        	weapon[ELECTRIC].availible = 1;
	if (key[KEY_F4])
        	weapon[MINIGUN].availible = 1;
	if (key[KEY_F5])
        	weapon[SHOTGUN].availible = 1;
	if (key[KEY_F6])
        	weapon[FLAME_THROWER].availible = 1;
	if (key[KEY_F7])
        	weapon[ICEMAKER].availible = 1;
	if (key[KEY_F8])
        	weapon[GRENADE_LAUNCHER].availible = 1;
	if (key[KEY_F9]){

		state |= STATE_IMMORTAL;
       		counter4 = 5000;
	        SOUND.PlaySFX_Critical("samples/pathetic.wav");
	}
	if (key[KEY_A])
        	if (CURRENT_WEAPON.ammo < CURRENT_WEAPON.max_ammo)
	                CURRENT_WEAPON.ammo++;
	if (key[KEY_Z])
        	if (CURRENT_WEAPON.ammo)
	        	CURRENT_WEAPON.ammo--;
#endif

/*	if (key[KEY_1] && weapon[SHOTGUN].availible)
        	current_weapon = SHOTGUN;
	else if (key[KEY_2] && weapon[FLAME_THROWER].availible)
        	current_weapon = FLAME_THROWER;
	else if (key[KEY_3] && weapon[ICEMAKER].availible)
        	current_weapon = ICEMAKER;
	else if (key[KEY_4] && weapon[GRENADE_LAUNCHER].availible)
        	current_weapon = GRENADE_LAUNCHER;
*/

	if (ctrl->weapon1() && weapon[SHOTGUN].availible)
        	current_weapon = SHOTGUN;
	else if (ctrl->weapon2() && weapon[FLAME_THROWER].availible)
        	current_weapon = FLAME_THROWER;
	else if (ctrl->weapon3() && weapon[ICEMAKER].availible)
        	current_weapon = ICEMAKER;
	else if (ctrl->weapon4() && weapon[GRENADE_LAUNCHER].availible)
        	current_weapon = GRENADE_LAUNCHER;
	else if (ctrl->weapon5() && weapon[PLASMA_GUN].availible)
        	current_weapon = PLASMA_GUN;
	else if (ctrl->weapon6() && weapon[MINIGUN].availible)
        	current_weapon = MINIGUN;
	else if (ctrl->weapon7() && weapon[ELECTRIC].availible)
        	current_weapon = ELECTRIC;

	if (ctrl->next_weapon()) {
		do {
	        	current_weapon++;
        	        if (current_weapon >= NUM_WEAPONS)
                		current_weapon = 0;
		} while (!weapon[current_weapon].availible);
	} else if (ctrl->prev_weapon()) {
        	do {
	                if (current_weapon == 0)
	                	current_weapon = NUM_WEAPONS;
        		current_weapon--;
		} while (!weapon[current_weapon].availible);
        }
        if (!counter) {
        	if (state & STATE_FIRE) {
                	state &= ~STATE_FIRE;
                        counter = CURRENT_WEAPON.fire_delay;
                } else if (ctrl->fire() && WEAPON_AVAILIBLE &&
//                } else if (ctrl->fire() && CURRENT_WEAPON.ammo && CURRENT_WEAPON.waterproof &&
                	direction != UP_DIR && direction != DOWN_DIR) {
	        	state |= STATE_FIRE;
        	        counter = CURRENT_WEAPON.fire_rep;
			CURRENT_WEAPON.ammo--;
                        switch (current_weapon) {
                        	case ELECTRIC:
                                	CURRENT_WEAPON.ammo -= 4;
                                        SOUND.PlaySFX("samples/el.wav");
                                        if (CURRENT_WEAPON.ammo < 0)
                                        	CURRENT_WEAPON.ammo = 0;
                                	for (int i = 0;i < EL_LENGTH;i++) {
                                        	int tmp;

                                        	if (direction == RIGHT_DIR)
							tmp = x + 67 + 32 * i;
						else
                                                	tmp = x - 32 * (i + 1);

						if (ENGINE.check_wall(x, y + 32, z))
                                                	break;

                                		ENGINE.create_object(new Beam_o(tmp, y + 32, z, friends));
					}
                                	break;
                        	case MINIGUN:
                                /* fallthrough */
                        	case SHOTGUN:

                        		if (direction == RIGHT_DIR) {
			                	ENGINE.create_effect(new shells_o(x + 36, y + 24, z, "soldier/hylsa", 1, -(random() % 5), -(random() % 8), 0));
						ENGINE.create_object(new HighSpeed_Bullet_o(x + 60, y + (height / 2), z, direction, friends));
			                } else {
						ENGINE.create_object(new HighSpeed_Bullet_o(x, y + (height / 2), z, direction, friends));
				        	ENGINE.create_effect(new shells_o(x + 28, y + 24, z, "soldier/hylsa", 1, random() % 5, -(random() % 8), 0));
		                	}

                                        break;
                                case FLAME_THROWER:
					SOUND.PlaySFX(FIRE_SAMPLE);
		                        ENGINE.create_object(new FireBall_o(x + (direction == RIGHT_DIR ? 67 : 20), y + 35, z, friends, speed_x == 0 ? direction == RIGHT_DIR ? 1 : -1 : speed_x, 0, 0));
                                        break;
                                case ICEMAKER:
					SOUND.PlaySFX(FIRE_SAMPLE);
		                        ENGINE.create_object(new IceBall_o(x + (direction == RIGHT_DIR ? 67 : 20), y + 35, z, friends, speed_x == 0 ? direction == RIGHT_DIR ? 1 : -1 : speed_x, 0, 0));
                                        break;
				case GRENADE_LAUNCHER:
					ENGINE.create_object(new rocket_o(x + (direction == RIGHT_DIR ? 67 : 20), y + 34, z, direction == RIGHT_DIR ? 5 : -5, mode));
                                	break;
				case PLASMA_GUN:
                                	SOUND.PlaySFX("samples/plasma3.wav");
                                	ENGINE.create_object(new Plasma_o(x + (direction == RIGHT_DIR ? 67 : 20), y + 28, z, direction, friends, 5));
                                	break;
			}
                } else if (ctrl->fire() && direction == UP_DIR) {
                	if (elev_station)
                        	ELEVATORSTATION(elev_station)->StartElevator();
                }
	}

        if (ctrl->right()) {
        	if (speed_x < MAX_X_SPEED)
                	speed_x += X_ACCEL;
		direction = RIGHT_DIR;
                state |= STATE_WALK;
	}
        if (ctrl->left()) {
        	if (speed_x > MIN_X_SPEED)
                	speed_x -= X_ACCEL;
		direction = LEFT_DIR;
                state |= STATE_WALK;
	}
        if (ctrl->up()) {
               	if (speed_z > MIN_Z_SPEED)
                	speed_z -= Z_ACCEL;
		direction = UP_DIR;
                state |= STATE_WALK;
                state &= ~STATE_FIRE;
        }
        if (ctrl->down()) {
               	if (speed_z < MAX_Z_SPEED)
                	speed_z += Z_ACCEL;
		direction = DOWN_DIR;
                state |= STATE_WALK;
                state &= ~STATE_FIRE;
        }

        if (ctrl->jump() && !jumping && !jump_keypressed) {
               	speed_y = MIN_Y_SPEED;
                jumping = 1;
		jump_keypressed = 1;
                saved_mode = mode;
        } else if (!ctrl->jump() && !jumping)
        	jump_keypressed = 0;

        if (state & STATE_WALK)
		switch (direction) {
                	case RIGHT_DIR:
                        	if (state & STATE_FIRE)
	                        	current_image = CURRENT_WEAPON.fire_right[anim.next_frame(4, FRAME_DELAY)];
                                else
	                        	current_image = CURRENT_WEAPON.walk_right[anim.next_frame(4, FRAME_DELAY)];
                                break;
                        case LEFT_DIR:
                        	if (state & STATE_FIRE)
	                        	current_image = CURRENT_WEAPON.fire_left[anim.next_frame(4, FRAME_DELAY)];
                                else
	                        	current_image = CURRENT_WEAPON.walk_left[anim.next_frame(4, FRAME_DELAY)];
                                break;
                        case UP_DIR:
                        	current_image = CURRENT_WEAPON.walk_up[anim.next_frame(4, FRAME_DELAY)];
                                break;
                        case DOWN_DIR:
                        	current_image = CURRENT_WEAPON.walk_down[anim.next_frame(4, FRAME_DELAY)];
                                break;
		}

        if (!speed_x && !speed_z) {
        	state &= ~STATE_WALK;
		switch (direction) {
                	case RIGHT_DIR:
                        	if (state & STATE_FIRE)
	                        	current_image = CURRENT_WEAPON.fire_right[5];
                                else
	                        	current_image = CURRENT_WEAPON.walk_right[5];
                                break;
                        case LEFT_DIR:
                        	if (state & STATE_FIRE)
	                        	current_image = CURRENT_WEAPON.fire_left[5];
                                else
	                        	current_image = CURRENT_WEAPON.walk_left[5];
                                break;
                        case UP_DIR:
                        	current_image = CURRENT_WEAPON.walk_up[5];
                                break;
                        case DOWN_DIR:
                        	current_image = CURRENT_WEAPON.walk_down[5];
                                break;
		}

	}

        // Fall or Stop
	if ((elevator != NULL && speed_y > 0) || ENGINE.check_floor(x + 31, y + 66 + speed_y, z)
	        || ENGINE.check_floor(x + width - 31, y + 66 + speed_y, z)) {

                if (mode == MODE_NORMAL)
	                if (fall_counter > FALL_DAMAGE_BEGIN)
        	        	energy -=  FALL_DAMAGE;
		fall_counter = 0;
        	speed_y = 0;
                jumping = 0;
                saved_mode = mode;
		if (energy <= 0) {
                	energy = 0;
                        PerformDeath(DEATH_EXPLOSION);
		}
        } else if (speed_y < MAX_Y_SPEED) {
        	if (saved_mode == MODE_NORMAL) {
	        	speed_y += Y_ACCEL;
		} else if (saved_mode == MODE_WATER) {
                	if (grav_counter == 4) {
	                	speed_y++;
                                grav_counter = 0;
			} else {
                        	grav_counter++;
			}
                }
	}

        if (speed_y > 0 && elevator != NULL) {
        	speed_y = 0;
                jumping = 0;
	}

        if (speed_y > 0) //r�knar hur "l�ngt" man ramlat
		fall_counter++;

        if (speed_y < 0)
        	if (ENGINE.check_floor(x + 31, y + speed_y, z)
                	|| ENGINE.check_floor(x + width - 31, y + speed_y, z)) {
                        	speed_y = 0;
                        }
// T�r
        if (speed_x > 0)
	        while (ENGINE.check_wall(x + width - 31 + speed_x, y + 3 * TILE_SIDE_HEIGHT, z ) && speed_x)
                	speed_x--;
	else if (speed_x < 0)
        	while (ENGINE.check_wall(x + 31 + speed_x, y + 3 * TILE_SIDE_HEIGHT, z ) && speed_x)
                	speed_x++;
/*        if (speed_x > 0)
	        while (ENGINE.check_wall(x + width - 31 + speed_x, y + 66, z ) && speed_x)
                	speed_x--;
	else if (speed_x < 0)
        	while (ENGINE.check_wall(x + 31 + speed_x, y + 66, z ) && speed_x)
                	speed_x++;*/
// Mage
        if (speed_x > 0)
	        while (ENGINE.check_wall(x + width - 31 + speed_x, y  + 2 * TILE_SIDE_HEIGHT, z ) && speed_x)
                	speed_x--;
	else if (speed_x < 0)
        	while (ENGINE.check_wall(x + 31 + speed_x, y + 2 * TILE_SIDE_HEIGHT, z ) && speed_x)
                	speed_x++;
// Huvud
        if (speed_x > 0)
	        while (ENGINE.check_wall(x + width - 31 + speed_x, y + TILE_SIDE_HEIGHT, z ) && speed_x)
                	speed_x--;
	else if (speed_x < 0)
        	while (ENGINE.check_wall(x + 31 + speed_x, y + TILE_SIDE_HEIGHT, z ) && speed_x)
                	speed_x++;

        // Obs bytte ut 3 -> 5 sedan f�rsvann hoppbuggen, konstigt...
/*	if (speed_z > 0)
        	while ((ENGINE.check_wall(x + 31, y + 66, z + speed_z + 5) ||
                	ENGINE.check_wall(x + width - 31, y + 66, z + speed_z + 5)) && speed_z)
                        speed_z--;
	else if (speed_z < 0)
        	while ((ENGINE.check_wall(x + 31, y + 66, z + speed_z - 5) ||
                	ENGINE.check_wall(x + width - 31, y + 66, z + speed_z - 5)) && speed_z)
                        speed_z++;*/

// Huvud
	if (speed_z > 0)
        	while ((ENGINE.check_wall(x + 31 + speed_x, y + TILE_SIDE_HEIGHT, z + speed_z + 5) ||
                	ENGINE.check_wall(x + width - 31 + speed_x, y + TILE_SIDE_HEIGHT, z + speed_z + 5)) && speed_z)
                        speed_z--;
	else if (speed_z < 0)
        	while ((ENGINE.check_wall(x + 31 + speed_x, y + TILE_SIDE_HEIGHT, z + speed_z - 5) ||
                	ENGINE.check_wall(x + width - 31 + speed_x, y + TILE_SIDE_HEIGHT, z + speed_z - 5)) && speed_z)
                        speed_z++;
// Mage
	if (speed_z > 0)
        	while ((ENGINE.check_wall(x + 31 + speed_x, y + 2 * TILE_SIDE_HEIGHT, z + speed_z + 5) ||
                	ENGINE.check_wall(x + width - 31 + speed_x, y + 2 * TILE_SIDE_HEIGHT, z + speed_z + 5)) && speed_z)
                        speed_z--;
	else if (speed_z < 0)
        	while ((ENGINE.check_wall(x + 31 + speed_x, y + 2 * TILE_SIDE_HEIGHT, z + speed_z - 5) ||
                	ENGINE.check_wall(x + width - 31 + speed_x, y + 2 * TILE_SIDE_HEIGHT, z + speed_z - 5)) && speed_z)
                        speed_z++;

// T�r
	if (speed_z > 0)
        	while ((ENGINE.check_wall(x + 31 + speed_x, y + 3 * TILE_SIDE_HEIGHT, z + speed_z + 5) ||
                	ENGINE.check_wall(x + width - 31 + speed_x, y + 3 * TILE_SIDE_HEIGHT, z + speed_z + 5)) && speed_z)
                        speed_z--;
	else if (speed_z < 0)
        	while ((ENGINE.check_wall(x + 31 + speed_x, y + 66, z + speed_z - 5) ||
                	ENGINE.check_wall(x + width - 31 + speed_x, y + 66, z + speed_z - 5)) && speed_z)
                        speed_z++;


/*        	while ((ENGINE.check_wall(x + 31, y + height, z + speed_z - 5) ||
			ENGINE.check_wall(x + width - 31, y + height, z + speed_z - 5) ||
			ENGINE.check_wall(x + 31, y + height, z + speed_z + 5) ||
			ENGINE.check_wall(x + width - 31, y + height, z + speed_z + 5)) && speed_z)
			if (speed_z > 0)
                        	speed_z--;
			else if (speed_z < 0)
                        	speed_z++;*/

/*        if (speed_z)
		while ((ENGINE.check_wall(x + 31, y + height, z + speed_z - 3) ||
			ENGINE.check_wall(x + width - 31, y + height, z + speed_z - 3)) &&  speed_z)
                        speed_z = 0;
			if (speed_z > 0)
                        	speed_z--;
			else if (speed_z < 0)
                        	speed_z++;*/


        x += speed_x;
        z += speed_z;
        y += speed_y;
        if (speed_y > 0 && elevator) {
		y += speed_y;
                y -= speed_y;
	}

/*        if (ENGINE.check_floor(x + 31, y + 66, z) ||
        	ENGINE.check_floor(x + width - 31, y + 66, z)) {
	        x -= speed_x;
	        z -= speed_z;
	        y -= speed_y;
        }*/
	while(ENGINE.check_floor(x + 31, y + 66, z)
		|| ENGINE.check_floor(x + width - 31, y + 66, z))
		y--;

        if (x < 0)
	        x = 0;
	if (z > MIN_Z)
        	z = MIN_Z;
	if (z < MAX_Z)
        	z = MAX_Z;
	layer = z / TILE_TOP_HEIGHT;

        if (state & STATE_IMMORTAL && !counter4)
        	state &= ~STATE_IMMORTAL;

        if (state & STATE_IMMORTAL) { //�r jag od�dlig
        	if (counter3) //vi ska ha genomskinligt
                	current_image = 240;//204;//168; //132;//65;
		if (!counter2) { //byt genoms. eller ej
                	counter3 = counter3 == 0 ? 1 : 0;
                         counter2 = 10;
		}
	}

        if (mode == MODE_WATER) {
        	if (!(random() % 75))
                	ENGINE.create_object(new Bubble_o(x, y, z));
        }

        elevator = NULL;
        elev_station = NULL;

	if (lives < 1) {
        	return -1;
	} else
	        return 0;
}/**************************************************************************/
player_o::~player_o() {
	delete ctrl;
}

void player_o::Collision(Object *o) {
	int tmp;
        if(o->GetWho() == FRIEND_ELEVATOR) {
        	elevator = o;
                return;
        } else if (o->GetWho() == FRIEND_ELEVSTAT) {
        	elev_station = o;
		return;
	}

	if (state & STATE_DEAD)
        	return;

	if (o->GetWho() == FRIEND_CHECKPOINT) {
        	save_x = x;
                save_y = y;
                save_z = z;
        }
	if (!(state & STATE_IMMORTAL)) {
		if (o->GetEnemies() & me) {
			energy -= o->GetStrength();
		}

                if (o->GetWho() & FRIEND_POWERUP)
                	return;

                if (o->GetWho() & ENEMY_HS_BULLET) {
                	if (o->GetDirection() == RIGHT_DIR) {
                        	ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z,
                                	2 + random() % 4));
			} else {
                        	ENGINE.create_effect(new blood_o(x + width / 2, y + random() % height, z,
                                	-(2 + random() % 4)));
			}
		}

		if (MAKE_PLAYER_IMMORTAL & o->GetWho()) {

                		state |= STATE_IMMORTAL;
	                        counter4 = TOUCH_DELAY;
 		}
	}
	if(energy < 0) {
        	if (o->GetWho() & ENEMY_EXPLOSION) {
                	tmp = DEATH_EXPLOSION;
		} else if (o->GetWho() & ENEMY_FIREBALL) {
                        tmp = DEATH_FIREBALL;
		} else if (o->GetWho() & ENEMY_ICEBALL) {
                        tmp = DEATH_ICEBALL;
		} else if (o->GetWho() & ENEMY_HS_BULLET) {
                        tmp = DEATH_EXPLOSION;
		} else
                        tmp = DEATH_EXPLOSION;
                PerformDeath(tmp);
/*	        counter = 60;
		lives--;
                energy = MAX_ENERGY;
                state |= STATE_DEAD;
                current_image = 168; //132;*/
//                rest(1000);
	}
}

int player_o::NewLevel() {
        save_x = x;
        save_y = y;
        save_z = z;
        card = 0;

        return 0;
}

void player_o::PerformDeath(int typ) {
	switch (typ) {
                case DEATH_ICEBALL:
			ENGINE.create_object(new FrosenBody_o(x, y + height, z, direction));
                        break;
                case DEATH_FIREBALL:
			ENGINE.create_effect(new BurningBody_o(x, y + height, z));
                        break;
                case DEATH_BULLET:
                /* fallthrough */
        	case DEATH_EXPLOSION:
                /* fallthrough */
                default:
			DEATH_BY_EXPLOSION;
                	break;
	}
        counter = 60;
        lives--;
        energy = MAX_ENERGY;
        state |= STATE_DEAD;
        current_image = 240;//168; //132;
}

void player_o::AddAmmo(int type, int num) {
	weapon[type].ammo += num;
        if (weapon[type].ammo > weapon[type].max_ammo)
        	weapon[type].ammo = weapon[type].max_ammo;
}
