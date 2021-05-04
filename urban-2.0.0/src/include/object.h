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
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "allegro.h"
#include "anim.h"

#define RIGHT_DIR 0
#define LEFT_DIR 1
#define UP_DIR 2
#define DOWN_DIR 3

#define EVIL_PLAYER 0
#define EVIL_ENEMY 1

#define FRIEND_PLAYER 0x01
#define FRIEND_SCIENTIST 0x02
#define FRIEND_SOLDIER 0x04
#define FRIEND_GRENADE 0x08
#define FRIEND_HS_BULLET 0x10
#define FRIEND_MINE 0x20
#define FRIEND_POWERUP 0x40
#define FRIEND_CARD 0x80
#define FRIEND_SOLDIER2 0x100
#define FRIEND_EXPLOSION 0x200
#define FRIEND_FIREBALL 0x400
#define FRIEND_VAKTM 0x800
#define FRIEND_ICEBALL 0x1000
#define FRIEND_FROZENBODY 0x2000
#define FRIEND_RAMBO 0x4000
#define FRIEND_CHECKPOINT 0x8000
#define FRIEND_DOOR 0x10000
#define FRIEND_AGENT 0x20000
#define FRIEND_DEKOR 0x40000
#define FRIEND_ELEVATOR 0x80000
#define FRIEND_ELEVSTAT 0x100000
#define FRIEND_ELEVWIRE 0x200000
#define FRIEND_WATER 0x400000
#define FRIEND_BUBBLE 0x800000
#define FRIEND_BEAM 0x1000000
#define FRIEND_TANK 0x2000000
#define FRIEND_PLASMA 0x4000000

#define ENEMY_PLAYER 0x01
#define ENEMY_SCIENTIST 0x02
#define ENEMY_SOLDIER 0x04
#define ENEMY_GRENADE 0x08
#define ENEMY_HS_BULLET 0x10
#define ENEMY_MINE 0x20
#define ENEMY_POWERUP 0x40
#define ENEMY_CARD 0x80
#define ENEMY_SOLDIER2 0x100
#define ENEMY_EXPLOSION 0x200
#define ENEMY_FIREBALL 0x400
#define ENEMY_VAKTM 0x800
#define ENEMY_ICEBALL 0x1000
#define ENEMY_FROZENBODY 0x2000
#define ENEMY_RAMBO 0x4000
#define ENEMY_CHECKPOINT 0x8000
#define ENEMY_DOOR 0x10000
#define ENEMY_AGENT 0x20000
#define ENEMY_DEKOR 0x40000
#define ENEMY_ELEVATOR 0x80000
#define ENEMY_ELEVSTAT 0x100000
#define ENEMY_ELEVWIRE 0x200000
#define ENEMY_WATER 0x400000
#define ENEMY_BUBBLE 0x800000
#define ENEMY_BEAM 0x1000000
#define ENEMY_TANK 0x2000000
#define ENEMY_PLASMA 0x4000000

#define MAKE_PLAYER_IMMORTAL (ENEMY_SOLDIER | ENEMY_SOLDIER2 | ENEMY_RAMBO)

#define shells_o bodyparts_o

#define MODE_NORMAL 0x01
#define MODE_WATER 0x02

#define SHOTGUN 0
#define FLAME_THROWER 1
#define ICEMAKER 2
#define GRENADE_LAUNCHER 3
#define PLASMA_GUN 4
#define MINIGUN 5
#define ELECTRIC 6

class Controllers;
enum Cardtype { none = 0,
    green = 1,
    red = 2,
    blue = 4,
    yellow = 8,
    extra_level = 16 };

struct Weapon {
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
    char waterproof;
    long max_ammo;
};

class Object {
public:
    Object(int X, int Y, int Z, int Mode = MODE_NORMAL);
    virtual ~Object();
    BITMAP* GetImage();
    virtual int update();
    int GetX() { return x; }
    int GetY() { return y; }
    int GetZ() { return z; }
    void SetX(int X) { x = X; }
    void SetY(int Y) { y = Y; }
    void SetZ(int Z) { z = Z; }
    int GetLayer() { return layer; }
    int GetScore() { return score; }
    int GetEnergy() { return energy; }
    int GetWidth() { return width; }
    int GetHeight() { return height; }

