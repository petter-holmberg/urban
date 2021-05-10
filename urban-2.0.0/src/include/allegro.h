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

    Libggi gfx driver.
    Copyright (C) 1999  Jonas Borgström

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

    Contact the author my mail:

    Jonas Borgström
    Bondevägen 6
    92332 Storuman
    Sweden

    or email:

    jonas_b@bitsmart.com
 *
*****************************************************************************/
#ifndef __GFX_H__
#define __GFX_H__

#include "keyinfo.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>

struct RGB {
    unsigned char r, g, b;
    unsigned char filler;
};

inline constexpr auto PAL_SIZE = 256;

using PALETTE = std::array<RGB, PAL_SIZE>;

extern PALETTE black_palette;

/* a bitmap structure */
struct BITMAP {
    int w, h;
    int stride;
    char* dat;
    char** line;

    sf::Texture texture;
    sf::Sprite* vis {};
    sf::Color pal[PAL_SIZE];
};

inline constexpr auto FONT_SIZE = 224; /* number of characters in a font */

struct FONT_8x8 /* a simple 8x8 font */
{
    unsigned char dat[FONT_SIZE][8];
};

struct FONT_8x16 /* a simple 8x16 font */
{
    unsigned char dat[FONT_SIZE][16];
};

struct FONT_PROP /* a proportional font */
{
    BITMAP* dat[FONT_SIZE];
    void (*render)(BITMAP* bmp, BITMAP* sprite, int x, int y, int color);
};

struct FONT /* can be either */
{
    int height;
    union {
        FONT_8x8* dat_8x8;
        FONT_8x16* dat_8x16;
        FONT_PROP* dat_prop;
    } dat;
};

template <typename T>
constexpr auto get16(T x)
{
    return (x[1] << 8) | x[0];
}

inline constexpr auto GFX_MODEX = 0;
inline constexpr auto GFX_TEXT = 0;
inline constexpr auto GFX_AUTODETECT = 0;

int vsync();
int play_memory_fli(char* fli_data, BITMAP* bmp, int loop, int (*callback)());
int play_fli(char* filename, BITMAP* bmp, int loop, int (*callback)());
int allegro_init();
int install_keyboard();
void keyboard_update();
void keyboard_reset();
int set_gfx_mode(int mode, int w, int h, int a, int b);
int set_palette(const PALETTE& p);
void blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void stretch_blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int s_w, int s_h, int dest_x, int dest_y, int d_w, int d_h);
void masked_blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void destroy_bitmap(BITMAP* bmp);
void clear_keybuf();
scan_code readkey();
BITMAP* create_bitmap(int w, int h);
void textout(BITMAP* bmp, FONT* f, char* str, int x, int y, int color);
void textout_centre(BITMAP* bmp, FONT* f, char* str, int x, int y, int color);
void clear(BITMAP* bmp);
void clear_to_color(BITMAP* bmp, int color);
void putpixel(BITMAP* bmp, int x, int y, int c);
void line(BITMAP* bmp, int x1, int y1, int x2, int y2, int color);
void rect(BITMAP* bmp, int x1, int y1, int x2, int y2, int color);
void rectfill(BITMAP* bmp, int x, int y, int x2, int y2, int c);
BITMAP* load_bitmap(const char* name, PALETTE palette);
int set_color(int, RGB* rgb);
int install_int_ex(void (*proc)(), long speed);
int install_int(void (*proc)(), long speed);
void remove_int(void (*proc)());
void textprintf(BITMAP* bmp, FONT* f, int x, int y, int color, const char* format, ...) __attribute__((format(printf, 6, 7)));
void fade_in(const PALETTE& p, int speed);
void fade_out(int speed);
void fade_from_range(const PALETTE& source, const PALETTE& dest, int speed, int from, int to);
void rest(long time);
void get_palette(PALETTE& p);
void fade_interpolate(const PALETTE& src, const PALETTE& dest, PALETTE& out, int pos, int start, int end);
int keypressed();
void stretch_sprite(BITMAP* bmp, BITMAP* sprite, int x, int y, int w, int h);
void xor_mode(int on);

extern sf::RenderWindow* window;
extern BITMAP* screen;
extern FONT* font;
extern std::array<bool, 128> key;

#endif
