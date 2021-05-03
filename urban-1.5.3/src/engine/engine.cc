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
    Thomas Nyberg                                       Jonas Borgstr�m
    Br�nnbergsv�gen 21b                                 Bondev�gen 6
    92136 Lycksele                                      92332 Storuman
    Sweden                                              Sweden

    or email:

    thomas.nyberg@usa.net                               jonas_b@bitsmart.com
*****************************************************************************/
#include <unistd.h>
#include <string.h>
#include <allegro.h>

#ifdef DJGPP
	#include <dos.h>
#else
	#include <sys/time.h>
	#include <signal.h>
#endif

#include <time.h>
#include "game.h"
#include "engine.h"
#include "object.h"
#include "object2.h"
#include "icache.h"
#include "otypes.h"
#include "urbfont.h"
#include "ctrls.h"
#include "sort.h"
#include "config.h"
#include "cheat.h"

#define LIGHTENING_MAX		58
#define LIGHTENING_STEP		5

#define MAP_WIDTH 		map.GetWidth()
#define MAP_HEIGHT 		map.GetHeight()
#define FRAMESPERSEC 		60
#define SCORE_BOARD_HEIGHT 	(61 - (2 * TILE_SIDE_HEIGHT))
#define TRANSPARENT_TILE        4096
#define FAKE_TILE		8192
#define IS_TRANSPARENT(n)       (map.GetTileInfo(n - 1) & TRANSPARENT_TILE)
#define IS_FAKE(n)		(map.GetTileInfo(n - 1) & FAKE_TILE)
#define SHAKE_DELAY		50
#define MSG_FRAME_DELAY		120

//#define NEED_TO_STEGA


#if defined(NEED_TO_STEGA) && defined(__DISTRIBUTION__)
#	warning NEED_TO_STEGA defined and compiling for distribution
#endif

#define SECONDS_TO_FRAMES(x) (x * FRAMESPERSEC)
#define FRAMES_TO_SECONDS(x) (x / FRAMESPERSEC)
//#define _ALLOW_Fx_LAYER_OFF

#define PLAYER_N_WEAPONS	7 //OBS!! �ndra i player.cc

#define OBJ_ON_LEFT (map_x - 500)
#define OBJ_ON_RIGHT (map_x + max_update_area_x)
#define OBJ_ON_TOP (map_y - 500)
#define OBJ_ON_BOTTOM (map_y + 500)

#define CLEAR_MESSAGES ( {\
while (num_messages > 0) \
	PopMessage(); \
} )

volatile long n = 0;
volatile long n2 = 0;
char lock_frame_count_to_60hz = 0;
Object *s_obj[MAX_OBJECTS];
extern class Config *config;
volatile int show_next_frame = 0;
char cheat_code_letters[64];
int cheat_cl_p = 0;
unsigned long cheat_codes_active = 0;

struct Letter Letters[] = {
{KEY_A, 'A'},
{KEY_B, 'B'},
{KEY_C, 'C'},
{KEY_D, 'D'},
{KEY_E, 'E'},
{KEY_F, 'F'},
{KEY_G, 'G'},
{KEY_H, 'H'},
{KEY_I, 'I'},
{KEY_J, 'J'},
{KEY_K, 'K'},
{KEY_L, 'L'},
{KEY_M, 'M'},
{KEY_N, 'N'},
{KEY_O, 'O'},
{KEY_P, 'P'},
{KEY_Q, 'Q'},
{KEY_R, 'R'},
{KEY_S, 'S'},
{KEY_T, 'T'},
{KEY_U, 'U'},
{KEY_V, 'V'},
{KEY_W, 'W'},
{KEY_X, 'X'},
{KEY_Y, 'Y'},
{KEY_Z, 'Z'}};

#define NUM_LETTERS (sizeof(Letters) / sizeof(Letters[0]))

struct CheatCode cheat_codes[] = {
	{"LINUSISGOD", CHEAT_MK_PLAYER_IMMORTAL},
	{"LINUXRULES", CHEAT_GIVE_PLAYER_HEALTH},
	{"VOLATILE", CHEAT_GIVE_PLAYER_FIRE},
	{"ILIKETOSKATE", CHEAT_GIVE_PLAYER_ICE},
	{"KABLAM", CHEAT_GIVE_PLAYER_GRENADE},
	{"STARTREKFOREVER", CHEAT_GIVE_PLAYER_PLASMA},
	{"LETSROCK", CHEAT_GIVE_PLAYER_MINIGUN},
	{"SITINTHECHAIR", CHEAT_GIVE_PLAYER_ELECTRIC},
	{"FIREAWAY", CHEAT_GIVE_PLAYER_1K_OF_AMMO},
	{"TALKINGTODEATH", CHEAT_GIVE_PLAYER_1K_AMMO_2_ALL},
	{"THENIDIE", CHEAT_GIVE_PLAYER_MAX_AMMO},
	{"NVTRULES", CHEAT_GIVE_PLAYER_ALL_WEAPONS},
	{"LETMEIN", CHEAT_GIVE_PLAYER_CARDS},
	{"THERECANBEONLYONE", CHEAT_CLEAR_LEVEL},
	{"TIMETODIE", CHEAT_SET_PLAYER_DEADLY},
	{"MICROSOFT", CHEAT_KILL_PLAYER},
	{"RETURNOFMRA", CHEAT_AIRSTRIKE},
};

#define NUM_CHEATCODES (sizeof(cheat_codes) / sizeof(cheat_codes[0]))


