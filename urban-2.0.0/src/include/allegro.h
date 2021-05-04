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

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h>

#define LOCK_FUNCTION(x)
#define LOCK_VARIABLE(x)

typedef struct RGB {
    unsigned char r, g, b;
    unsigned char filler;
} RGB;

#define PAL_SIZE 256

typedef RGB PALLETE[PAL_SIZE];
#define PALETTE PALLETE
extern PALETTE black_palette;

/* a bitmap structure */
typedef struct BITMAP {
    int w, h;
    int stride;
    char* dat;
    char** line;

    sf::Texture texture;
    sf::Sprite* vis {};
    sf::Color pal[256];
} BITMAP;

#define FONT_SIZE 224 /* number of characters in a font */

typedef struct FONT_8x8 /* a simple 8x8 font */
{
    unsigned char dat[FONT_SIZE][8];
} FONT_8x8;

typedef struct FONT_8x16 /* a simple 8x16 font */
{
    unsigned char dat[FONT_SIZE][16];
} FONT_8x16;

typedef struct FONT_PROP /* a proportional font */
{
    BITMAP* dat[FONT_SIZE];
    void (*render)(BITMAP* bmp, BITMAP* sprite, int x, int y, int color);
} FONT_PROP;

typedef struct FONT /* can be either */
{
    int height;
    union {
        FONT_8x8* dat_8x8;
        FONT_8x16* dat_8x16;
        FONT_PROP* dat_prop;
    } dat;
} FONT;

#define get16(x) ((x[1] << 8) | x[0])
#define GFX_MODEX 0
#define GFX_TEXT 0
#define GFX_AUTODETECT 0
int vsync();
int play_memory_fli(void* fli_data, BITMAP* bmp, int loop, int (*callback)());
int play_fli(char* fli_data, BITMAP* bmp, int loop, int (*callback)());
int allegro_init();
int install_keyboard();
void keyboard_update();
void keyboard_reset();
int install_timer();
int set_gfx_mode(int mode, int w, int h, int a, int b);
int set_palette(PALETTE p);
void blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void stretch_blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int s_w, int s_h, int dest_x, int dest_y, int d_w, int d_h);
void masked_blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void destroy_bitmap(BITMAP* bmp);
void clear_keybuf();
int readkey();
BITMAP* create_bitmap(int w, int h);
void textout(BITMAP* bmp, FONT* f, char* str, int x, int y, int color);
void textout_centre(BITMAP* bmp, FONT* f, char* str, int x, int y, int color);
void clear(BITMAP* bmp);
void clear_to_color(BITMAP* bmp, int color);
void putpixel(BITMAP* bmp, int x, int y, int c);
void line(BITMAP* bmp, int x1, int y1, int x2, int y2, int color);
void rect(BITMAP* bmp, int x1, int y1, int x2, int y2, int color);
void rectfill(BITMAP* bmp, int x, int y, int x2, int y2, int c);
BITMAP* load_bitmap(char* name, PALETTE palette);
int set_color(int, RGB* rgb);
int install_int_ex(void (*proc)(), long speed);
int install_int(void (*proc)(), long speed);
void remove_int(void (*proc)());
void textprintf(BITMAP* bmp, FONT* f, int x, int y, int color, const char* format, ...) __attribute__((format(printf, 6, 7)));
void fade_in(PALLETE p, int speed);
void fade_out(int speed);
void fade_from_range(PALLETE source, PALLETE dest, int speed, int from, int to);
void rest(long time);
void get_palette(PALLETE p);
void fade_interpolate(PALLETE src, PALETTE dest, PALETTE out, int pos, int start, int end);
int keypressed();
void stretch_sprite(BITMAP* bmp, BITMAP* sprite, int x, int y, int w, int h);
void xor_mode(int on);

extern sf::RenderWindow* window;
extern BITMAP* screen;
extern FONT* font;
extern unsigned char key[128];

