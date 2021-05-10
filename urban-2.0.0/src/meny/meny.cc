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
    Thomas Nyberg                                       Jonas Borgström
    Brännbergsvägen 21b                                 Bondevägen 6
    92136 Lycksele                                      92332 Storuman
    Sweden                                              Sweden

    or email:

    thomas.nyberg@usa.net                               jonas_b@bitsmart.com
*****************************************************************************/
#include <allegro.h>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>

#include <unistd.h>

void close_gfx();

#include "config.h"
#include "engine.h"
#include "game.h"
#include "highscor.h"
#include "icache.h"
#include "intro.h"
#include "urbfont.h"

inline constexpr auto MENU_MUSIC = "snd/modules/nole.xm";
inline constexpr auto CREDITS_MUSIC = "snd/modules/extrem.xm";
inline constexpr auto MAXPOS = 6;
inline constexpr auto MINPOS = 1;
inline constexpr auto FONT_H = 26;
inline constexpr auto MENY_POS_X = 60;
inline constexpr auto MENY_POS_Y = 85;

/* From credits.cc */
void showcredits();
/*****************************************************************************************/
Config* config = nullptr;
void LoadGame();
/*****************************************************************************************/
auto MenuChoice(int alternative) -> int
{
    std::unique_ptr<HighScore> hs;

    switch (alternative) {
    case 1:
        NewGame(0);

        if (is_sound_on()) {
            char buffer[1024];

            sprintf(buffer, "%s/%s", DATPATH, MENU_MUSIC);
            SOUND.PlayMusic(buffer);
        }
        break;

    case 2:
        LoadGame();

        if (is_sound_on()) {
            char buffer[1024];

            sprintf(buffer, "%s/%s", DATPATH, MENU_MUSIC);
            SOUND.PlayMusic(buffer);
        }
        clear_keybuf();
        break;

    case 3:
        hs.reset(new HighScore());
        readkey();
        break;

    case 4:
        if (is_sound_on()) {
            char buffer[1024];

            sprintf(buffer, "%s/%s", DATPATH, CREDITS_MUSIC);
            SOUND.PlayMusic(buffer);
        }
        fade_out(5);

        showcredits();

        if (is_sound_on()) {
            char buffer[1024];

            sprintf(buffer, "%s/%s", DATPATH, MENU_MUSIC);
            SOUND.PlayMusic(buffer);
        }
        break;

    case 5:
        config->Start();
        break;

    case 6:
        return 0;
    }
    return 1;
}
/*****************************************************************************************/
auto Do_Menu(const std::string& text, int num_items, int pos) -> int
{
    PALETTE pal;
    UrbanFont m(LARGE_FONT);
    m.SetScale(65);
    //	Controllers *ctrl;

    BITMAP* choice = icache.GetImage("pekare.pcx", pal);
    BITMAP* backg = icache.GetImage("ibild.pcx", pal);
    set_palette(pal);
    BITMAP* textbmp = m.print(text.c_str());
    int text_x = (SCREEN_WIDTH - textbmp->w) / 2;
    int text_y = (SCREEN_HEIGHT - textbmp->h) / 2;
    int looping = 1;

    blit(backg, screen, 0, 0, 0, 0, backg->w, backg->h);
    masked_blit(textbmp, screen, 0, 0, text_x, text_y, textbmp->w, textbmp->h);

    masked_blit(choice, screen, 0, 0, text_x - 10 - choice->w, text_y + FONT_H * (pos - 1), choice->w, choice->h);
    rest(1000);

    while (looping != 0) {
        clear_keybuf();

        blit(backg, screen, text_x - 10 - choice->w, text_y - 20, text_x - 10 - choice->w, text_y - 20, choice->w, choice->h * 10);
        masked_blit(choice, screen, 0, 0, text_x - 10 - choice->w, text_y + FONT_H * (pos - 1), choice->w, choice->h);

        switch (readkey()) {

        case scan_code::KEY_ENTER:
        case scan_code::KEY_SPACE:
            looping = 0;
            break;

        case scan_code::KEY_ESC:
            pos = 0;
            looping = 0;
            break;

        case scan_code::KEY_UP:
            if (pos > 1) {
                pos--;
            } else {
                pos = num_items;
            }
            break;

        case scan_code::KEY_DOWN:
            if (pos < num_items) {
                pos++;
            } else {
                pos = 1;
            }
            break;
        }
    }

    destroy_bitmap(textbmp);

    clear_keybuf();

    return pos;
}
/*****************************************************************************************/
void LoadGame()
{
    char filename[512];
    FILE* fs = nullptr;
    int slot = 0;

    sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));
    if ((fs = fopen(filename, "rb")) == nullptr) {
        for (auto& SavedGame : SavedGames) {
            SavedGame.level = 0;
            memcpy(&SavedGame.dat, &DefaultPData, sizeof(struct PlayerData));
        }
        /* Create dir */
        sprintf(filename, "%s/.urban", getenv("HOME"));
        std::filesystem::create_directory(std::filesystem::path { filename });

        sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));
        if ((fs = fopen(filename, "wb")) != nullptr) {

            fwrite(SavedGames, 1, 5 * sizeof(struct SaveGameData), fs);
            fclose(fs);
        }
    } else {
        auto err = fread(SavedGames, 1, 5 * sizeof(struct SaveGameData), fs);
        fclose(fs);
    }

    std::string buffer;

    for (auto& SavedGame : SavedGames) {
        if (SavedGame.level != 0) {
            buffer += std::string(SavedGame.name) + "\n";
        } else {
            buffer = "EMPTY\n";
        }
    }

    if ((slot = Do_Menu(buffer, 5)) > 0) {
        if (SavedGames[slot - 1].level != 0) {
            NewGame(slot);
        }
    }
}