extern "C" {
	void testframe(...) {
		n = n2;
		n2 = 0;
	}
	END_OF_FUNCTION(testframe);
#ifdef DJGPP
	void update_screen() {
#else
	void update_screen(int sig) {
#endif
        	show_next_frame = 1;
	}
	END_OF_FUNCTION(update_screen);
}
/**************************************************************************/
Engine::Engine() {
	LOCK_FUNCTION(testframe);
	LOCK_FUNCTION(update_screen);
	LOCK_VARIABLE(n);
	LOCK_VARIABLE(n2);
	LOCK_VARIABLE(ENGINE.buffers);
        LOCK_VARIABLE(show_next_frame);
	player = NULL;
	num_objects = 0;
        num_alwaysupdate = 0;
        num_innerlayer = 0;
        num_dekorations = 0;
        num_effects = 0;
        num_pre_dekor = 0;
        num_pre_effect = 0;
	ringbuffer = ringbuffer2 = 0;
	num_messages = 0;
	always_shake = 0;
        max_time = -1;
	max_update_area_x = 500;
	max_update_area_y = 500;
	min_update_area_x = 500;
	min_update_area_y = 500;

	// Init framebuffer
        buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
}
/**************************************************************************/
void Engine::EnableEarthquake() {
	do_shake = SHAKE_DELAY;
        EnableLightening();
}
/**************************************************************************/
void Engine::EnableLightening() {
	if(lightening < LIGHTENING_MAX)
		lightening+= 10;

	if(lightening > LIGHTENING_MAX)
		lightening = LIGHTENING_MAX;
}
/**************************************************************************/
void Engine::UpdateLightening() {
	PALETTE out;
        static RGB *white = NULL;

        if(white == NULL) {

        	white = new RGB[256];
                for(int i=0;i<256;i++) {

                	white[i].r = white[i].g = white[i].b = 63;
                }
        }

	if(lightening) {
		// Update Palette
        	if(!(lightening % LIGHTENING_STEP) || lightening == LIGHTENING_MAX) {

				fade_interpolate(map.GetPal(), white, out, lightening, 0, 255);

                        set_palette(out);
        	}
	        lightening-= 4;

                if(lightening <= 0) {

                	lightening = 0;
                        set_palette(map.GetPal());
                }
        }
}
/**************************************************************************/
Engine::~Engine() {
	int i;

	for(i = 0;i < num_objects;i++) {
		delete objects[i];
	}
	for(i = 0;i < num_alwaysupdate;i++) {
		delete alwaysupdate[i];
	}
	for (i = 0;i < num_effects;i++) {
		delete effects[i];
	}
	for (i = 0;i < (ringbuffer ? MAX_DEKORATION_OBJECTS : num_dekorations);i++) {
		delete dekorations[i];
	}
	for (i = 0;i < num_pre_effect;i++) {
		delete map_effect[i];
	}
	for (i = 0;i < num_pre_dekor;i++) {
		delete map_dekor[i];
	}

}
/**************************************************************************/
void Engine::create_objects() {
	int i, j, k;
        int procent;
        PALETTE pal;
        BITMAP *ibild = icache.GetImage("ibild.pcx", pal);
        UrbanFont fnt(SMALL_FONT2);
        char buffer[80];
        char *episode = NULL;
        char *level = NULL;
        episode = map.GetOption("EPISODE_NAME");
        level = map.GetOption("LEVEL_NAME");

        set_palette (pal);

	blit(ibild, screen, 0, 0, 0, 0,	SCREEN_WIDTH, SCREEN_HEIGHT);

	for (k = 0;k < 3;k++)
		for (i = 0;i < MAP_WIDTH;i++) {
			procent = (100 * (k * MAP_WIDTH + i)) / (3 * MAP_WIDTH);

                        sprintf(buffer, "Loading %d%%", procent);
                        BITMAP *bmp = fnt.print(buffer);
                        blit(ibild, screen, 160 - bmp->w / 2, SCREEN_HEIGHT - bmp->h, 160 - bmp->w / 2, SCREEN_HEIGHT - bmp->h,
                        	bmp->w, bmp->h);
                        masked_blit(bmp, screen, 0, 0, 160 - bmp->w / 2, SCREEN_HEIGHT - bmp->h,
                        	bmp->w, bmp->h);
                        destroy_bitmap(bmp);
                        if (level != NULL) {
	                        bmp = fnt.print(level);
        	                masked_blit(bmp, screen, 0, 0, 160 - bmp->w / 2, SCREEN_HEIGHT / 2 - bmp->h,
                	        	bmp->w, bmp->h);
                        	destroy_bitmap(bmp);
			}
                        if (episode != NULL) {
	                        bmp = fnt.print(episode);
        	                masked_blit(bmp, screen, 0, 0, 160 - bmp->w / 2, SCREEN_HEIGHT / 2 - 2 * bmp->h,
                	        	bmp->w, bmp->h);
                        	destroy_bitmap(bmp);
			}

			for (j = 0;j < MAP_HEIGHT;j++) {
                        	switch (map.GetDekor(i, j, k)) {
                                	case TYPE_PLAYER:
						map_dekor[num_pre_dekor++] =
							new player_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));

                                        	break;
                                };
                                switch (map.GetEffect(i, j, k)) {
                                	case TYPE_PLAYER:
                                        	map_effect[num_pre_effect++] =
							new player_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
                                };

				switch(map.GetObj(i, j, k)) {
					case TYPE_PLAYER:
						if(player == NULL) {
							objects[num_objects++] =
								new player_o(i * (TILE_WIDTH - 1),
									j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), control_type);
							player = objects[num_objects -1];
						} else {
							((player_o *)player)->MoveTo(i * (TILE_WIDTH - 1), j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						}
						break;
					case TYPE_SCIENTIST:
						objects[num_objects++] =
							new scientist_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_SOLDIER:
						objects[num_objects++] =
							new soldier_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_SOLDIER2:
						objects[num_objects++] =
							new soldier2_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_VAKTM:
						objects[num_objects++] =
							new Vaktm_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_POWERUP_ENERGY:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_ENERGY);
						break;
					case TYPE_EXTRA_LIFE:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_EXTRA_LIFE);
						break;
					case TYPE_CHECKPOINT:
						objects[num_objects++] =
							new checkpoint_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_WEAPON_FLAME_THROWER:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_WEAPON_FLAME_THROWER);
						break;
					case TYPE_WEAPON_ICEMAKER:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_WEAPON_ICEMAKER);
						break;
					case TYPE_WEAPON_GRENADE_LAUNCHER:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_WEAPON_GRENADE_LAUNCHER);
						break;
					case TYPE_WEAPON_PLASMA:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_WEAPON_PLASMA);
						break;
					case TYPE_AMMO_PLASMA:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_AMMO_PLASMA);
						break;
					case TYPE_WEAPON_ELECTRIC:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_WEAPON_ELECTRIC);
						break;
					case TYPE_AMMO_ELECTRIC:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_AMMO_ELECTRIC);
						break;
					case TYPE_AMMO_FLAME_THROWER:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_AMMO_FLAME_THROWER);
						break;
					case TYPE_AMMO_ICEMAKER:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_AMMO_ICEMAKER);
						break;
					case TYPE_AMMO_GRENADE_LAUNCHER:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_AMMO_GRENADE_LAUNCHER);
						break;
					case TYPE_AMMO_GRENADE_LAUNCHER6:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_AMMO_GRENADE_LAUNCHER6);
						break;
					case TYPE_BOSS_SCIENTIST:
						objects[num_objects++] =
							new Boss_scientist_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_BOSS_SOLDIER:
						objects[num_objects++] =
							new Boss_soldier_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_MINE:
						objects[num_objects++] =
							new mine_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_FLOORGUN:
                                        	objects[num_objects++] =
                                        		new FloorGun_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_RAMBO:
                                        	objects[num_objects++] =
                                        		new Rambo_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_BOSS_RAMBO:
                                        	objects[num_objects++] =
                                        		new Boss_Rambo_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_ARNOLD:
                                        	objects[num_objects++] =
                                        		new Arnold_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_BOSS_ARNOLD:
                                        	objects[num_objects++] =
                                        		new Boss_Arnold_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_DOOR: //OBS!!!
					case TYPE_GREEN_DOOR: //OBS!!!
                                        	innerlayer[num_innerlayer++] =
                                        		new Door_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT, green);
						break;
					case TYPE_RED_DOOR: //OBS!!!
                                        	innerlayer[num_innerlayer++] =
                                        		new Door_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT, red);
						break;
					case TYPE_BLUE_DOOR: //OBS!!!
                                        	innerlayer[num_innerlayer++] =
                                        		new Door_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT, blue);
						break;
					case TYPE_BARREL:
                                        	objects[num_objects++] =
                                        		new Barrel_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT);
						break;
					case TYPE_DOLPH:
                                        	objects[num_objects++] =
                                        		new Dolph_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_BOSS_DOLPH:
                                        	objects[num_objects++] =
                                        		new Boss_Dolph_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_EINSTEIN:
						objects[num_objects++] =
							new Einstein_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_BOSS_EINSTEIN:
						objects[num_objects++] =
							new Boss_Einstein_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));

						break;
					case TYPE_CRYCHECK:
						objects[num_objects++] =
							new Crycheck_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_BOSS_CRYCHECK:
						objects[num_objects++] =
							new Boss_Crycheck_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));

						break;
					case TYPE_LEFT_COMPUTER_SCREEN:
						objects[num_objects++] =
							new ComputerScreen_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), 0);
						break;
					case TYPE_RIGHT_COMPUTER_SCREEN:
						objects[num_objects++] =
							new ComputerScreen_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), 1);
						break;
					case TYPE_DR_GREEN:
						objects[num_objects++] =
							new DrGreen_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_BOSS_DR_GREEN:
						objects[num_objects++] =
							new Boss_DrGreen_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_DEKOR_ROBOT1:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/robot1.pcx");
						break;
                                        case TYPE_DEKOR_ROBOT2:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/robot2.pcx");
						break;
                                        case TYPE_DEKOR_ROBOT3:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/robot3.pcx");
						break;
                                        case TYPE_DEKOR_STOL1:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/stol1.pcx");
						break;
                                        case TYPE_DEKOR_STOL2:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/stol2.pcx");
						break;
					case TYPE_DEKOR_RITNING1:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/ritning1.pcx");
						break;
                                        case TYPE_DEKOR_RITNING2:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/ritning2.pcx");
						break;
                                        case TYPE_DEKOR_WEAPON1:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/vapen.pcx");
						break;
                                        case TYPE_DEKOR_HELMET1:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/hjalm.pcx");
						break;
					case TYPE_DEKOR_ROCKETPART1:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/rockpart.pcx");
						break;
					case TYPE_DEKOR_ROCKETPART2:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/warhead.pcx");
						break;
					case TYPE_DEKOR_ROCKETPART3:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/rockp2.pcx");
						break;
					case TYPE_SEAGAL:
                                        	objects[num_objects++] =
                                        		new Seagal_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_BOSS_SEAGAL:
                                        	objects[num_objects++] =
                                        		new Boss_Seagal_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
                                        case TYPE_DEKOR_FLAG:
                                        	objects[num_objects++] =
							new Dekor_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT,
									"dekor/flagga.pcx");
						break;
					case TYPE_DOOR_EXTRA_LEVEL:
                                        	innerlayer[num_innerlayer++] =
                                        		new Door_o(i * (TILE_WIDTH - 1),
                                                        	j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT, extra_level);
						break;
					case TYPE_ELEVATOR:
						innerlayer[num_innerlayer++] =
							new Elevator_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_ELEVUP:
						innerlayer[num_innerlayer++] =
							new ElevatorStation_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1),
								UP_DIR, none);
						break;
					case TYPE_ELEVDOWN:
						innerlayer[num_innerlayer++] =
							new ElevatorStation_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1),
								DOWN_DIR, none);
						break;
					case TYPE_ELEVUP_GREEN:
						innerlayer[num_innerlayer++] =
							new ElevatorStation_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1),
								UP_DIR, green);
						break;
					case TYPE_ELEVDOWN_GREEN:
						innerlayer[num_innerlayer++] =
							new ElevatorStation_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1),
								DOWN_DIR, green);
						break;
					case TYPE_ELEVWIRE:
						innerlayer[num_innerlayer++] =
							new ElevatorWire_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_SOLDIER4:
						objects[num_objects++] =
							new soldier4_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_SOLDIER3:
						objects[num_objects++] =
							new soldier3_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_WATER:
						objects[num_objects++] =
							new Water_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT - 2 + k, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_BUBBLEMAKER:
						objects[num_objects++] =
							new BubbleMaker_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_WEAPON_MINIGUN:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_WEAPON_MINIGUN);
						break;
					case TYPE_AMMO_MINIGUN:
						objects[num_objects++] =
							new powerup_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), POWERUP_AMMO_MINIGUN);
						break;
					case TYPE_AIRSTRIKER:
                                        	alwaysupdate[num_alwaysupdate++] =
							new Airstrike_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1));
						break;
					case TYPE_RED_CARD:
						objects[num_objects++] =
							new card_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), red);
						break;
                                        case TYPE_BLUE_CARD:
						objects[num_objects++] =
							new card_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), blue);
						break;
                                        case TYPE_GREEN_CARD:
						objects[num_objects++] =
							new card_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1), green);
						break;
					case TYPE_TANK:
                                        	objects[num_objects++] =
							new Tank_o(i * (TILE_WIDTH - 1),
								j * TILE_SIDE_HEIGHT, k * TILE_TOP_HEIGHT);
						break;
				};

			}
		}
	/* Move the elevators to the end of the list */
        j = num_innerlayer - 1;
	for(i = 0; i< j;i++) {
        	if(innerlayer[i]->GetWho() == FRIEND_ELEVATOR) {
			while(j > i) {
		        	if(innerlayer[j]->GetWho() != FRIEND_ELEVATOR) {

                                	Object *tmp = innerlayer[i];
                                        innerlayer[i] = innerlayer[j];
                                        innerlayer[j] = tmp;
                                        j--;
                                        break;
                                }
                                j--;
			}
                }
	}

	i = j = sound.GetMusicVolume();
        for(;i > 0;i--) {
        	sound.SetMusicVolume(i);
                rest(10);
	}
