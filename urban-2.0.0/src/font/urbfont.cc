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
#include "urbfont.h"
#include "engine.h"
#include "icache.h"
#include <allegro.h>

struct Fontinfo UrbFontInfo[] = {
    { 9, 15, 8, 15 },
    { 32, 32, 34, 41 }
};

struct CharInfo* charinfo[2];

struct CharInfo charinfo_small[] = {
    { 'A', 0, 0 },
    { 'B', 1, 0 },
    { 'C', 2, 0 },
    { 'D', 3, 0 },
    { 'E', 4, 0 },
    { 'F', 5, 0 },
    { 'G', 6, 0 },
    { 'H', 7, 0 },
    { 'I', 8, 0 },
    { 'J', 9, 0 },
    { 'K', 10, 0 },
    { 'L', 11, 0 },
    { 'M', 12, 0 },
    { 'N', 13, 0 },
    { 'O', 14, 0 },
    { 'P', 15, 0 },
    { 'Q', 16, 0 },
    { 'R', 17, 0 },
    { 'S', 0, 1 },
    { 'T', 1, 1 },
    { 'U', 2, 1 },
    { 'V', 3, 1 },
    { 'W', 4, 1 },
    { 'X', 5, 1 },
    { 'Y', 6, 1 },
    { 'Z', 7, 1 },
    { 'Å', 8, 1 },
    { 'Ä', 9, 1 },
    { 'Ö', 10, 1 },
    { 'a', 0, 2 },
    { 'b', 1, 2 },
    { 'c', 2, 2 },
    { 'd', 3, 2 },
    { 'e', 4, 2 },
    { 'f', 5, 2 },
    { 'g', 6, 2 },
    { 'h', 7, 2 },
    { 'i', 8, 2 },
    { 'j', 9, 2 },
    { 'k', 10, 2 },
    { 'l', 11, 2 },
    { 'm', 12, 2 },
    { 'n', 13, 2 },
    { 'o', 14, 2 },
    { 'p', 15, 2 },
    { 'q', 16, 2 },
    { 'r', 17, 2 },
    { 's', 0, 3 },
    { 't', 1, 3 },
    { 'u', 2, 3 },
    { 'v', 3, 3 },
    { 'w', 4, 3 },
    { 'x', 5, 3 },
    { 'y', 6, 3 },
    { 'z', 7, 3 },
    { 'å', 8, 3 },
    { 'ä', 9, 3 },
    { 'ö', 10, 3 },
    { '1', 0, 4 },
    { '2', 1, 4 },
    { '3', 2, 4 },
    { '4', 3, 4 },
    { '5', 4, 4 },
    { '6', 5, 4 },
    { '7', 6, 4 },
    { '8', 7, 4 },
    { '9', 8, 4 },
    { '0', 9, 4 },
    { ',', 10, 4 },
    { '.', 11, 4 },
    { ':', 12, 4 },
    { '%', 13, 4 },
    { '!', 14, 4 },
    { '?', 15, 4 },
    { '(', 16, 4 },
    { ')', 17, 4 },
    { '\'', 0, 5 },
    { '\0', 0, 0 }
};

struct CharInfo charinfo_large[] = {
    { 'A', 0, 0 },
    { 'B', 1, 0 },
    { 'C', 2, 0 },
    { 'D', 3, 0 },
    { 'E', 4, 0 },
    { 'F', 5, 0 },
    { 'G', 6, 0 },
    { 'H', 7, 0 },
    { 'I', 8, 0 },
    { 'J', 9, 0 },
    { 'K', 0, 1 },
    { 'L', 1, 1 },
    { 'M', 2, 1 },
    { 'N', 3, 1 },
    { 'O', 4, 1 },
    { 'P', 5, 1 },
    { 'Q', 6, 1 },
    { 'R', 7, 1 },
    { 'S', 8, 1 },
    { 'T', 9, 1 },
    { 'U', 0, 2 },
    { 'V', 1, 2 },
    { 'W', 2, 2 },
    { 'X', 3, 2 },
    { 'Y', 4, 2 },
    { 'Z', 5, 2 },
    { ':', 6, 2 },
    { '-', 7, 2 },
    { '\0', 0, 0 }
};

