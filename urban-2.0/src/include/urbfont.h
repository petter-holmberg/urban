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
#ifndef __URBFONT_H__
#define __URBFONT_H__

#include "allegro.h"

inline constexpr auto FONT_MAGIC = 0x55464e54;

inline constexpr auto FONT_FLAG_BITMAP = 0x01;
inline constexpr auto FONT_FLAG_TRANSPARENT = 0x02;

inline constexpr auto SMALL_FONT = 0;
inline constexpr auto LARGE_FONT = 1;
inline constexpr auto SMALL_FONT2 = 2;

struct urban_font_header {
    unsigned long magic;
    unsigned long flags;
    long width;
    long height;
    long size;
    char num_chars;
    unsigned long offsets[256];
    long reserved[4];
};

struct Fontinfo {
    int width;
    int height;
    int space_x;
    int space_y;
};

struct CharInfo {
    char token;
    int x;
    int y;
};

class UrbanFont {
public:
    UrbanFont(int font = SMALL_FONT);
    UrbanFont();
    ~UrbanFont();
    UrbanFont(const UrbanFont&) = delete;
    UrbanFont& operator=(const UrbanFont&) = delete;
    int LoadFontPcx(const char* filename);
    static int UnloadFontPcx();
    int LoadFontFile(const char*) { return 0; }
    int UnloadFontFile() { return 0; }
    void SetScale(int procent);
    void print(const char* text, int x, int y, BITMAP* dest = screen);
    void print_centre(const char* text, int x, int y, BITMAP* dest = screen);
    BITMAP* print(const char* text);
    PALETTE& GetPal() { return pal; };

private:
    BITMAP* create_textbitmap(const char* text) const;
    BITMAP** Fontimages {};
    BITMAP* font_image {};
    PALETTE pal {};
    int font_nr {};
    int scale {};
    int space_x {}, space_y {}, width {}, height {};
};

#endif
