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
#include "game.h"
#include "allegro.h"
#include "ctrls.h"
#include "engine.h"
#include "highscor.h"
#include "icache.h"
#include "outtro.h"
#include "urbfont.h"
#include <algorithm>
#include <filesystem>
#include <string>

/**************************************************************************/
// Global ImageCache
ImageCache icache;
// Credits
void showcredits();
/**************************************************************************/
Engine* engine = nullptr;
uint32_t flags_ = 0xffffffff;

extern char lock_frame_count_to_60hz;

SaveGameData SavedGames[5];

const char* maps[] = { "gamemaps/level11.map",
    "gamemaps/level12.map",
    "gamemaps/level13.map",
    "gamemaps/level21.map",
    "gamemaps/level22.map",
    "gamemaps/level23.map",
    "gamemaps/level31.map",
    "gamemaps/level32.map",
    "gamemaps/level33.map",
    "gamemaps/level41.map",
    "gamemaps/level42.map",
    "gamemaps/level43.map",
    "gamemaps/level51.map",
    "gamemaps/level52.map",
    "gamemaps/level53.map",
    "gamemaps/level61.map",
    "gamemaps/level62.map",
    "gamemaps/level63.map", nullptr };

const char* extra_maps[] = { "",
    "",
    "",
    "",
    "",
    "", nullptr };

inline constexpr auto NUM_LEVELS = 18;

int MaxLevelNum = 1;

char* demo_filename;

