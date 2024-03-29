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
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "allegro.h"
#include "anim.h"
#include <vector>

inline constexpr auto RIGHT_DIR = 0;
inline constexpr auto LEFT_DIR = 1;
inline constexpr auto UP_DIR = 2;
inline constexpr auto DOWN_DIR = 3;

inline constexpr auto EVIL_PLAYER = 0;
inline constexpr auto EVIL_ENEMY = 1;

inline constexpr auto FRIEND_PLAYER = 0x01;
inline constexpr auto FRIEND_SCIENTIST = 0x02;
inline constexpr auto FRIEND_SOLDIER = 0x04;
inline constexpr auto FRIEND_GRENADE = 0x08;
inline constexpr auto FRIEND_HS_BULLET = 0x10;
inline constexpr auto FRIEND_MINE = 0x20;
inline constexpr auto FRIEND_POWERUP = 0x40;
inline constexpr auto FRIEND_CARD = 0x80;
inline constexpr auto FRIEND_SOLDIER2 = 0x100;
inline constexpr auto FRIEND_EXPLOSION = 0x200;
inline constexpr auto FRIEND_FIREBALL = 0x400;
inline constexpr auto FRIEND_VAKTM = 0x800;
inline constexpr auto FRIEND_ICEBALL = 0x1000;
inline constexpr auto FRIEND_FROZENBODY = 0x2000;
inline constexpr auto FRIEND_RAMBO = 0x4000;
inline constexpr auto FRIEND_CHECKPOINT = 0x8000;
inline constexpr auto FRIEND_DOOR = 0x10000;
inline constexpr auto FRIEND_AGENT = 0x20000;
inline constexpr auto FRIEND_DEKOR = 0x40000;
inline constexpr auto FRIEND_ELEVATOR = 0x80000;
inline constexpr auto FRIEND_ELEVSTAT = 0x100000;
inline constexpr auto FRIEND_ELEVWIRE = 0x200000;
inline constexpr auto FRIEND_WATER = 0x400000;
inline constexpr auto FRIEND_BUBBLE = 0x800000;
inline constexpr auto FRIEND_BEAM = 0x1000000;
inline constexpr auto FRIEND_TANK = 0x2000000;
inline constexpr auto FRIEND_PLASMA = 0x4000000;

inline constexpr auto ENEMY_PLAYER = 0x01;
inline constexpr auto ENEMY_SCIENTIST = 0x02;
inline constexpr auto ENEMY_SOLDIER = 0x04;
inline constexpr auto ENEMY_GRENADE = 0x08;
inline constexpr auto ENEMY_HS_BULLET = 0x10;
inline constexpr auto ENEMY_MINE = 0x20;
inline constexpr auto ENEMY_POWERUP = 0x40;
inline constexpr auto ENEMY_CARD = 0x80;
inline constexpr auto ENEMY_SOLDIER2 = 0x100;
inline constexpr auto ENEMY_EXPLOSION = 0x200;
inline constexpr auto ENEMY_FIREBALL = 0x400;
inline constexpr auto ENEMY_VAKTM = 0x800;
inline constexpr auto ENEMY_ICEBALL = 0x1000;
inline constexpr auto ENEMY_FROZENBODY = 0x2000;
inline constexpr auto ENEMY_RAMBO = 0x4000;
inline constexpr auto ENEMY_CHECKPOINT = 0x8000;
inline constexpr auto ENEMY_DOOR = 0x10000;
inline constexpr auto ENEMY_AGENT = 0x20000;
inline constexpr auto ENEMY_DEKOR = 0x40000;
inline constexpr auto ENEMY_ELEVATOR = 0x80000;
inline constexpr auto ENEMY_ELEVSTAT = 0x100000;
inline constexpr auto ENEMY_ELEVWIRE = 0x200000;
inline constexpr auto ENEMY_WATER = 0x400000;
inline constexpr auto ENEMY_BUBBLE = 0x800000;
inline constexpr auto ENEMY_BEAM = 0x1000000;
inline constexpr auto ENEMY_TANK = 0x2000000;
inline constexpr auto ENEMY_PLASMA = 0x4000000;

inline constexpr auto MAKE_PLAYER_IMMORTAL = ENEMY_SOLDIER | ENEMY_SOLDIER2 | ENEMY_RAMBO;

inline constexpr auto MODE_NORMAL = 0x01;
inline constexpr auto MODE_WATER = 0x02;

inline constexpr auto SHOTGUN = 0;
inline constexpr auto FLAME_THROWER = 1;
inline constexpr auto ICEMAKER = 2;
inline constexpr auto GRENADE_LAUNCHER = 3;
inline constexpr auto PLASMA_GUN = 4;
inline constexpr auto MINIGUN = 5;
inline constexpr auto ELECTRIC = 6;

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
    std::vector<BITMAP*> images {};
    int x {}, y {}, z {};
    int layer {};
    int speed_x {}, speed_y {}, speed_z {};
    size_t current_image {};
    int width {}, height {};
    int mode {};

    int coll_x {}, coll_y {};
    int coll_width {}, coll_height {};

    int direction {};
    int state {};
    int counter {}, counter2 {}, counter3 {}, counter4 {}, counter5 {};
    float fcounter {}, fcounter2 {}, fcounter3 {};
    int grav_counter {};

    int score {};
    unsigned long friends {}, enemies {}, me {};

    int energy {}; //m�ngden liv gubben har
    int strength {}; //hur mycket man ska bli skadad av gubben
};

class player_o : public Object {
public:
    player_o(int X, int Y, int Z, int controls = 0x02); //OBS!! <- HACK
    void MoveTo(int X, int Y, int Z);
    int update();
    virtual ~player_o();
    player_o(const player_o&) = delete;
    player_o& operator=(const player_o&) = delete;
    void Collision(Object*);