    int GetCollisionX() { return x + coll_x; }
    int GetCollisionY() { return y + coll_y; }
    int GetCollisionHeight() { return coll_height; }
    int GetCollisionWidth() { return coll_width; }

    int GetSpeedX() { return speed_x; }
    int GetSpeedY() { return speed_y; }
    int GetSpeedZ() { return speed_z; }
    int GetEnemies() { return enemies; }
    int GetFriends() { return friends; }
    int GetWho() { return me; }
    int GetDirection() { return direction; }
    virtual void Collision(Object*);

    void SetMode(int Mode) { mode = Mode; }
    int GetMode() { return mode; }

    int GetStrength() { return strength; }
    int SetStrength(int s) { return (strength += s); }
    int SetHealth(int s) { return (strength = s); }
    int SetEnergy(int s) { return (energy = s); }
    int IsImmortal() { return state & 0x80; }; // STATE_IMMORTAL = 0x80
protected:
    BITMAP** images;
    int x, y, z;
    int layer;
    int speed_x, speed_y, speed_z;
    int current_image;
    int num_images;
    int width, height;
    int mode;

    int coll_x, coll_y;
    int coll_width, coll_height;

    int direction;
    int state;
    int counter, counter2, counter3, counter4, counter5;
    float fcounter, fcounter2, fcounter3;
    int grav_counter;

    int score;
    unsigned long friends, enemies, me;

    int energy; //mängden liv gubben har
    int strength; //hur mycket man ska bli skadad av gubben
};

class player_o : public Object {
public:
    player_o(int X, int Y, int Z, int controls = 0x02); //OBS!! <- HACK
    void MoveTo(int X, int Y, int Z);
    int update();
    virtual ~player_o();
    void Collision(Object*);

    /* Unikt för spelarobjektet */
    void SetAmmo(int type, int num) { weapon[type].ammo = num; }

    void AddAmmo(int type, int num);
    void EnableWeapon(int type)
    {
        weapon[type].availible = 1;
        if (type > current_weapon)
            current_weapon = type;
    }
    void DisableWeapon(int type) { weapon[type].availible = 0; }
    int GetAmmo() { return weapon[current_weapon].ammo; }
    int GetAmmo(int t) { return weapon[t].ammo; }
    int GetWeapon() { return current_weapon; }
    int HaveWeapon(int type) { return weapon[type].availible; }
    void SetLife(int n) { lives = n; }
    void AddLife() { lives++; }
    void RemoveLife() { lives--; }
    int GetLives() { return lives; }
    void GiveCard(Cardtype Card) { card |= Card; }
    int HaveCard(Cardtype Card) { return card & Card; }
    int NewLevel();
    void KillPlayer() { PerformDeath(0); }

private:
    void PerformDeath(int typ);
    Anim anim;
    Controllers* ctrl;
    Weapon* weapon;
    Object* elevator;
    Object* elev_station;
    int current_weapon;
    int lives;
    int save_x, save_y, save_z;
    int control_type;
    char card;
};

class scientist_o : public Object {
public:
    scientist_o(int X, int Y, int Z);
    int update();
    virtual ~scientist_o();
    void Collision(Object*);

private:
    Anim anim;
};

class Einstein_o : public Object {
public:
    Einstein_o(int X, int Y, int Z);
    int update();
    virtual ~Einstein_o();
    void Collision(Object*);

private:
    Anim anim;
};

class Vaktm_o : public Object {
public:
    Vaktm_o(int X, int Y, int Z);
    int update();
    virtual ~Vaktm_o();
    void Collision(Object*);

private:
    Anim anim;
};

class Boss_scientist_o : public scientist_o {
public:
    Boss_scientist_o(int X, int Y, int Z);
    int update();
    virtual ~Boss_scientist_o();
};

class Boss_Einstein_o : public Einstein_o {
public:
    Boss_Einstein_o(int X, int Y, int Z);
    int update();
    virtual ~Boss_Einstein_o();
};