void UrbanSetGFXMode()
{
    set_gfx_mode(GFX_MODEX, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    set_palette(black_palette);
}

extern char* special_thanks;

/*****************************************************************************************/
auto main(int argc, char** argv) -> int
{
    //	int looping = 1;
    int pos = 1;
    int c = 0;
    const char* meny_text = "START GAME\nLOAD GAME\nHIGHSCORE\nCREDITS\nCONFIG\nQUIT";
    config = new Config;

    allegro_init();
    install_keyboard();
    keyboard_reset();

    engine = new Engine;

    opterr = 0;
    set_spec_level_off();
    set_vert_retrace_off();
    /*
Möjliga växlar:
-f<mapname> Namn på karta att spela
-x Slå av ljudeffekterna
-v Slå av vertical retrace
-b Slå av bakgrunderna
-s Slå av musiken
-i Visa inte intron
-r Slå på vertical Retrace
-h visa hjälp
*/
    while ((c = getopt(argc, argv, "hrisbvxf:")) != -1) {
        switch (c) {
        case 'h':
            printf("Urban v1.2\n");
            printf("Options:\n");
            printf("-f<mapname>   Play a specific map\n");
            printf("-x            Disable sound effects\n");
            printf("-s            Disable music\n");
            printf("-i            Don't show intro\n");
            printf("-h            Show this\n");
            return 0;
            break;
        case 'f':
            set_spec_level_on();
            UrbanSetGFXMode();
            ENGINE.play_level(optarg);
            return 0;
            break;
        case 's':
            set_sound_off();
            break;
        case 'b':
            set_background_off();
            break;
        case 'v':
            set_vert_retrace_off();
            break;
        case 'r':
            set_vert_retrace_on();
            break;
        case 'x':
            set_soundfx_off();
            break;
        case 'i':
            set_intro_off();
            break;
        }
    }

    UrbanSetGFXMode();

    if (is_intro_on()) {
        // Run intro
        auto* intro = new Intro;

        intro->RunIntro();
        delete intro;
    }
    clear_keybuf();

    if (is_sound_on()) {
        char buffer[1024];

        sprintf(buffer, "%s/%s", DATPATH, MENU_MUSIC);
        SOUND.PlayMusic(buffer);
    }

    while (((pos = Do_Menu(meny_text, 6, pos)) != 0) && pos != 6) {
        MenuChoice(pos);
    }

    //        fade_out(6);
    {
        UrbanFont L(LARGE_FONT);
        UrbanFont S(SMALL_FONT);
        /*	clear_to_color(screen, 200);*/
        PALETTE pal;
        BITMAP* backg = icache.GetImage("ibild.pcx", pal);
        blit(backg, screen, 0, 0, 0, 0, backg->w, backg->h);

        rest(500);

        L.print_centre("THE URBAN", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, screen);
        L.print_centre("HOMEPAGE", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, screen);
        S.print_centre("urban.home.dhs.org", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 40, screen);
        int i = 600;

        while ((keypressed() == 0) && (i != 0)) {
            rest(100), i--;
        }
    }
    fade_out(6);

    close_gfx();
    return 0;
}