UrbanFont::UrbanFont(int font)
{
    font_image = nullptr;
    font_nr = font;

    charinfo[0] = charinfo_small;
    charinfo[1] = charinfo_large;

    if (font_nr == SMALL_FONT) {

        LoadFontPcx("dafont2.pcx");

    } else if (font_nr == LARGE_FONT) {

        LoadFontPcx("rsifont.pcx");

    } else if (font_nr == SMALL_FONT2) {

        LoadFontPcx("dafont3.pcx");
        font_nr = SMALL_FONT;
    }
    width = UrbFontInfo[font_nr].width;
    height = UrbFontInfo[font_nr].height;
    space_x = UrbFontInfo[font_nr].space_x;
    space_y = UrbFontInfo[font_nr].space_y;

    scale = 100;
}
void UrbanFont::SetScale(int proc)
{
    scale = proc;

    space_x = (scale * UrbFontInfo[font_nr].space_x) / 100;
    space_y = (scale * UrbFontInfo[font_nr].space_y) / 100;
    width = (scale * UrbFontInfo[font_nr].width) / 100;
    height = (scale * UrbFontInfo[font_nr].height) / 100;
}

UrbanFont::UrbanFont()
{
    font_image = nullptr;
}

UrbanFont::~UrbanFont()
{
    UnloadFontPcx();
}

auto UrbanFont::LoadFontPcx(const char* filename) -> int
{
    font_image = icache.GetImage(filename, pal);

    return 0;
}

auto UrbanFont::UnloadFontPcx() -> int
{
    return 0;
}

void UrbanFont::print(const char* text, int x, int y, BITMAP* dest)
{
    if (font_image == nullptr) {
        return;
    }

    BITMAP* bmp = print(text);

    masked_blit(bmp, dest, 0, 0, x, y, bmp->w, bmp->h);

    destroy_bitmap(bmp);
}

void UrbanFont::print_centre(const char* text, int x, int y, BITMAP* dest)
{
    if (font_image == nullptr) {
        return;
    }

    BITMAP* bmp = print(text);

    masked_blit(bmp, dest, 0, 0, x - bmp->w / 2, y - bmp->h / 2, bmp->w, bmp->h);

    destroy_bitmap(bmp);
}

auto UrbanFont::print(const char* text) -> BITMAP*
{
    int i = 0;
    int dest_x = 0;
    int dest_y = 0;

    if (font_image == nullptr) {
        return nullptr;
    }

    BITMAP* bmp = create_textbitmap(text);
    /*
        if(w == 0) {
        	space_x = UrbFontInfo[font_nr].space_x;
        	space_y = UrbFontInfo[font_nr].space_y;
        } else {
        	space_x = w + 2;
        	space_y = h + 3;
        }*/

    clear(bmp);

    while (*text != 0) {
        i = 0;
        switch (*text) {
        case '\n':
            dest_y += space_y;
            //                        	dest_y += UrbFontInfo[font_nr].space_y;
            dest_x = 0;
            text++;
            break;

        case ' ':
            dest_x += space_x;
            //                        	dest_x += UrbFontInfo[font_nr].space_x;
            text++;
            break;

        default:
            while ((charinfo[font_nr][i].token != 0) && charinfo[font_nr][i].token != *text) {
                i++;
            }

            text++;

            if (charinfo[font_nr][i].token == 0) {
                continue;
            }

            if (scale == 100) {
                blit(font_image, bmp, charinfo[font_nr][i].x * UrbFontInfo[font_nr].width,
                    charinfo[font_nr][i].y * UrbFontInfo[font_nr].height, dest_x, dest_y,
                    width, height);
            } else {
                stretch_blit(font_image, bmp, charinfo[font_nr][i].x * UrbFontInfo[font_nr].width,
                    charinfo[font_nr][i].y * UrbFontInfo[font_nr].height, UrbFontInfo[font_nr].width, UrbFontInfo[font_nr].height,
                    dest_x, dest_y, width, height);
            }

            dest_x += space_x;
            /*		              	blit(font_image, bmp, charinfo[font_nr][i].x * UrbFontInfo[font_nr].width,
		              		charinfo[font_nr][i].y * UrbFontInfo[font_nr].height, dest_x, dest_y,
		              		UrbFontInfo[font_nr].width, UrbFontInfo[font_nr].height);
                        	dest_x += UrbFontInfo[font_nr].space_x;*/
        }
    }
    return bmp;
}

auto UrbanFont::create_textbitmap(const char* text) const -> BITMAP*
{
    //        int w = 0, h = UrbFontInfo[font_nr].space_y;
    int w = 0;
    int h = space_y;
    int max_w = 0;

    while (*text != 0) {
        if (*text++ == '\n') {
            //                	h += UrbFontInfo[font_nr].space_y;
            h += space_y;
            w = 0;
        } else {
            //                	w += UrbFontInfo[font_nr].space_x;
            w += space_x;
            if (w > max_w) {
                max_w = w;
            }
        }
    }
    return create_bitmap(max_w, h);
}
