#ifndef __CHEAT_H__
#define __CHEAT_H__

#include "keyinfo.h"

struct Letter {
    scan_code key;
    char letter;
};

inline constexpr auto CHEAT_MK_PLAYER_IMMORTAL = 0x01;
inline constexpr auto CHEAT_GIVE_PLAYER_HEALTH = 0x02;
inline constexpr auto CHEAT_GIVE_PLAYER_FIRE = 0x04;
inline constexpr auto CHEAT_GIVE_PLAYER_ICE = 0x08;
inline constexpr auto CHEAT_GIVE_PLAYER_GRENADE = 0x10;
inline constexpr auto CHEAT_GIVE_PLAYER_PLASMA = 0x20;
inline constexpr auto CHEAT_GIVE_PLAYER_MINIGUN = 0x40;
inline constexpr auto CHEAT_GIVE_PLAYER_ELECTRIC = 0x80;
inline constexpr auto CHEAT_GIVE_PLAYER_1K_OF_AMMO = 0x100;
inline constexpr auto CHEAT_GIVE_PLAYER_1K_AMMO_2_ALL = 0x200;
inline constexpr auto CHEAT_GIVE_PLAYER_MAX_AMMO = 0x400;
inline constexpr auto CHEAT_GIVE_PLAYER_ALL_WEAPONS = 0x800;
inline constexpr auto CHEAT_GIVE_PLAYER_CARDS = 0x1000;
inline constexpr auto CHEAT_CLEAR_LEVEL = 0x2000;
inline constexpr auto CHEAT_SET_PLAYER_DEADLY = 0x4000;
inline constexpr auto CHEAT_KILL_PLAYER = 0x8000;
inline constexpr auto CHEAT_AIRSTRIKE = 0x10000;

struct CheatCode {
    const char* code;
    unsigned long cheat;
};

extern unsigned long cheat_codes_active;

#endif
