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
#include "highscor.h"
#include "allegro.h"
#include "engine.h"
#include "urbfont.h"
#include <algorithm>
#include <filesystem>
#include <string>
/***************************************************************************/

/***************************************************************************/
HighScore::HighScore()
{
    char buffer[256];
    Open();
    UrbanFont fnt(SMALL_FONT2);
    BITMAP* textbmp = create_bitmap(220, 210);
    clear(textbmp);
    int x = 0;
    int y = 0;

    // Ritar ett rutnär
    for (x = 0; x < 320; x += 2) {
        for (y = 0; y < 240; y += 2) {

            putpixel(screen, x, y, 0);
            putpixel(screen, x + 1, y + 1, 0);
        }
    }
    rectfill(screen, 45, 25, 320 - 45, 233, 27);
    rectfill(screen, 50, 30, 320 - 50, 228, 25);
    rectfill(screen, 50, 30, 320 - 50, 63, 23);

    fnt.print("***** HALL OF FAME *****", 60, 13, textbmp);
    fnt.print("  Name          Score Level", 0, 30, textbmp);

    for (int i = 0; i < NUM_HIGHSCORES; i++) {

        sprintf(buffer, "%2d", i + 1);
        fnt.print(buffer, 15, 50 + i * 16, textbmp);

        if (highscore[i].Level != 0) {
            sprintf(buffer, " %-10s%6d   %1d:%1d", highscore[i].Name.data(),
                highscore[i].Score, ((highscore[i].Level - 1) / 3) + 1, ((highscore[i].Level - 1) % 3) + 1);
        } else {
            sprintf(buffer, " %-10s%6d   0:0", "Empty", 0);
        }

        fnt.print(buffer, 31, 50 + i * 16, textbmp);
    }
    masked_blit(textbmp, screen, 0, 0, 50, 20, 220, 210);
    destroy_bitmap(textbmp);
}
/***************************************************************************/
auto HighScore::GetName() -> std::string
{
    static std::string Name;
    PALETTE pal;
    BITMAP* backg = icache.GetImage("ibild.pcx", pal);
    BITMAP* textbmp = create_bitmap(220, 55);
    UrbanFont fnt(SMALL_FONT2);

    if (Name.empty()) {
        Name = "Unknown";
    }

    auto pos = Name.size();

    set_palette(pal);
    blit(backg, screen, 0, 0, 0, 0, 320, 240);

    clear_keybuf();

    do {
        rectfill(screen, 45, 55, 320 - 45, 240 - 120, 27);
        rectfill(screen, 50, 60, 320 - 50, 240 - 125, 25);
        rectfill(screen, 50, 60, 320 - 50, 240 - 165, 23);

        clear(textbmp);

        fnt.print_centre("HighScore", 110, 8, textbmp);
        fnt.print_centre("Please Enter Your Name:", 110, 24, textbmp);
        fnt.print_centre(Name.c_str(), 110, 43, textbmp);

        masked_blit(textbmp, screen, 0, 0, 50, 60, 220, 55);

        auto letter = readtext();

        if (letter == 13) { // ENTER
            Name[pos] = 0;
            destroy_bitmap(textbmp);
            return Name;
        }
        if (letter == 27) { // ESC
            destroy_bitmap(textbmp);
            Name = "Unknown";
            return Name;
        }
        if (letter == 59) { // BACKSPACE
            if (pos == 0) {
                continue;
            }
            pos--;
            Name[pos] = 0;
        } else if (((letter >= 'A') && (letter <= 'z')) || (letter == ' ')) {
            Name[pos++] = letter;
            Name[pos] = 0;
        }
    } while (1);

    destroy_bitmap(textbmp);
    Name = "Unknown";
    return Name;
}
/***************************************************************************/
HighScore::HighScore(int score, int level)
{
    Open();

    for (int i = 0; i < NUM_HIGHSCORES; i++) {

        if (score > highscore[i].Score) {
            std::move(std::begin(highscore) + i + 1, std::begin(highscore) + i + 1 + NUM_HIGHSCORES, std::begin(highscore) + i);
            auto name { GetName() };
            std::copy(name.c_str(), name.c_str() + name.size(), std::begin(highscore[i].Name));
            highscore[i].Score = score;
            highscore[i].Level = level;
            break;
        }
    }

    Save();
}
/***************************************************************************/
void HighScore::Open()
{
    // Reset Score
    highscore.fill({});
    char filename[1024];

    sprintf(filename, "%s/.urban/hs.dat", getenv("HOME"));

    if ((fd = fopen(filename, "rb")) == nullptr) {
        return;
    }

    if (fread(highscore.data(), sizeof(Score_t), NUM_HIGHSCORES, fd) != NUM_HIGHSCORES) {
        throw std::runtime_error("hs.dat fread failed");
    }

    fclose(fd);
}
/***************************************************************************/
void HighScore::Save()
{
    char filename[1024];

    sprintf(filename, "%s/.urban", getenv("HOME"));
    std::filesystem::create_directory(std::filesystem::path { filename });

    sprintf(filename, "%s/.urban/hs.dat", getenv("HOME"));

    if ((fd = fopen(filename, "wb")) == nullptr) {
        return;
    }

    fwrite(highscore.data(), sizeof(Score_t), NUM_HIGHSCORES, fd);

    fclose(fd);
}
/***************************************************************************/