class soldier_o : public Object {
public:
    soldier_o(int X, int, int Y, int Z);
    soldier_o(int X, int Y, int Z);
    int update();
    virtual ~soldier_o();
    void Collision(Object*);

private:
    Anim anim;
};

class soldier2_o : public Object {
public:
    soldier2_o(int X, int, int Y, int Z);
    soldier2_o(int X, int Y, int Z);
    int update();
    virtual ~soldier2_o();
    void Collision(Object*);

private:
    Anim anim;
};

class Rambo_o : public Object {
public:
    Rambo_o(int X, int Y, int Z);
    int update();
    virtual ~Rambo_o();
    void Collision(Object*);

private:
    Anim anim;
};

class Boss_Rambo_o : public Rambo_o {
public:
    Boss_Rambo_o(int X, int Y, int Z);
    int update();
    virtual ~Boss_Rambo_o();
};

class Arnold_o : public Object {
public:
    Arnold_o(int X, int Y, int Z);
    int update();
    virtual ~Arnold_o();
    void Collision(Object*);

private:
    Anim anim;
};

class Boss_Arnold_o : public Arnold_o {
public:
    Boss_Arnold_o(int X, int Y, int Z);
    int update();
    virtual ~Boss_Arnold_o();
};

class Dolph_o : public Object {
public:
    Dolph_o(int X, int Y, int Z);
    int update();
    virtual ~Dolph_o();
    void Collision(Object*);

private:
    Anim anim;
};

class Boss_Dolph_o : public Dolph_o {
public:
    Boss_Dolph_o(int X, int Y, int Z);
    int update();
    virtual ~Boss_Dolph_o();
};

class Crycheck_o : public Object {
public:
    Crycheck_o(int X, int Y, int Z);
    int update();
    virtual ~Crycheck_o();
    void Collision(Object*);

private:
    Anim anim;
};

class Boss_Crycheck_o : public Crycheck_o {
public:
    Boss_Crycheck_o(int X, int Y, int Z);
    int update();
    virtual ~Boss_Crycheck_o();
};

class Spy_o : public Object {
public:
    Spy_o(int X, int Y, int Z);
    int update();
    virtual ~Spy_o();
    void Collision(Object*);
    /* Unikt för Spy */
    void ResetSpy() { victim = 0; }
    int GetSpy() { return victim; }

private:
    int victim;
};

class FloorGun_o : public Object {
public:
    FloorGun_o(int X, int Y, int Z);
    int update();
    virtual ~FloorGun_o();
    void Collision(Object*);

private:
    Anim anim;
    Spy_o* spy;
};

class Boss_soldier_o : public soldier_o {
public:
    Boss_soldier_o(int X, int Y, int Z);
    int update();
    virtual ~Boss_soldier_o();
};

class Boss_soldier2_o : public soldier2_o {
public:
    Boss_soldier2_o(int X, int Y, int Z);
    int update();
    virtual ~Boss_soldier2_o();
};

//projektiler
class grenade_o : public Object {
public:
    grenade_o(int X, int Y, int Z, int SpeedX = 0, int SpeedY = 0, int SpeedZ = 0);
    void Collision(Object*);
    int update();
    virtual ~grenade_o();
    //                static void init();
};
class rocket_o : public Object {
public:
    rocket_o(int X, int Y, int Z, int Speed, int Mode = MODE_NORMAL);
    void Collision(Object*);
    int update();
    virtual ~rocket_o();
    //                static void init();
};
#define POWERUP_ENERGY 1
#define POWERUP_AMMO_SHOTGUN 2
#define POWERUP_AMMO_ICEMAKER 3
#define POWERUP_AMMO_FLAME_THROWER 4
#define POWERUP_AMMO_GRENADE_LAUNCHER 5
#define POWERUP_AMMO_GRENADE_LAUNCHER6 6
#define POWERUP_WEAPON_FLAME_THROWER 7
#define POWERUP_WEAPON_ICEMAKER 8
#define POWERUP_WEAPON_GRENADE_LAUNCHER 9
#define POWERUP_EXTRA_LIFE 10
#define POWERUP_AMMO_PLASMA 11
#define POWERUP_WEAPON_PLASMA 12
#define POWERUP_AMMO_MINIGUN 13
#define POWERUP_WEAPON_MINIGUN 14
#define POWERUP_AMMO_ELECTRIC 15
#define POWERUP_WEAPON_ELECTRIC 16

