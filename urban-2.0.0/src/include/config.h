#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "ctrls.h"
#include "keyinfo.h"

inline constexpr auto QUALITY_NORMAL = 1;
inline constexpr auto QUALITY_HIGH = 2;

struct KeyConf {
    scan_code key_up = scan_code::KEY_UP;
    scan_code key_down = scan_code::KEY_DOWN;
    scan_code key_left = scan_code::KEY_LEFT;
    scan_code key_right = scan_code::KEY_RIGHT;
    scan_code key_jump = scan_code::KEY_SPACE;
    scan_code key_fire = scan_code::KEY_LCONTROL;
    scan_code key_prevweapon = scan_code::KEY_LSHIFT;
    scan_code key_nextweapon = scan_code::KEY_RSHIFT;
};

class Config {
public:
    Config();
    void Start();
    KeyConf keyconf;
    int ctrl_type = CONTROLLER_KEYBOARD;
    int music_vol = 90;
    int sfx_vol = 128;
    int gfx_quality = QUALITY_NORMAL;

private:
    void StartControls();
    void ChangeGFXQuality();
    void ChangeMusicVol();
    void ChangeSFXVol();
    static scan_code GetKey(const char* Label);
    int GetButton(const char* Label);
    void ConfigureKeyboard();
    void ConfigureGamepad();
    void Load();
    void Save();
};

#endif
