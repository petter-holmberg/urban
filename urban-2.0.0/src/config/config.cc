#include <allegro.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/**************************************************************************************/
#include "config.h"
#include "ctrls.h"
#include "engine.h"
#include "game.h"
#include "keyinfo.h"
#include "urbfont.h"
/**************************************************************************************/
void Config::Load()
{
    char filename[1024];
    FILE* fd = nullptr;

    if ((fd = fopen(filename, "rb")) == nullptr) {
        return;
    }

    auto err = fread(&keyconf, sizeof(struct KeyConf), 1, fd);

    fclose(fd);
}
/**************************************************************************************/
void Config::Save()
{
    char filename[1024];
    FILE* fd = nullptr;

    /* Create dir */
    sprintf(filename, "%s/.urban", getenv("HOME"));
    mkdir(filename, S_IRUSR | S_IWUSR | S_IXUSR);

    sprintf(filename, "%s/.urban/ctrl.dat", getenv("HOME"));
    if ((fd = fopen(filename, "wb")) == nullptr) {
        return;
    }

    fwrite(&keyconf, sizeof(struct KeyConf), 1, fd);

    fclose(fd);
}
/**************************************************************************************/
Config::Config()
{
    // Default values

    keyconf.key_up = KEY_UP;
    keyconf.key_down = KEY_DOWN;
    keyconf.key_left = KEY_LEFT;
    keyconf.key_right = KEY_RIGHT;
    keyconf.key_jump = KEY_SPACE;
    keyconf.key_fire = KEY_LCONTROL;
    keyconf.key_prevweapon = KEY_LSHIFT;
    keyconf.key_nextweapon = KEY_RSHIFT;
    keyconf.ctrl_type = CONTROLLER_KEYBOARD;
    keyconf.music_vol = 90;
    keyconf.sfx_vol = 128;
    keyconf.gfx_quality = QUALITY_NORMAL;

    Load();
}
/**************************************************************************************/
void Config::ConfigureKeyboard()
{
    PALETTE Pal;
    int temp = 0;
    BITMAP* bmp = icache.GetImage("ibild.pcx", Pal);
    set_palette(Pal);

    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("UP")) != (-1)) {
        keyconf.key_up = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("DOWN")) != (-1)) {
        keyconf.key_down = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("LEFT")) != (-1)) {
        keyconf.key_left = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("RIGHT")) != (-1)) {
        keyconf.key_right = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("FIRE")) != (-1)) {
        keyconf.key_fire = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("JUMP")) != (-1)) {
        keyconf.key_jump = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("Prev Weapon")) != (-1)) {
        keyconf.key_prevweapon = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("Next Weapon")) != (-1)) {
        keyconf.key_nextweapon = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    keyconf.ctrl_type = CONTROLLER_KEYBOARD;

    icache.FreeImage(bmp);
}
/**************************************************************************************/
void Config::ChangeMusicVol()
{
    PALETTE pal;
    BITMAP* backg = icache.GetImage("ibild.pcx", pal);
    int running = 1;
    int vol = keyconf.music_vol;

    blit(backg, screen, 0, 0, 0, 0, backg->w, backg->h);

    UrbanFont m(LARGE_FONT);
    m.SetScale(65);
    m.print_centre("MUSIC VOLUME", SCREEN_WIDTH / 2, 50);

    rectfill(screen, (SCREEN_WIDTH - 256) / 2 + 2, 102,
        vol + (SCREEN_WIDTH - 256) / 2 + 2, 122, 207);

    rectfill(screen, (SCREEN_WIDTH - 256) / 2, 100,
        vol + (SCREEN_WIDTH - 256) / 2, 120, 192);

    while (running != 0) {
        blit(backg, screen, 0, 100, 0, 100, backg->w, 23);

        rectfill(screen, (SCREEN_WIDTH - 256) / 2 + 2, 102,
            vol + (SCREEN_WIDTH - 256) / 2 + 2, 122, 207);

        rectfill(screen, (SCREEN_WIDTH - 256) / 2, 100,
            vol + (SCREEN_WIDTH - 256) / 2, 120, 192);

        switch ((readkey() >> 8)) { // 207 192

        case KEY_LEFT:
        case KEY_DOWN:

            if (vol > 1) {
                vol -= 2;
            }

            SOUND.SetMusicVolume(vol);
            keyconf.music_vol = vol;
            break;

        case KEY_UP:
        case KEY_RIGHT:

            if (vol < 254) {
                vol += 2;
            }

            SOUND.SetMusicVolume(vol);
            keyconf.music_vol = vol;
            break;

        default:
            running = 0;
            Save();
            break;
        };
    };
    icache.FreeImage(backg);
}
/**************************************************************************************/
void Config::ChangeSFXVol()
{
    PALETTE pal;
    BITMAP* backg = icache.GetImage("ibild.pcx", pal);
    int running = 1;
    int vol = keyconf.sfx_vol;

    blit(backg, screen, 0, 0, 0, 0, backg->w, backg->h);

    UrbanFont m(LARGE_FONT);
    m.SetScale(65);
    m.print_centre("SFX VOLUME", SCREEN_WIDTH / 2, 50);

    rectfill(screen, (SCREEN_WIDTH - 256) / 2 + 2, 102,
        vol + (SCREEN_WIDTH - 256) / 2 + 2, 122, 207);

    rectfill(screen, (SCREEN_WIDTH - 256) / 2, 100,
        vol + (SCREEN_WIDTH - 256) / 2, 120, 192);

    while (running != 0) {
        blit(backg, screen, 0, 100, 0, 100, backg->w, 23);

        rectfill(screen, (SCREEN_WIDTH - 256) / 2 + 2, 102,
            vol + (SCREEN_WIDTH - 256) / 2 + 2, 122, 207);

        rectfill(screen, (SCREEN_WIDTH - 256) / 2, 100,
            vol + (SCREEN_WIDTH - 256) / 2, 120, 192);

        switch ((readkey() >> 8)) { // 207 192

        case KEY_LEFT:
        case KEY_DOWN:

            if (vol > 1) {
                vol -= 2;
            }

            keyconf.sfx_vol = vol;
            break;

        case KEY_UP:
        case KEY_RIGHT:

            if (vol < 254) {
                vol += 2;
            }

            keyconf.sfx_vol = vol;
            break;

        default:
            running = 0;
            break;
        };
    };
    icache.FreeImage(backg);
}
/**************************************************************************************/
void Config::ChangeGFXQuality()
{
    int tmp = Do_Menu("NORMAL\nHIGH", 2, keyconf.gfx_quality);

    if ((tmp > 0) && (tmp < 3)) {
        keyconf.gfx_quality = tmp;
    }
}
/**************************************************************************************/
#define MENY_POS_X 60
#define MENY_POS_Y 85
#define FONT_H 26
void Config::Start()
{
    int pos = 1;

    while (((pos = Do_Menu("CONTROLS\nGRAPHICS\nMUSIC VOL\nSFX VOL\nBACK", 5, pos)) != 0) && pos != 5) {
        switch (pos) {
        case 1:
            StartControls();
            break;

        case 2:
            ChangeGFXQuality();
            break;

        case 3:
            ChangeMusicVol();
            break;

        case 4:
            ChangeSFXVol();
            break;
        };
    };
}
/**************************************************************************************/
void Config::StartControls()
{
    ConfigureKeyboard();
    Save();
}
/**************************************************************************************/
auto Config::GetKey(const char* Label) -> int
{
    int i = 0;
    int temp = 0;
    char buffer[128];
    UrbanFont fnt(SMALL_FONT2);

    sprintf(buffer, "Please press your key for '%s'", Label);
    fnt.print_centre(buffer, 320 / 2, 90);

    clear_keybuf();
    if (temp == 0) {
        temp = readkey();
        temp >>= 8;
    }
    i = 0;
    while (keyinfo[i].num != 0) {
        if (keyinfo[i].num == temp) {

            fnt.print_centre(keyinfo[i].description, 320 / 2, 120);
            while (key[temp] != 0U) {
                ;
            }
            rest(500);
            return temp;
        }
        i++;
    }
    return -1;
}
/**************************************************************************************/