/*
Behandla level-options
always_shake
*/
	char *opt;
        opt = map.GetOption("ALWAYS_SHAKE");

        if (opt != NULL && !strcmp(opt, "ON"))
        	always_shake = 1;
	else
        	always_shake = 0;

	opt = map.GetOption("TIME");
        if (opt != NULL) {
	        max_time = SECONDS_TO_FRAMES(atoi(opt));
	        if (!max_time)
        		max_time = -1;
	} else
		max_time = -1;

	if (always_shake)
        	do_shake = 1;

	char buf2[200];// = new char[200];
        char *s = map.GetOption("MUSIC");

        if (s) {
        	sprintf(buf2, "%s/%s", DATPATH, s);
	} else {

        	switch (random() % 8) {
        		case 0:
		        	sprintf(buf2, "%s/snd/modules/blazer.xm", DATPATH);
	                        break;
        	        case 1:
			        sprintf(buf2, "%s/snd/modules/looser.xm", DATPATH);
                        	break;
	                case 2:
			        sprintf(buf2, "%s/snd/modules/madness2.xm", DATPATH);
                	        break;
	                case 3:
			        sprintf(buf2, "%s/snd/modules/fett.xm", DATPATH);
                	        break;
			case 4:
			        sprintf(buf2, "%s/snd/modules/basesolo.xm", DATPATH);
                	        break;
	                case 5:
			        sprintf(buf2, "%s/snd/modules/nole.xm", DATPATH);
                	        break;
	                case 6:
			        sprintf(buf2, "%s/snd/modules/demise.xm", DATPATH);
                	        break;
			case 7:
        	        default:
				sprintf(buf2, "%s/snd/modules/slayers.xm", DATPATH);
                		break;
		}
	}
        if (IS_SOUND_ON)
		sound.PlayMusic(buf2);
        for(;i < j;i++) {
                sound.SetMusicVolume(i);
                rest(10);
	}

        blit(ibild, screen, 0, 0, 0, 0, ibild->w, ibild->h);
        int w123 = 0;

        if (level != NULL) {
		BITMAP *bmp = fnt.print(level);
		masked_blit(bmp, screen, 0, 0, 160 - bmp->w / 2, SCREEN_HEIGHT / 2 - bmp->h,
			bmp->w, bmp->h);
		destroy_bitmap(bmp);
                w123 = 1;
	}
	if (episode != NULL) {
		BITMAP *bmp = fnt.print(episode);
		masked_blit(bmp, screen, 0, 0, 160 - bmp->w / 2, SCREEN_HEIGHT / 2 - 2 * bmp->h,
			bmp->w, bmp->h);
		destroy_bitmap(bmp);
                w123 = 1;
	}
        if (w123) {
        	for (int i = 0;i < 100;i++) {
	        	rest(50);
                        if (keypressed())
                        	break;
		}
	}


	icache.FreeImage(ibild);
}
/**************************************************************************/
void Engine::NewGame() {
	int i;

	for(i = 0; i < num_objects; i++) {

        	delete objects[i];
	}
	for(i = 0; i < num_alwaysupdate; i++) {

        	delete objects[i];
	}
        player = NULL;
	for (i = 0;i < num_effects;i++) {

		delete effects[i];
	}
	for (i = 0;i < num_innerlayer;i++) {

		delete innerlayer[i];
	}
	num_effects = 0;
	for (i = 0;i < (ringbuffer ? MAX_DEKORATION_OBJECTS : num_dekorations);i++) {

		delete dekorations[i];
	}

	num_dekorations = 0;
        num_objects = 0;
        num_alwaysupdate = 0;
        num_effects = 0;
        num_innerlayer = 0;
        do_shake = 0;
	ringbuffer = ringbuffer2 = 0;
	map_x = 0;
	map_y = 0;
	level_complete = 0;
        lightening = 0;

        score.ResetScore();
}