//Increases the energy or ammo of the player
class powerup_o : public Object {
public:
    powerup_o(int X, int Y, int Z, int Type);
    void Collision(Object*);
    int update();
    virtual ~powerup_o();

private:
    int type;
    char msg[128];
};

class card_o : public Object {
public:
    card_o(int X, int Y, int Z, Cardtype Card = green);
    void Collision(Object*);
    int update();
    virtual ~card_o();

private:
    Cardtype card;
};

class Door_o : public Object {
public:
    Door_o(int X, int Y, int Z, Cardtype Card = green);
    int update();
    void Collision(Object*);
    virtual ~Door_o();

private:
    Cardtype card;
    Anim anim;
};

class checkpoint_o : public Object {
public:
    checkpoint_o(int X, int Y, int Z);
    void Collision(Object*);
    int update();
    virtual ~checkpoint_o();
};

class mine_o : public Object {
public:
    mine_o(int X, int Y, int Z);
    void Collision(Object*);
    int update();
    virtual ~mine_o();

private:
};

class HighSpeed_Bullet_o : public Object {
public:
    HighSpeed_Bullet_o(int X, int Y, int Z, int Dir, int Evilness = EVIL_ENEMY, int Damage = 1);
    int update();
    void Collision(Object*);
    virtual ~HighSpeed_Bullet_o();
    //                static void init();
};

class FireBall_o : public Object {
public:
    FireBall_o(int X, int Y, int Z, int Friends = 0, int SpeedX = 0, int SpeedY = 0, int SpeedZ = 0, int Strength = 2);
    int update();
    void Collision(Object*);
    virtual ~FireBall_o();

private:
    Anim anim;
};

class IceBall_o : public Object {
public:
    IceBall_o(int X, int Y, int Z, int Friends = 0, int SpeedX = 0, int SpeedY = 0, int SpeedZ = 0);
    int update();
    void Collision(Object*);
    virtual ~IceBall_o();

private:
    Anim anim;
};

class blood_o : public Object {
public:
    blood_o(int X, int Y, int Z, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0);
    int update();
    virtual ~blood_o();
};

class bodyparts_o : public Object {
public:
    bodyparts_o(int X, int Y, int Z, const char* name, int num_pics, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0);
    int update();
    virtual ~bodyparts_o();

private:
    Anim anim;
};

class BurningBody_o : public Object {
public:
    BurningBody_o(int X, int Y, int Z, int Move = 1);
    int update();
    virtual ~BurningBody_o();

private:
    Anim anim;
};

class FrosenBody_o : public Object {
public:
    FrosenBody_o(int X, int Y, int Z, int dir);
    int update();
    void Collision(Object*);
    virtual ~FrosenBody_o();
};

class Fire_o : public Object {
public:
    Fire_o(int X, int Y, int Z, int size = 1);
    int update();
    virtual ~Fire_o();

private:
    Anim anim;
};

class Barrel_o : public Object {
public:
    Barrel_o(int X, int Y, int Z, int Type = 0);
    int update();
    virtual ~Barrel_o();
    void Collision(Object*);

private:
    int type;
};

class Animation_o : public Object {
public:
    Animation_o(int X, int Y, int Z, const char* name, int num_pics, int dek_frame = -1, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0);
    int update();
    virtual ~Animation_o();

private:
    Anim anim;
    int DekorationFrame;
};

class explosion_o : public Object {
public:
    explosion_o(int X, int Y, int Z, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0, int Friends = 0);
    int update();
    virtual ~explosion_o();
    void Collision(Object*);

private:
    Anim anim;
};

class gnistor_o : public Object {
public:
    gnistor_o(int X, int Y, int Z, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0, int Direction = 0);
    int update();
    virtual ~gnistor_o();
    void Collision(Object*);

private:
    Anim anim;
};

#endif
