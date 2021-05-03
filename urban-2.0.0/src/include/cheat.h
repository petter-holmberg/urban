#ifndef __CHEAT_H__
#define __CHEAT_H__

struct Letter {
	int key;
	char letter;
};

#define CHEAT_MK_PLAYER_IMMORTAL		0x01
#define CHEAT_GIVE_PLAYER_HEALTH		0x02
#define CHEAT_GIVE_PLAYER_FIRE			0x04
#define CHEAT_GIVE_PLAYER_ICE			0x08
#define CHEAT_GIVE_PLAYER_GRENADE		0x10
#define CHEAT_GIVE_PLAYER_PLASMA		0x20
#define CHEAT_GIVE_PLAYER_MINIGUN		0x40
#define CHEAT_GIVE_PLAYER_ELECTRIC		0x80
#define CHEAT_GIVE_PLAYER_1K_OF_AMMO		0x100
#define CHEAT_GIVE_PLAYER_1K_AMMO_2_ALL		0x200
#define CHEAT_GIVE_PLAYER_MAX_AMMO		0x400
#define CHEAT_GIVE_PLAYER_ALL_WEAPONS		0x800
#define CHEAT_GIVE_PLAYER_CARDS			0x1000
#define CHEAT_CLEAR_LEVEL			0x2000
#define CHEAT_SET_PLAYER_DEADLY			0x4000
#define CHEAT_KILL_PLAYER			0x8000
#define CHEAT_AIRSTRIKE				0x10000

struct CheatCode {
	const char *code;
	unsigned long cheat;
};


extern unsigned long cheat_codes_active;

#endif
