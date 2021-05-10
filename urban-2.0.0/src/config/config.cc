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
#include "config.h"
#include "allegro.h"
#include "ctrls.h"
#include "engine.h"
#include "game.h"
#include "keyinfo.h"
#include "urbfont.h"
#include <filesystem>
#include <string>
/**************************************************************************************/
void Config::Load()
{
    std::string filename = ::getenv("HOME");
    filename += "/.urban/ctrl.dat";

    FILE* fd = nullptr;

    if ((fd = fopen(filename.c_str(), "rb")) == nullptr) {
        return;
    }

    auto err = fread(&keyconf, sizeof(KeyConf), 1, fd);

    fclose(fd);
}
/**************************************************************************************/
void Config::Save()
{
    /* Create dir */
    std::string filename = ::getenv("HOME");
    filename += "/.urban";

    std::filesystem::create_directory(std::filesystem::path { filename });

    filename += "/ctrl.dat";
    FILE* fd = nullptr;
    if ((fd = fopen(filename.c_str(), "wb")) == nullptr) {
        return;
    }

    fwrite(&keyconf, sizeof(KeyConf), 1, fd);

    fclose(fd);
}
/**************************************************************************************/
Config::Config()
{
    Load();
}
/**************************************************************************************/
void Config::ConfigureKeyboard()
{
    PALETTE Pal;
    scan_code temp {};
    BITMAP* bmp = icache.GetImage("ibild.pcx", Pal);
    set_palette(Pal);

    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("UP")) != scan_code::INVALID) {
        keyconf.key_up = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("DOWN")) != scan_code::INVALID) {
        keyconf.key_down = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("LEFT")) != scan_code::INVALID) {
        keyconf.key_left = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("RIGHT")) != scan_code::INVALID) {
        keyconf.key_right = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("FIRE")) != scan_code::INVALID) {
        keyconf.key_fire = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("JUMP")) != scan_code::INVALID) {
        keyconf.key_jump = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("Prev Weapon")) != scan_code::INVALID) {
        keyconf.key_prevweapon = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    if ((temp = GetKey("Next Weapon")) != scan_code::INVALID) {
        keyconf.key_nextweapon = temp;
    } else {
        return;
    }
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

    ctrl_type = CONTROLLER_KEYBOARD;
}
/**************************************************************************************/
void Config::ChangeMusicVol()
{
    PALETTE pal;
    BITMAP* backg = icache.GetImage("ibild.pcx", pal);
    int running = 1;
    int vol = music_vol;

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

        switch (readkey()) { // 207 192

        case scan_code::KEY_LEFT:
        case scan_code::KEY_DOWN:

            if (vol > 1) {
                vol -= 2;
            }

            SOUND.SetMusicVolume(vol);
            music_vol = vol;
            break;

        case scan_code::KEY_UP:
        case scan_code::KEY_RIGHT:

            if (vol < 254) {
                vol += 2;
            }

            SOUND.SetMusicVolume(vol);
            music_vol = vol;
            break;

        default:
            running = 0;
            Save();
            break;
        };
    };
}
/**************************************************************************************/
void Config::ChangeSFXVol()
{
    PALETTE pal;
    BITMAP* backg = icache.GetImage("ibild.pcx", pal);
    int running = 1;
    int vol = sfx_vol;

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

        switch (readkey()) { // 207 192

        case scan_code::KEY_LEFT:
        case scan_code::KEY_DOWN:

            if (vol > 1) {
                vol -= 2;
            }

            sfx_vol = vol;
            break;

        case scan_code::KEY_UP:
        case scan_code::KEY_RIGHT:

            if (vol < 254) {
                vol += 2;
            }

            sfx_vol = vol;
            break;

        default:
            running = 0;
            break;
        };
    };
}
/**************************************************************************************/
void Config::ChangeGFXQuality()
{
    int tmp = Do_Menu("NORMAL\nHIGH", 2, gfx_quality);

    if ((tmp > 0) && (tmp < 3)) {
        gfx_quality = tmp;
    }
}
/**************************************************************************************/
inline constexpr auto MENY_POS_X = 60;
inline constexpr auto MENY_POS_Y = 85;
inline constexpr auto FONT_H = 26;
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
auto Config::GetKey(const char* Label) -> scan_code
{
    UrbanFont fnt(SMALL_FONT2);

    std::string buffer{"Please press your key for '"};
    buffer += Label + std::string("'");
    fnt.print_centre(buffer.c_str(), 320 / 2, 90);

    clear_keybuf();
    auto temp = readkey();
    size_t i = 0;
    while (keyinfo[i].num != scan_code::NONE) {
        if (keyinfo[i].num == temp) {
            fnt.print_centre(static_cast<const char*>(keyinfo[i].description), 320 / 2, 120);
            rest(500);
            return temp;
        }
        i++;
    }
    return scan_code::INVALID;
}
/**************************************************************************************/