    /* Unikt f�r spelarobjektet */
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
    Anim anim {};
    Controllers* ctrl {};
    Weapon* weapon {};
    Object* elevator {};
    Object* elev_station {};
    int current_weapon {};
    int lives {};
    int save_x {}, save_y {}, save_z {};
    int control_type {};
    char card {};
};

class scientist_o : public Object {
public:
    scientist_o(int X, int Y, int Z);
    int update();
    virtual ~scientist_o();
    void Collision(Object*);

private:
    Anim anim {};
};

class Einstein_o : public Object {
public:
    Einstein_o(int X, int Y, int Z);
    int update();
    virtual ~Einstein_o();
    void Collision(Object*);

private:
    Anim anim {};
};

class Vaktm_o : public Object {
public:
    Vaktm_o(int X, int Y, int Z);
    int update();
    virtual ~Vaktm_o();
    void Collision(Object*);

private:
    Anim anim {};
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
    Anim anim {};
};

class soldier2_o : public Object {
public:
    soldier2_o(int X, int, int Y, int Z);
    soldier2_o(int X, int Y, int Z);
    int update();
    virtual ~soldier2_o();
    void Collision(Object*);

private:
    Anim anim {};
};

class Rambo_o : public Object {
public:
    Rambo_o(int X, int Y, int Z);
    int update();
    virtual ~Rambo_o();
    void Collision(Object*);

private:
    Anim anim {};
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
    Anim anim {};
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
    Anim anim {};
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
    Anim anim {};
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
    /* Unikt f�r Spy */
    void ResetSpy() { victim = 0; }
    int GetSpy() { return victim; }

private:
    int victim {};
};

class FloorGun_o : public Object {
public:
    FloorGun_o(int X, int Y, int Z);
    int update();
    virtual ~FloorGun_o();
    FloorGun_o(const FloorGun_o&) = delete;
    FloorGun_o& operator=(const FloorGun_o&) = delete;
    void Collision(Object*);

private:
    Anim anim {};
    Spy_o* spy {};
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
inline constexpr auto POWERUP_ENERGY = 1;
inline constexpr auto POWERUP_AMMO_SHOTGUN = 2;
inline constexpr auto POWERUP_AMMO_ICEMAKER = 3;
inline constexpr auto POWERUP_AMMO_FLAME_THROWER = 4;
inline constexpr auto POWERUP_AMMO_GRENADE_LAUNCHER = 5;
inline constexpr auto POWERUP_AMMO_GRENADE_LAUNCHER6 = 6;
inline constexpr auto POWERUP_WEAPON_FLAME_THROWER = 7;
inline constexpr auto POWERUP_WEAPON_ICEMAKER = 8;
inline constexpr auto POWERUP_WEAPON_GRENADE_LAUNCHER = 9;
inline constexpr auto POWERUP_EXTRA_LIFE = 10;
inline constexpr auto POWERUP_AMMO_PLASMA = 11;
inline constexpr auto POWERUP_WEAPON_PLASMA = 12;
inline constexpr auto POWERUP_AMMO_MINIGUN = 13;
inline constexpr auto POWERUP_WEAPON_MINIGUN = 14;
inline constexpr auto POWERUP_AMMO_ELECTRIC = 15;
inline constexpr auto POWERUP_WEAPON_ELECTRIC = 16;

//Increases the energy or ammo of the player
class powerup_o : public Object {
public:
    powerup_o(int X, int Y, int Z, int Type);
    void Collision(Object*);
    int update();
    virtual ~powerup_o();

private:
    int type {};
    char msg[128] {};
};

class card_o : public Object {
public:
    card_o(int X, int Y, int Z, Cardtype Card = green);
    void Collision(Object*);
    int update();
    virtual ~card_o();

private:
    Cardtype card {};
};

class Door_o : public Object {
public:
    Door_o(int X, int Y, int Z, Cardtype Card = green);
    int update();
    void Collision(Object*);
    virtual ~Door_o();

private:
    Cardtype card {};
    Anim anim {};
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
    HighSpeed_Bullet_o(int X, int Y, int Z, int Dir, int friendship = EVIL_ENEMY, int Damage = 1);
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
    Anim anim {};
};

class IceBall_o : public Object {
public:
    IceBall_o(int X, int Y, int Z, int Friends = 0, int SpeedX = 0, int SpeedY = 0, int SpeedZ = 0);
    int update();
    void Collision(Object*);
    virtual ~IceBall_o();

private:
    Anim anim {};
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
    Anim anim {};
};

using shells_o = bodyparts_o;

class BurningBody_o : public Object {
public:
    BurningBody_o(int X, int Y, int Z, int Move = 1);
    int update();
    virtual ~BurningBody_o();

private:
    Anim anim {};
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
    Anim anim {};
};

class Barrel_o : public Object {
public:
    Barrel_o(int X, int Y, int Z, int Type = 0);
    int update();
    virtual ~Barrel_o();
    void Collision(Object*);

private:
    int type {};
};

class Animation_o : public Object {
public:
    Animation_o(int X, int Y, int Z, const char* name, int num_pics, int dek_frame = -1, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0);
    int update();
    virtual ~Animation_o();

private:
    Anim anim {};
    int DekorationFrame {};
};

class explosion_o : public Object {
public:
    explosion_o(int X, int Y, int Z, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0, int Friends = 0);
    int update();
    virtual ~explosion_o();
    void Collision(Object*);

private:
    Anim anim {};
};

class gnistor_o : public Object {
public:
    gnistor_o(int X, int Y, int Z, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0, int Direction = 0);
    int update();
    virtual ~gnistor_o();
    void Collision(Object*);

private:
    Anim anim {};
};

#endif
