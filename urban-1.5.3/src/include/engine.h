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
#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "map.h"
#include "object.h"
#include "icache.h"
#include "sound.h"
#include "score.h"

#define MAX_OBJECTS 		1000
#define MAX_INNERLAYER 		100
#define MAX_EFFECTS_OBJECTS	400
#define MAX_DEKORATION_OBJECTS	400
#define MAX_MAP_EFFECT	 	400
#define MAX_MAP_DEKORATION_OBJECTS	400
#define MAX_SHELL_OBJECTS	100
#define MAX_MESSAGES		10
#define SCREEN_WIDTH 		320
#define SCREEN_HEIGHT 		240
#define TILE_SIZE     		1250
#define TILE_WIDTH    		26
#define TILE_HEIGHT   		40
#define ENGINE 			(*engine)
#define TILE_SIDE_HEIGHT 	24
#define TILE_TOP_HEIGHT 	(TILE_HEIGHT - TILE_SIDE_HEIGHT)
#define MAX_Z			(TILE_TOP_HEIGHT >> 1)
#define MIN_Z			(3 * TILE_TOP_HEIGHT - 5)
#define DELETE_ME 		-1
#define REMOVE_ME 		-2
#define NUM_BUFFERS		1

#define PLAYER ENGINE.player
#define SOUND ENGINE.sound
class Engine;

extern Engine ENGINE;

struct Buffer {
	BITMAP *buffer;
        volatile int updated;
        volatile int offsetx;
        volatile int offsety;
};

#define LOW_PRIORITY	0x01
#define HIGH_PRIORITY	0x02

struct MessageQueue {
	BITMAP *msg;
        int priority;
        int delay;
};

struct PlayerData {
	int num_lives;
        int weapon_ammo[10];
        int weapon_avail[10];
        int health;
        int score;
};

extern "C" {
	void screen_update();
}

class Engine {
	public:
        	Engine();
                ~Engine();
                void NewGame();
                int play_level(char *map_name, struct PlayerData *p_dat = NULL, int controls = 0x02); //OBS!! <- HACK
		void create_objects();

		void ClearLevel() {level_complete = 1;}
                void PlayExtraLevel() {level_complete = 2;}
                void EnableEarthquake();
                void EnableLightening();

                void SetMaxUpdateX(int M) {max_update_area_x = M;}
                void SetMaxUpdateY(int M) {max_update_area_y = M;}
                void SetMinUpdateX(int M) {min_update_area_x = M;}
                void SetMinUpdateY(int M) {min_update_area_y = M;}

                int check_floor(int x, int y, int z);
                int check_wall(int x, int y, int z);
                void ClearTile(int x, int y, int z);
                void SetTile(int x, int y, int z, int t);
		void check_collision();
		int check_collision(Object *o);
		void create_object(Object *obj);
		void create_effect(Object *obj);
		void create_alwaysupdate(Object *obj);
		void create_dekoration(Object *obj);
                void create_shell(Object *obj);
                char *GetLevelName() {return level_name;};

                void PushMessage(char *msg, int prior = LOW_PRIORITY);
                Object *player;
                Sound sound;
		BITMAP *buffer;
		Buffer buffers[3];
		int do_shake;
                ScoreBoard score;
        	Map map;

	private:

                void UpdateLightening();

                volatile char level_complete;

                Object *objects[MAX_OBJECTS];
                Object *alwaysupdate[MAX_OBJECTS];
                Object *effects[MAX_EFFECTS_OBJECTS];
                Object *dekorations[MAX_DEKORATION_OBJECTS];
                Object *shells[MAX_SHELL_OBJECTS];  //Oanvänd?
                Object *innerlayer[MAX_INNERLAYER];
                Object *map_effect[MAX_MAP_EFFECT];
                Object *map_dekor[MAX_MAP_DEKORATION_OBJECTS];

                int num_objects;
                int num_effects;
                int num_dekorations;
                int num_innerlayer;
                int num_alwaysupdate;
                int num_shells;
                int num_pre_dekor;
                int num_pre_effect;
                int lightening;

                int map_x, map_y;
                char *level_name;

                int display_map();
        	int ringbuffer;
                int ringbuffer2;
                int control_type;

		int play_loop();
                void PopMessage();

//		BITMAP *messages[MAX_MESSAGES];
		struct MessageQueue messages[MAX_MESSAGES];
                int num_messages;
//                friend screen_update();
		/* options till motorn */
                int always_shake; //banan skakar konstant
                int max_time; //tid till undergång!

                //OBS!!! HACK
                int max_update_area_x;
                int max_update_area_y;
                int min_update_area_x;
                int min_update_area_y;

};

#endif