PlayerData DefaultPData = {
    3,
    { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    300, 0
};

auto GetLevelName(std::string text) -> int;
/**************************************************************************/
auto SaveGame(std::string name) -> int
{
    int pos = 0;

    pos = Do_Menu("CONTINUE\nSAVE GAME", 2);

    if (pos != 2) {
        return 0;
    }

    std::string buffer;
    for (auto& SavedGame : SavedGames) {
        if (SavedGame.level != 0) {
            buffer += std::string { SavedGame.name } + "\n";
        } else {
            buffer += "EMPTY\n";
        }
    }

    if ((pos = Do_Menu(buffer.c_str(), 5)) != 0) {
        if (SavedGames[pos - 1].level != 0) {
            name = SavedGames[pos - 1].name;
        }

        if (GetLevelName(name) != 0) {
            return pos;
        }
        {
            return 0;
        }
    } else {
        return 0;
    }
}

/**************************************************************************/
void display_level_info(int level)
{
    char message[256];
    PALETTE Pal;
    UrbanFont fnt(LARGE_FONT);

    BITMAP* bmp = icache.GetImage("ibild.pcx", Pal);
    blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);
    if (level == -1) {
        sprintf(message, "GAME OVER");
    } else if (level > NUM_LEVELS) {

        {
            // Run Outtro
            auto* outtro = new Outtro;

            Outtro::RunOuttro();
            delete outtro;

            // Show credits
            showcredits();
        }
        /*	        fnt.SetScale(65);
		sprintf(message, "MISSION");
		fnt.print_centre(message, 320 / 2, 190 / 2);
		sprintf(message, "COMPLETED");*/
    } else {
        return;
    }

    fnt.print_centre(message, 320 / 2, 240 / 2);
    fade_in(Pal, 2);
    rest(2000);
    clear_keybuf();
}
/**************************************************************************/
auto GetLevelName(std::string text) -> int
{
    char message[50];
    int pos = 1;
    UrbanFont fnt(SMALL_FONT2);

    if (text.empty()) {
        text = "MYLEVEL";
    }

    // Ritar ett rutnär
    for (int x = 0; x < 320; x += 2) {
        for (int y = 0; y < 240; y += 2) {

            putpixel(screen, x, y, 0);
            putpixel(screen, x + 1, y + 1, 0);
        }
    }

    pos = text.size();

    BITMAP* textbmp = create_bitmap(220, 55);

    clear_keybuf();

    sprintf(message, "Slot Name");

    do {
        rectfill(screen, 45, 55, 320 - 45, 240 - 120, 27);
        rectfill(screen, 50, 60, 320 - 50, 240 - 125, 25);

        clear(textbmp);
        fnt.print_centre(message, 110, 20, textbmp);
        fnt.print_centre(text.c_str(), 110, 40, textbmp);

        masked_blit(textbmp, screen, 0, 0, 50, 60, 220, 55);

        auto key = readkey();

        if (key == scan_code::KEY_ENTER) {

            text[pos] = 0;
            /*                        strupr(text);*/
            return 1;
        }
        if (key == scan_code::KEY_ESC) {

            destroy_bitmap(textbmp);
            return 0;
        }
        if (key == scan_code::KEY_BACKSPACE) {
            if (pos == 0) {
                continue;
            }
            pos--;
            text[pos] = 0;
        }
        if (key == scan_code::KEY_SPACE) {
            if (pos > 9) {
                continue;
            }
            text[pos++] = ' ';
        } else if (pos < 9) {

            /*
            if (!((key & 0xff) < 'A' || (key & 0xff) > 'z') || ((key & 0xff) == ' ')) {

                text[pos++] = toupper((key & 0xff));
                text[pos] = 0;
            }
*/
        }
    } while (1);

    destroy_bitmap(textbmp);
    return 0;
}
/**************************************************************************/
auto NewGame(int slot) -> int
{
    int quit = 0;
    std::unique_ptr<HighScore> hs;
    const char* map_name = nullptr;
    char name[80];
    char filename[1024];
    PlayerData pdat {};
    int level = 1;

    FILE* fs = nullptr;

    ENGINE.NewGame();

    display_level_info(level);

    sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));

    if ((fs = fopen(filename, "rb")) == nullptr) {

        for (auto& SavedGame : SavedGames) {
            SavedGame.level = 0;
            SavedGame.name[0] = 0;
            SavedGame.dat = DefaultPData;
        }
        /* Create dir */
        sprintf(filename, "%s/.urban", getenv("HOME"));
        std::filesystem::create_directory(std::filesystem::path { filename });

        sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));
        if ((fs = fopen(filename, "wb")) != nullptr) {
            fwrite(SavedGames, 1, 5 * sizeof(SaveGameData), fs);
        }
    } else {
        auto err = fread(SavedGames, 1, 5 * sizeof(SaveGameData), fs);
    }

    if (fs != nullptr) {
        fclose(fs);
    }

    if (slot == 0 || SavedGames[slot - 1].level == 0) {
        pdat = DefaultPData;
    } else {
        pdat = SavedGames[slot - 1].dat;
        level = SavedGames[slot - 1].level;
    }

    map_name = maps[level - 1];

    while (quit == 0) {
        switch (ENGINE.play_level(map_name, &pdat)) {
        case -1: // Game over
            SOUND.PlaySFX("samples/haha.wav");

            quit = 1;
            display_level_info(-1);
            hs.reset(new HighScore(ENGINE.score.GetScore(), level));
            break;
        case 0: // ESC pressed
            fade_out(5);
            quit = 1;
            break;
        case 2: //kör extrabana
            fade_out(5);
            map_name = extra_maps[level - 1];
            if (*map_name != 0) {
                break;
            }
        case 1: // Level Clear
            level++;
            fade_out(5);
            display_level_info(level);
            if (level > NUM_LEVELS) {
                quit = 1;
                level--;
                hs.reset(new HighScore(ENGINE.score.GetScore(), level));
            } else {
                slot = SaveGame(name);
                if (slot != 0) {
                    SavedGames[slot - 1].dat = pdat;
                    SavedGames[slot - 1].level = level;
                    std::copy(std::begin(name), std::end(name), std::begin(SavedGames[slot - 1].name));

                    /* Create dir */
                    sprintf(filename, "%s/.urban", getenv("HOME"));
                    std::filesystem::create_directory(std::filesystem::path { filename });

                    sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));
                    if ((fs = fopen(filename, "wb")) != nullptr) {
                        auto err = fwrite(SavedGames, 1, 5 * sizeof(struct SaveGameData), fs);
                        fclose(fs);
                    }
                }
            }

            map_name = maps[level - 1];
            break;
        };
    };
    return 1;
}