/**************************************************************************/
int Engine::play_level(char *map_name, struct PlayerData *p_dat, int controls) {
	int i;
	RGB *pal;

        srandom(time(NULL));

        control_type = controls;

	if(!map.LoadMap(map_name)) {
		printf("Can't open map '%s'\n", map_name);
		exit(1);
	}
	// Black background
	pal = map.GetPal();
	pal[0].r = 0;
	pal[0].g = 0;
	pal[0].b = 0;

	for(i = 0; i < num_objects; i++) {
		if (objects[i] != player)
			delete objects[i];

	}
	for(i = 0; i < num_alwaysupdate; i++) {
		delete alwaysupdate[i];

	}
	if(player != NULL) {
		num_objects = 1;
		objects[0] = player;
	}

	for (i = 0;i < num_effects;i++) {
		delete effects[i];
	}
	num_effects = 0;

	for (i = 0;i < num_innerlayer;i++) {
		delete innerlayer[i];
	}
        num_innerlayer = 0;

	for (i = 0;i < num_pre_dekor;i++) {
		delete map_dekor[i];
	}
	num_pre_dekor = 0;

	for (i = 0;i < num_pre_effect;i++) {
		delete map_effect[i];
	}
	num_pre_effect = 0;


	for (i = 0;i < (ringbuffer ? MAX_DEKORATION_OBJECTS : num_dekorations);i++) {
		delete dekorations[i];
	}
	num_dekorations = 0;
        do_shake = 0;
	ringbuffer = ringbuffer2 = 0;
	map_x = 0;
	map_y = 0;
	level_complete = 0;
        lightening = 0;

	create_objects();
#ifdef DJGPP
	install_int(testframe, 1000);

	install_int(update_screen, 1000/FRAMESPERSEC);
#else
	signal(SIGALRM, update_screen);

	struct itimerval tval, oldval;

	tval.it_interval.tv_sec = 0;
	tval.it_interval.tv_usec = 1000000/FRAMESPERSEC;
	tval.it_value.tv_sec = 0;
	tval.it_value.tv_usec = 1000000/FRAMESPERSEC;

	setitimer(ITIMER_REAL, &tval, &oldval);
#endif
//	CLEAR_MESSAGES;
	for (i = 0;i < num_messages;i++)
        	PopMessage();

#define _PLAYER ((player_o *)player)
	if (p_dat) {
                score.SetScore(p_dat->score);
		_PLAYER->SetEnergy(p_dat->health);
        	_PLAYER->SetLife(p_dat->num_lives);
		for (i = 0;i < PLAYER_N_WEAPONS;i++) {
		        _PLAYER->SetAmmo(i, p_dat->weapon_ammo[i]);
                	if (p_dat->weapon_avail[i])
				_PLAYER->EnableWeapon(i);
			else
        	        	_PLAYER->DisableWeapon(i);
		}
	}

        clear_keybuf();

	memset(cheat_code_letters, 0, 64);
	cheat_cl_p = 0;

	int return_code = play_loop();

        clear_keybuf();

        if (p_dat) {
	        p_dat->health = _PLAYER->GetEnergy();
        	p_dat->num_lives = _PLAYER->GetLives();
		for (i = 0;i < PLAYER_N_WEAPONS;i++) {
        		p_dat->weapon_ammo[i] = _PLAYER->GetAmmo(i);
                	p_dat->weapon_avail[i] = _PLAYER->HaveWeapon(i);
		}
                p_dat->score = score.GetScore();
	}
#undef _PLAYER

	// Remove screen update callback
#ifdef DJGPP
	remove_int(testframe);
	remove_int(update_screen);
#else
	setitimer(ITIMER_REAL, &oldval, NULL);
	clear_keybuf();
#endif
	return return_code;
}
/**************************************************************************/
int Engine::display_map() {
	int i, j, k;
	int startx = (map_x / (TILE_WIDTH - 1));
	int starty = (map_y / (TILE_SIDE_HEIGHT));
	int offsetx = (startx * TILE_WIDTH) - map_x;
	int offsety = (starty * TILE_SIDE_HEIGHT) - map_y;
        int bgstartx = map_x / BG_TILE_WIDTH;
        int bgstarty = (map_y / BG_TILE_HEIGHT)  - 1;
        int bgoffsetx = (bgstartx * BG_TILE_WIDTH) - map_x;
        int bgoffsety = (bgstarty * BG_TILE_HEIGHT) - map_y;
	static int background_availible = 0;
	int kalle3;
	int temp;
	int ki;
        int numosc = 0;
        static UrbanFont *Ufont = NULL;
        char buff[512];
	int draw_player;
	
        if (Ufont == NULL)
                Ufont = new UrbanFont(SMALL_FONT);

	map.UpdatePal();
	UpdateLightening();

#ifndef DJGPP
	keyboard_update(); // Update Keyboard for *NIX
#endif
       	if (!background_availible)
		clear_to_color (buffer, 1);
	check_collision();

        if (do_shake && always_shake == 0)
        	do_shake--;
	n2++;
	for (i = bgstartx;i < (bgstartx + 4);i++)
		for (j = (bgstarty + 3);j >= bgstarty;j--) {
                	if (j < 0) //<- OBS!!! HACK
                        	continue;
			if ((kalle3 = map.GetBackGround(i, j))) {
				blit(map.GetBGTile(kalle3 - 1), buffer,
					0, 0, (i - bgstartx) * BG_TILE_WIDTH + bgoffsetx,
					(j - bgstarty) * BG_TILE_HEIGHT + bgoffsety + SCORE_BOARD_HEIGHT,
					BG_TILE_WIDTH, BG_TILE_HEIGHT);
				background_availible = 1;
			}
		}
	for (k = 0;k < 3;k++) {
#ifdef _ALLOW_Fx_LAYER_OFF
		if (key[KEY_F1] && k == 0)
			continue;
		if (key[KEY_F2] && k == 1)
			continue;
		if (key[KEY_F3] && k == 2)
			continue;
#endif
		ki = (TILE_TOP_HEIGHT * k) - k;
		draw_player = 0;
		for (i = startx;i < (startx + 14);i++)
			for (j = (starty + 10);j >= starty;j--)
				if ((kalle3 = map.GetBG(i, j, k))) {
					if (IS_TRANSPARENT(kalle3))
						masked_blit(map.GetTile(kalle3 - 1), buffer,
                                                	0, 0, (i - startx) * TILE_WIDTH + offsetx - i,
                                                        (j - starty) * TILE_SIDE_HEIGHT + offsety + ki + SCORE_BOARD_HEIGHT,
                                                        TILE_WIDTH, TILE_HEIGHT);
					else
						blit(map.GetTile(kalle3 - 1), buffer,
							0, 0, (i - startx) * TILE_WIDTH + offsetx - i,
							(j - starty) * TILE_SIDE_HEIGHT + offsety + ki + SCORE_BOARD_HEIGHT,
							TILE_WIDTH, TILE_HEIGHT);
				}

		for (i = 0;i < num_innerlayer;i++)
			if (innerlayer[i] && innerlayer[i]->GetLayer() == k) {
				if ((temp = innerlayer[i]->update()) < 0) {
					if (temp == DELETE_ME)
						delete innerlayer[i];
					memmove(&innerlayer[i], &innerlayer[i + 1], sizeof(Object *) * (num_innerlayer - i - 1));
					i--;
					num_innerlayer--;
				} else {
					masked_blit(innerlayer[i]->GetImage(), buffer, 0, 0,
						innerlayer[i]->GetX() - map_x,   //(k - 1)
						innerlayer[i]->GetY() - map_y + innerlayer[i]->GetZ() - k + SCORE_BOARD_HEIGHT,
						innerlayer[i]->GetImage()->w, innerlayer[i]->GetImage()->h);
				}
			}



		for (i = 0;i < num_pre_dekor;i++)
			if (map_dekor[i] && map_dekor[i]->GetLayer() == k) {
				masked_blit(map_dekor[i]->GetImage(), buffer, 0, 0,
					map_dekor[i]->GetX() - map_x,   //(k - 1)
					map_dekor[i]->GetY() - map_y + map_dekor[i]->GetZ() - k + SCORE_BOARD_HEIGHT,
					map_dekor[i]->GetImage()->w, map_dekor[i]->GetImage()->h);
			}


		for (i = 0;i < num_pre_effect;i++)
			if (map_effect[i] && map_effect[i]->GetLayer() == k) {
				if ((temp = map_effect[i]->update()) < 0) {
					if (temp == DELETE_ME)
						delete map_effect[i];
					memmove(&map_effect[i], &map_effect[i + 1], sizeof(Object *) * (num_pre_effect - i - 1));
					i--;
					num_pre_effect--;
				} else {
					masked_blit(map_effect[i]->GetImage(), buffer, 0, 0,
						map_effect[i]->GetX() - map_x,   //(k - 1)
						map_effect[i]->GetY() - map_y + map_effect[i]->GetZ() - k + SCORE_BOARD_HEIGHT,
						map_effect[i]->GetImage()->w, map_effect[i]->GetImage()->h);
				}
			}



		for (i = 0;i < (ringbuffer ? MAX_DEKORATION_OBJECTS : num_dekorations);i++)
			if (dekorations[i] && dekorations[i]->GetLayer() == k) {
				masked_blit(dekorations[i]->GetImage(), buffer, 0, 0,
					dekorations[i]->GetX() - map_x,   //(k - 1)
					dekorations[i]->GetY() - map_y + dekorations[i]->GetZ() - k + SCORE_BOARD_HEIGHT,
					dekorations[i]->GetImage()->w, dekorations[i]->GetImage()->h);
			}
//Ny GETZ
		for (i = 0;i < num_effects;i++)
			if (effects[i] && effects[i]->GetLayer() == k) {
				if ((temp = effects[i]->update()) < 0) {
					if (temp == DELETE_ME)
						delete effects[i];
					memmove(&effects[i], &effects[i + 1], sizeof(Object *) * (num_effects - i - 1));
					i--;
					num_effects--;
				} else {
					masked_blit(effects[i]->GetImage(), buffer, 0, 0,
						effects[i]->GetX() - map_x,   //(k - 1)
						effects[i]->GetY() - map_y + effects[i]->GetZ() - k + SCORE_BOARD_HEIGHT,
						effects[i]->GetImage()->w, effects[i]->GetImage()->h);
				}
			}
		for(i = 0;i < num_alwaysupdate;i++)
			if (alwaysupdate[i] && alwaysupdate[i]->GetLayer() == k) {
				numosc++;
				if ((temp = alwaysupdate[i]->update()) < 0) {
					score.AddScore(alwaysupdate[i]->GetScore());
                                        if (alwaysupdate[i]->GetScore()) {
	                                        create_effect(new Points_o(alwaysupdate[i]->GetX(), alwaysupdate[i]->GetY(), alwaysupdate[i]->GetZ(),
        	                                	alwaysupdate[i]->GetScore()));
					}
					// Delete object from list
					if (temp == DELETE_ME)
						delete alwaysupdate[i];
					memmove(&alwaysupdate[i], &alwaysupdate[i+1], sizeof(Object *) * (num_alwaysupdate - i - 1));
					i--;
					num_alwaysupdate--;
				} else {
					masked_blit(alwaysupdate[i]->GetImage(), buffer, 0, 0,
						(kalle3 = alwaysupdate[i]->GetX()) - map_x,   //(k - 1)
                       	                        alwaysupdate[i]->GetY() - map_y + alwaysupdate[i]->GetZ() - k + SCORE_BOARD_HEIGHT,
						alwaysupdate[i]->GetImage()->w, alwaysupdate[i]->GetImage()->h);
				}
			}
		for(i = 0;i < num_objects;i++)
			if (objects[i] && objects[i]->GetLayer() == k &&
				objects[i]->GetX() > OBJ_ON_LEFT &&
				objects[i]->GetX() < OBJ_ON_RIGHT &&
				objects[i]->GetY() > OBJ_ON_TOP &&
				objects[i]->GetY() < OBJ_ON_BOTTOM) {
				numosc++;
				if ((temp = objects[i]->update()) < 0) {
					score.AddScore(objects[i]->GetScore());
                                        if (objects[i]->GetScore()) {
	                                        create_effect(new Points_o(objects[i]->GetX(), objects[i]->GetY(), objects[i]->GetZ(),
        	                                	objects[i]->GetScore()));
					}
					// Delete object from list
					if(objects[i] == player) {

							return -1;
					} else {
						if (temp == DELETE_ME)
							delete objects[i];
						memmove(&objects[i], &objects[i+1], sizeof(Object *) * (num_objects - i - 1));
						i--;
						num_objects--;
					}
				} else {
                                	if (objects[i] == player)
                                        	draw_player = 1;
					else
						masked_blit(objects[i]->GetImage(), buffer, 0, 0,
							(kalle3 = objects[i]->GetX()) - map_x,   //(k - 1)
                        	                        objects[i]->GetY() - map_y + objects[i]->GetZ() - k + SCORE_BOARD_HEIGHT,
							objects[i]->GetImage()->w, objects[i]->GetImage()->h);
				}
			}
		if (draw_player) {
			masked_blit(player->GetImage(), buffer, 0, 0,
				(kalle3 = player->GetX()) - map_x,   //(k - 1)
				player->GetY() - map_y + player->GetZ() - k + SCORE_BOARD_HEIGHT,
				player->GetImage()->w, player->GetImage()->h);
			draw_player = 0;
		}

	}
	score.Display(buffer);

        /* Pause */
        if(key[KEY_P]) {
        	BITMAP *buf = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);

        	while(keypressed())
                	readkey();

                blit(screen, buf, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	        // Draw a grid

	        for(int x = 0;x < 320; x+=2) {
		        for(int y = 0;y < 240; y+=2) {

				putpixel(buf, x, y, 0);
				putpixel(buf, x + 1, y + 1, 0);
			}
		}
                Ufont->print_centre("PAUSE", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, buf);

                blit(buf, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

                destroy_bitmap(buf);

        	while(!keypressed())
        		rest(10);

		clear_keybuf();
        }

        if(key[KEY_F11])
		textprintf(buffer, font, 0, 0, 36, "Frames/s %ld", n);

        if (num_messages > 0) {
        	masked_blit(messages[0].msg, buffer, 0, 0,
        		(SCREEN_WIDTH - messages[0].msg->w) / 2, 60,
        		messages[0].msg->w, messages[0].msg->h);
		messages[0].delay++;
                if (messages[0].delay > MSG_FRAME_DELAY) {
                	PopMessage();
		}
	}

        if (max_time != -1) {
        	max_time--;
                sprintf(buff, "Time left: %d", FRAMES_TO_SECONDS(max_time)); //OBS!! HACK
                Ufont->print(buff, 10, 60, buffer);
	}
        if (max_time == 0) {
        	((player_o*)player)->KillPlayer();
		char *opt = map.GetOption("TIME");
        	max_time = SECONDS_TO_FRAMES(atoi(opt));
        }

#ifdef DJGPP
	if (config->keyconf.gfx_quality == QUALITY_NORMAL)
		while (show_next_frame == 0);
	else if (config->keyconf.gfx_quality == QUALITY_HIGH)
        	vsync();
#else
		while (show_next_frame == 0) usleep(10);
#endif

	show_next_frame = 0;

	if (do_shake) {
		blit(buffer, screen, 0, 0, random() % 3 - 1, random() % 3 - 1, SCREEN_WIDTH, SCREEN_HEIGHT);
	} else
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

//handle cheatcodes
	cheat_codes_active = 0; /* reset cheats */

	for (i = 0;i < NUM_LETTERS;i++) {
		if (key[Letters[i].key] && cheat_cl_p < 63) {
			if (cheat_cl_p > 0 && cheat_code_letters[cheat_cl_p - 1] == Letters[i].letter)
				break;
				
			cheat_code_letters[cheat_cl_p++] = Letters[i].letter;
			cheat_code_letters[cheat_cl_p] = '\0'; //null-terminated
			//compare with known cheatcodes
			int clear_cc = 1;
			for (j = 0;j < NUM_CHEATCODES;j++) {
				if (!strncmp(cheat_codes[j].code, cheat_code_letters, cheat_cl_p)) {
					if (!strcmp(cheat_codes[j].code, cheat_code_letters)) { // cheat code match
						cheat_codes_active |= cheat_codes[j].cheat;
						PushMessage("Cheat activated");
						break;						
					} else {
						clear_cc = 0;						
					}						
				}
			}
			if (clear_cc) {
				memset(cheat_code_letters, 0, 64);
				cheat_cl_p = 0;
			}
			break;
		}
	}
	
	if (cheat_codes_active & CHEAT_CLEAR_LEVEL)
		ClearLevel();
		
	if (cheat_codes_active & CHEAT_AIRSTRIKE) {
		if (player->GetY() - 200 > 50) {
			create_alwaysupdate(new Airstrike_o(player->GetX(), player->GetY() - 200,
				player->GetZ()));
		} else
			PushMessage("Unable to issue airstrike");					
	}				
	return 1;
}
/**************************************************************************/
/* skapar ett objekt, ex projektil                                        */
/**************************************************************************/
void Engine::create_object(Object *obj) {
	objects[num_objects++] = obj;
}

void Engine::create_effect(Object *obj) {
	if (num_effects < MAX_EFFECTS_OBJECTS)
		effects[num_effects++] = obj;
}

void Engine::create_alwaysupdate(Object *obj) {
	if (num_alwaysupdate < MAX_OBJECTS)
		alwaysupdate[num_alwaysupdate++] = obj;
}

void Engine::create_dekoration(Object *obj) {
	if(num_dekorations >= MAX_DEKORATION_OBJECTS) {
		ringbuffer = 1;
		num_dekorations = 0;
	}
	if(ringbuffer)
		delete dekorations[num_dekorations];

	dekorations[num_dekorations++] = obj;
}

void Engine::create_shell(Object *obj) {
	if(num_shells >= MAX_SHELL_OBJECTS) {
		ringbuffer2 = 1;
		num_shells = 0;
	}
	if(ringbuffer2)
		delete shells[num_shells];

	shells[num_shells++] = obj;
}

/**************************************************************************/
int Engine::check_floor(int x, int y, int z) {
	int bx, by, bz;
	bx = x / (TILE_WIDTH - 1);
	by = y / TILE_SIDE_HEIGHT;
	bz = z / (TILE_TOP_HEIGHT  - 1);
        if (bz > 2)
        	bz = 2;

/*        if (map.GetBG(bx, by, bz))
		map.SetBG(bx, by, bz, 10);*/
	if(IS_FAKE(map.GetBG(bx, by, bz)))
        	return 0;
	else
		return map.GetBG(bx, by, bz) == 0 ? 0 : 1;
}

void Engine::ClearTile(int x, int y, int z) {
	int bx, by, bz;
	bx = x / (TILE_WIDTH - 1);
	by = y / TILE_SIDE_HEIGHT;
	bz = z / TILE_TOP_HEIGHT;
	map.SetBG(bx, by, bz, 0);
}

void Engine::SetTile(int x, int y, int z, int t) {
	int bx, by, bz;
	bx = x / (TILE_WIDTH - 1);
	by = y / TILE_SIDE_HEIGHT;
	bz = z / TILE_TOP_HEIGHT;
	map.SetBG(bx, by, bz, t);
}

int Engine::check_collision(Object *o) {
	int x_dist;
	int y_dist;
	int width, height;
	int z;
//        register int i;
	register int j;

	z = o->GetLayer();

	for(j = 0;j < num_objects;j++) {
			// Don't check the same object
		if (o == objects[j])
			continue;
		if (z != objects[j]->GetLayer())
			continue;

		x_dist = abs(o->GetX() + (o->GetWidth() >> 1) -
			(objects[j]->GetX() + (objects[j]->GetWidth() >> 1)));
		width = (o->GetWidth() + objects[j]->GetWidth()) >> 1;
		if (x_dist < width) {
			y_dist = abs(o->GetY() + (o->GetHeight() >> 1) -
				(objects[j]->GetY() + (objects[j]->GetHeight() >> 1)));
			height = (o->GetHeight() + objects[j]->GetHeight()) >> 1;
			if (y_dist < height) {
				return 1;
			}
		}
	}
	return 0;
}

/**************************************************************************/
void Engine::check_collision() {
	int x_dist;
	int y_dist;
	int width, height;
	int z;
	register int i;
	register int j;

        // Innerlayers
        if(player->GetLayer() == 0)
		for(i = 0;i < num_innerlayer;i++) {
			x_dist = abs(player->GetCollisionX() + (player->GetCollisionWidth() >> 1) -
				(innerlayer[i]->GetCollisionX() + (innerlayer[i]->GetCollisionWidth() >> 1)));
			width = (player->GetCollisionWidth() + innerlayer[i]->GetCollisionWidth()) >> 1;

			if (x_dist < width) {

				y_dist = abs(player->GetCollisionY() + (player->GetCollisionHeight() >> 1) -
					(innerlayer[i]->GetCollisionY() + (innerlayer[i]->GetCollisionHeight() >> 1)));
				height = (player->GetCollisionHeight() + innerlayer[i]->GetCollisionHeight()) >> 1;

				if (y_dist < height) {

					player->Collision(innerlayer[i]);
					innerlayer[i]->Collision(player);
				}
			}
	        }
	for(i = 0;i < num_innerlayer;i++)
		for(j = i + 1;j < num_innerlayer;j++) {
			x_dist = abs(innerlayer[j]->GetCollisionX() + (innerlayer[j]->GetCollisionWidth() >> 1) -
				(innerlayer[i]->GetCollisionX() + (innerlayer[i]->GetCollisionWidth() >> 1)));
			width = (innerlayer[j]->GetCollisionWidth() + innerlayer[i]->GetCollisionWidth()) >> 1;

			if (x_dist < width) {

				y_dist = abs(innerlayer[j]->GetCollisionY() + (innerlayer[j]->GetCollisionHeight() >> 1) -
					(innerlayer[i]->GetCollisionY() + (innerlayer[i]->GetCollisionHeight() >> 1)));
				height = (innerlayer[j]->GetCollisionHeight() + innerlayer[i]->GetCollisionHeight()) >> 1;

				if (y_dist < height) {

					innerlayer[j]->Collision(innerlayer[i]);
					innerlayer[i]->Collision(innerlayer[j]);
				}
			}
	        }

	// Objects
	for(i = 0;i < num_objects;i++) {
                if (objects[i]->GetX() < OBJ_ON_LEFT || objects[i]->GetX() > OBJ_ON_RIGHT ||
                	objects[i]->GetY() < OBJ_ON_TOP || objects[i]->GetY() > OBJ_ON_BOTTOM)
                	continue;
		z = objects[i]->GetLayer();

		for(j = i + 1;j < num_objects;j++) {
			// Don't check the same object
			if (z != objects[j]->GetLayer())
				continue;
	                if (objects[j]->GetX() < OBJ_ON_LEFT || objects[j]->GetX() > OBJ_ON_RIGHT ||
        	        	objects[j]->GetY() < OBJ_ON_TOP || objects[j]->GetY() > OBJ_ON_BOTTOM)
                		continue;


			x_dist = abs(objects[i]->GetCollisionX() + (objects[i]->GetCollisionWidth() >> 1) -
				(objects[j]->GetCollisionX() + (objects[j]->GetCollisionWidth() >> 1)));
			width = (objects[i]->GetCollisionWidth() + objects[j]->GetCollisionWidth()) >> 1;

			if (x_dist < width) {

				y_dist = abs(objects[i]->GetCollisionY() + (objects[i]->GetCollisionHeight() >> 1) -
					(objects[j]->GetCollisionY() + (objects[j]->GetCollisionHeight() >> 1)));
				height = (objects[i]->GetCollisionHeight() + objects[j]->GetCollisionHeight()) >> 1;

				if (y_dist < height) {

					objects[i]->Collision(objects[j]);
					objects[j]->Collision(objects[i]);
				}
			}
		}
	}
	// Alwaysupdate
	for(i = 0;i < num_alwaysupdate;i++) {
		z = alwaysupdate[i]->GetLayer();

		for(j = 0;j < num_objects;j++) {
			// Don't check the same object
			if (z != objects[j]->GetLayer())
				continue;

			x_dist = abs(alwaysupdate[i]->GetCollisionX() + (alwaysupdate[i]->GetCollisionWidth() >> 1) -
				(objects[j]->GetCollisionX() + (objects[j]->GetCollisionWidth() >> 1)));
			width = (alwaysupdate[i]->GetCollisionWidth() + objects[j]->GetCollisionWidth()) >> 1;

			if (x_dist < width) {

				y_dist = abs(alwaysupdate[i]->GetCollisionY() + (alwaysupdate[i]->GetCollisionHeight() >> 1) -
					(objects[j]->GetCollisionY() + (objects[j]->GetCollisionHeight() >> 1)));
				height = (alwaysupdate[i]->GetCollisionHeight() + objects[j]->GetCollisionHeight()) >> 1;

				if (y_dist < height) {

					alwaysupdate[i]->Collision(objects[j]);
					objects[j]->Collision(alwaysupdate[i]);
				}
			}
		}
	}
}
/**************************************************************************/
int Engine::check_wall(int x, int y, int z) {
	int bx, by, bz;

	y -= TILE_TOP_HEIGHT;
	bx = x / (TILE_WIDTH - 1);
	by = y / TILE_SIDE_HEIGHT;
	bz = z / (TILE_TOP_HEIGHT -1);

/*	if (map.GetBG(bx, by, bz))
		map.SetBG(bx, by, bz, 9);*/
	if(IS_FAKE(map.GetBG(bx, by, bz)))
        	return 0;
	else
		return map.GetBG(bx, by, bz) == 0 ? 0 : 1;
}
/**************************************************************************/
#define SCROLL \
		map_x = player->GetX() - 100; \
                playery = player->GetY();     \
		\
                if((playery - 65) < map_y) {  \
\
                	map_y = playery - 65; \
\
                } else if((playery - 110 + SCORE_BOARD_HEIGHT) > map_y) { \
\
                	map_y = (playery - 110 + SCORE_BOARD_HEIGHT);\
                }\
\
                if(map_y > (MAP_HEIGHT * TILE_SIDE_HEIGHT - SCREEN_HEIGHT + 25)) \
                	map_y = (MAP_HEIGHT * TILE_SIDE_HEIGHT - SCREEN_HEIGHT + 25); \
\
                if(map_y < 0) \
                	map_y = 0; \
\
		if (map_x > (MAP_WIDTH * (TILE_WIDTH - 1) - 330)) \
			map_x = (MAP_WIDTH * (TILE_WIDTH -1)  - 330); \
\
		if(map_x < 0) \
			map_x = 0;

int Engine::play_loop() {
	int first_frame = 1;
        int playery, i;
        PALETTE output;

        map_y = 0;
        set_palette(black_palette);

	for(;;) {
		SCROLL;
		if(display_map() == -1)
			return -1;

		if(first_frame) {

                        for(i=0;i<65;i++) {

				fade_interpolate(black_palette, map.GetPal(), output, i, 0, 255);
                        	SCROLL;
                                set_palette(output);
				if(display_map() == -1)
                                	return -1;
			}
//			fade_in(map.GetPal(), 2);
			first_frame = 0;
		}

		if(key[KEY_ESC])
			return 0;
#ifdef DJGPP
		if(key[KEY_F12]) {

                	while(key[KEY_F12]);

			PALETTE pal;
                        char name[80];
                        int counter = 0;

                        do {

                        	sprintf(name, "dump%d.pcx", counter++);
                        } while (!access(name, F_OK));

			get_palette(pal);
			BITMAP *bmp = create_sub_bitmap(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			save_bitmap(name, bmp, pal);
			destroy_bitmap(bmp);
		}
#endif
		if (level_complete)
			return level_complete;
	}
}

void Engine::PushMessage(char *msg, int prior) {
        static UrbanFont *font = NULL;

        if (font == NULL)
                font = new UrbanFont(SMALL_FONT);

	messages[0].priority = prior;
        messages[0].msg = font->print(msg);
        messages[0].delay = 0;
        num_messages = 1;
#if 0
        if (num_messages < MAX_MESSAGES) {
        	messages[num_messages].priority = prior;
                messages[num_messages].msg = font->print(msg);
                messages[num_messages].delay = 0;
                num_messages++;
/*	       	memmove(&messages[1], &messages[0], sizeof(struct MessageQueue) * num_messages);
	       	messages[0].priority = prior;
        	messages[0].msg = font->print(msg);
		messages[0].delay = 0;
        	num_messages++;*/
	}
/*
        if (num_messages < MAX_MESSAGES) {
		messages[num_messages++] = font->print(msg);
	}*/
#endif
}

void Engine::PopMessage() {
	if (num_messages > 0) {
        	destroy_bitmap(messages[0].msg);
                num_messages--;
	}
}


void show_important_stuff() {
}