#define SCANCODE_TO_KEY(c) (((c) << 8) + (int)key_ascii_table[c])
#define SCANCODE_TO_CAPS(c) (((c) << 8) + (int)key_capslock_table[c])
#define SCANCODE_TO_SHIFT(c) (((c) << 8) + (int)key_shift_table[c])
#define SCANCODE_TO_CONTROL(c) (((c) << 8) + (int)key_control_table[c])
#define SCANCODE_TO_ALTGR(c) (((c) << 8) + (int)key_altgr_table[c])
#define SCANCODE_TO_ALT(c) ((c) << 8)

#define KB_SHIFT_FLAG 0x0001
#define KB_CTRL_FLAG 0x0002
#define KB_ALT_FLAG 0x0004
#define KB_LWIN_FLAG 0x0008
#define KB_RWIN_FLAG 0x0010
#define KB_MENU_FLAG 0x0020
#define KB_SCROLOCK_FLAG 0x0100
#define KB_NUMLOCK_FLAG 0x0200
#define KB_CAPSLOCK_FLAG 0x0400
#define KB_INALTSEQ_FLAG 0x0800
#define KB_ACCENT1_FLAG 0x1000
#define KB_ACCENT1_S_FLAG 0x2000
#define KB_ACCENT2_FLAG 0x4000
#define KB_ACCENT2_S_FLAG 0x8000

#define KB_NORMAL 1
#define KB_EXTENDED 2

#define KEY_ESC 1 /* keyboard scan codes  */
#define KEY_1 2
#define KEY_2 3
#define KEY_3 4
#define KEY_4 5
#define KEY_5 6
#define KEY_6 7
#define KEY_7 8
#define KEY_8 9
#define KEY_9 10
#define KEY_0 11
#define KEY_MINUS 12
#define KEY_EQUALS 13
#define KEY_BACKSPACE 14
#define KEY_TAB 15
#define KEY_Q 16
#define KEY_W 17
#define KEY_E 18
#define KEY_R 19
#define KEY_T 20
#define KEY_Y 21
#define KEY_U 22
#define KEY_I 23
#define KEY_O 24
#define KEY_P 25
#define KEY_OPENBRACE 26
#define KEY_CLOSEBRACE 27
#define KEY_ENTER 28
#define KEY_CONTROL 29
#define KEY_LCONTROL 29
#define KEY_A 30
#define KEY_S 31
#define KEY_D 32
#define KEY_F 33
#define KEY_G 34
#define KEY_H 35
#define KEY_J 36
#define KEY_K 37
#define KEY_L 38
#define KEY_COLON 39
#define KEY_QUOTE 40
#define KEY_TILDE 41
#define KEY_LSHIFT 42
#define KEY_BACKSLASH 43
#define KEY_Z 44
#define KEY_X 45
#define KEY_C 46
#define KEY_V 47
#define KEY_B 48
#define KEY_N 49
#define KEY_M 50
#define KEY_COMMA 51
#define KEY_STOP 52
#define KEY_SLASH 53
#define KEY_RSHIFT 54
#define KEY_ASTERISK 55
#define KEY_ALT 56
#define KEY_SPACE 57
#define KEY_CAPSLOCK 58
#define KEY_F1 59
#define KEY_F2 60
#define KEY_F3 61
#define KEY_F4 62
#define KEY_F5 63
#define KEY_F6 64
#define KEY_F7 65
#define KEY_F8 66
#define KEY_F9 67
#define KEY_F10 68
#define KEY_NUMLOCK 69
#define KEY_SCRLOCK 70
#define KEY_HOME 71
#define KEY_UP 72
#define KEY_PGUP 73
#define KEY_MINUS_PAD 74
#define KEY_LEFT 75
#define KEY_5_PAD 76
#define KEY_RIGHT 77
#define KEY_PLUS_PAD 78
#define KEY_END 79
#define KEY_DOWN 80
#define KEY_PGDN 81
#define KEY_INSERT 82
#define KEY_DEL 83
#define KEY_PRTSCR 84
#define KEY_F11 87
#define KEY_F12 88
#define KEY_LWIN 91
#define KEY_RWIN 92
#define KEY_MENU 93
#define KEY_PAD 100
#define KEY_RCONTROL 120
#define KEY_ALTGR 121
#define KEY_SLASH2 122
#define KEY_PAUSE 123

#define END_OF_FUNCTION(x)
#endif
