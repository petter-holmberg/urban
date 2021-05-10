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

#include "icache.h"
#include "map.h"
#include "object.h"
#include "score.h"
#include "sound.h"
#include <array>
#include <atomic>

inline constexpr auto MAX_OBJECTS = 1000;
inline constexpr auto MAX_INNERLAYER = 100;
inline constexpr auto MAX_EFFECTS_OBJECTS = 400;
inline constexpr auto MAX_DEKORATION_OBJECTS = 400;
inline constexpr auto MAX_MAP_EFFECT = 400;
inline constexpr auto MAX_MAP_DEKORATION_OBJECTS = 400;
inline constexpr auto MAX_SHELL_OBJECTS = 100;
inline constexpr auto MAX_MESSAGES = 10;
inline constexpr auto SCREEN_WIDTH = 320;
inline constexpr auto SCREEN_HEIGHT = 240;
inline constexpr auto TILE_SIZE = 1250;
inline constexpr auto TILE_WIDTH = 26;
inline constexpr auto TILE_HEIGHT = 40;
inline constexpr auto TILE_SIDE_HEIGHT = 24;
inline constexpr auto TILE_TOP_HEIGHT = TILE_HEIGHT - TILE_SIDE_HEIGHT;
inline constexpr auto MAX_Z = TILE_TOP_HEIGHT / 2;
inline constexpr auto MIN_Z = 3 * TILE_TOP_HEIGHT - 5;
inline constexpr auto DELETE_ME = -1;
inline constexpr auto REMOVE_ME = -2;
inline constexpr auto NUM_BUFFERS = 1;

#define ENGINE (*engine)
#define PLAYER ENGINE.player
#define SOUND ENGINE.sound

class Engine;

extern Engine ENGINE;

struct Buffer {
    BITMAP* buffer;
};

inline constexpr auto LOW_PRIORITY = 0x01;
inline constexpr auto HIGH_PRIORITY = 0x02;

struct MessageQueue {
    BITMAP* msg;
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

void screen_update();

class Engine {
public:
    Engine();
    ~Engine();
    void NewGame();
    int play_level(const char* map_name, PlayerData* p_dat = nullptr, int controls = 0x02); //OBS!! <- HACK
    void create_objects();

    void ClearLevel() { level_complete = 1; }
    void PlayExtraLevel() { level_complete = 2; }
    void EnableEarthquake();
    void EnableLightening();

    void SetMaxUpdateX(int M) { max_update_area_x = M; }
    void SetMaxUpdateY(int M) { max_update_area_y = M; }
    void SetMinUpdateX(int M) { min_update_area_x = M; }
    void SetMinUpdateY(int M) { min_update_area_y = M; }

    int check_floor(int x, int y, int z);
    int check_wall(int x, int y, int z);
    void ClearTile(int x, int y, int z);
    void SetTile(int x, int y, int z, int t);
    void check_collision();
    int check_collision(Object* o);
    void create_object(Object* obj);
    void create_effect(Object* obj);
    void create_alwaysupdate(Object* obj);
    void create_dekoration(Object* obj);
    void create_shell(Object* obj);
    char* GetLevelName() { return level_name; };

    void PushMessage(const char* msg, int prior = LOW_PRIORITY);
    Object* player;
    Sound sound;
    BITMAP* buffer;
    Buffer buffers[3] {};
    int do_shake {};
    ScoreBoard score;
    Map map;

private:
    void UpdateLightening();

    std::atomic<char> level_complete {};

    std::array<Object*, MAX_OBJECTS> objects {};
    std::array<Object*, MAX_OBJECTS> alwaysupdate {};
    std::array<Object*, MAX_EFFECTS_OBJECTS> effects {};
    std::array<Object*, MAX_DEKORATION_OBJECTS> dekorations {};
    std::array<Object*, MAX_SHELL_OBJECTS> shells {}; //Oanvänd?
    std::array<Object*, MAX_INNERLAYER> innerlayer {};
    std::array<Object*, MAX_MAP_EFFECT> map_effect {};
    std::array<Object*, MAX_MAP_DEKORATION_OBJECTS> map_dekor {};

    int num_objects;
    int num_effects;
    int num_dekorations;
    int num_innerlayer;
    int num_alwaysupdate;
    int num_shells {};
    int num_pre_dekor;
    int num_pre_effect;
    int lightening {};

    int map_x {}, map_y {};
    char* level_name {};

    int display_map();
    int ringbuffer;
    int ringbuffer2;
    int control_type {};

    int play_loop();
    void PopMessage();

    MessageQueue messages[MAX_MESSAGES] {};
    int num_messages;
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
