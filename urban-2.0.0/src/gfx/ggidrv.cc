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
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <allegro.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/**************************************************************************/
/* BITMAP *create_bitmap(int w, int h); */
/**************************************************************************/
sf::RenderWindow* window;
BITMAP* screen;
FONT* font;
PALETTE black_palette;
unsigned char key[128];
/**************************************************************************/
static int screen_offset_x, screen_offset_y;
static int num_keypressed;
static int have_directbuffer;
/**************************************************************************/
void textprintf(BITMAP* bmp, FONT* f, int x, int y, int color, const char* format, ...)
{
}
/**************************************************************************/
void rest(long t)
{
    usleep(1000 * t);
}
/**************************************************************************/
void keyboard_update()
{
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased: {
            unsigned char is_pressed = (event.type == sf::Event::KeyPressed ? 1 : 0);
            switch (event.key.code) {
            case sf::Keyboard::Key::A:
                key[KEY_A] = is_pressed;
                break;
            case sf::Keyboard::Key::B:
                key[KEY_B] = is_pressed;
                break;
            case sf::Keyboard::Key::C:
                key[KEY_C] = is_pressed;
                break;
            case sf::Keyboard::Key::D:
                key[KEY_D] = is_pressed;
                break;
            case sf::Keyboard::Key::E:
                key[KEY_E] = is_pressed;
                break;
            case sf::Keyboard::Key::F:
                key[KEY_F] = is_pressed;
                break;
            case sf::Keyboard::Key::G:
                key[KEY_G] = is_pressed;
                break;
            case sf::Keyboard::Key::H:
                key[KEY_H] = is_pressed;
                break;
            case sf::Keyboard::Key::I:
                key[KEY_I] = is_pressed;
                break;
            case sf::Keyboard::Key::J:
                key[KEY_J] = is_pressed;
                break;
            case sf::Keyboard::Key::K:
                key[KEY_K] = is_pressed;
                break;
            case sf::Keyboard::Key::L:
                key[KEY_L] = is_pressed;
                break;
            case sf::Keyboard::Key::M:
                key[KEY_M] = is_pressed;
                break;
            case sf::Keyboard::Key::N:
                key[KEY_N] = is_pressed;
                break;
            case sf::Keyboard::Key::O:
                key[KEY_O] = is_pressed;
                break;
            case sf::Keyboard::Key::P:
                key[KEY_P] = is_pressed;
                break;
            case sf::Keyboard::Key::Q:
                key[KEY_Q] = is_pressed;
                break;
            case sf::Keyboard::Key::R:
                key[KEY_R] = is_pressed;
                break;
            case sf::Keyboard::Key::S:
                key[KEY_S] = is_pressed;
                break;
            case sf::Keyboard::Key::T:
                key[KEY_T] = is_pressed;
                break;
            case sf::Keyboard::Key::U:
                key[KEY_U] = is_pressed;
                break;
            case sf::Keyboard::Key::V:
                key[KEY_V] = is_pressed;
                break;
            case sf::Keyboard::Key::W:
                key[KEY_W] = is_pressed;
                break;
            case sf::Keyboard::Key::X:
                key[KEY_X] = is_pressed;
                break;
            case sf::Keyboard::Key::Y:
                key[KEY_Y] = is_pressed;
                break;
            case sf::Keyboard::Key::Z:
                key[KEY_Z] = is_pressed;
                break;
            case sf::Keyboard::Key::LControl:
                key[KEY_LCONTROL] = is_pressed;
                break;
            case sf::Keyboard::Key::RControl:
                key[KEY_RCONTROL] = is_pressed;
                break;
            case sf::Keyboard::Key::LShift:
                key[KEY_LSHIFT] = is_pressed;
                break;
            case sf::Keyboard::Key::RShift:
                key[KEY_RSHIFT] = is_pressed;
                break;
            case sf::Keyboard::Key::LAlt:
                key[KEY_ALT] = is_pressed;
                break;
            case sf::Keyboard::Key::RAlt:
                key[KEY_ALTGR] = 1;
                break;
            case sf::Keyboard::Key::F1:
                key[KEY_F1] = is_pressed;
                break;
            case sf::Keyboard::Key::F2:
                key[KEY_F2] = is_pressed;
                break;
            case sf::Keyboard::Key::F3:
                key[KEY_F3] = is_pressed;
                break;
            case sf::Keyboard::Key::F4:
                key[KEY_F4] = is_pressed;
                break;
            case sf::Keyboard::Key::F5:
                key[KEY_F5] = is_pressed;
                break;
            case sf::Keyboard::Key::F6:
                key[KEY_F6] = is_pressed;
                break;
            case sf::Keyboard::Key::F7:
                key[KEY_F7] = is_pressed;
                break;
            case sf::Keyboard::Key::F8:
                key[KEY_F8] = is_pressed;
                break;
            case sf::Keyboard::Key::F9:
                key[KEY_F9] = is_pressed;
                break;
            case sf::Keyboard::Key::F10:
                key[KEY_F10] = is_pressed;
                break;
            case sf::Keyboard::Key::F11:
                key[KEY_F11] = is_pressed;
                break;
            case sf::Keyboard::Key::F12:
                key[KEY_F12] = is_pressed;
                break;
            case sf::Keyboard::Key::Num1:
                key[KEY_1] = is_pressed;
                break;
            case sf::Keyboard::Key::Num2:
                key[KEY_2] = is_pressed;
                break;
            case sf::Keyboard::Key::Num3:
                key[KEY_3] = is_pressed;
                break;
            case sf::Keyboard::Key::Num4:
                key[KEY_4] = is_pressed;
                break;
            case sf::Keyboard::Key::Num5:
                key[KEY_5] = is_pressed;
                break;
            case sf::Keyboard::Key::Num6:
                key[KEY_6] = is_pressed;
                break;
            case sf::Keyboard::Key::Num7:
                key[KEY_7] = is_pressed;
                break;
            case sf::Keyboard::Key::Num8:
                key[KEY_8] = is_pressed;
                break;
            case sf::Keyboard::Key::Num9:
                key[KEY_9] = is_pressed;
                break;
            case sf::Keyboard::Key::Num0:
                key[KEY_0] = is_pressed;
                break;
            case sf::Keyboard::Key::Escape:
                key[KEY_ESC] = is_pressed;
                break;
            case sf::Keyboard::Key::Space:
                key[KEY_SPACE] = is_pressed;
                break;
            case sf::Keyboard::Key::Up:
                key[KEY_UP] = is_pressed;
                break;
            case sf::Keyboard::Key::Down:
                key[KEY_DOWN] = is_pressed;
                break;
            case sf::Keyboard::Key::Left:
                key[KEY_LEFT] = is_pressed;
                break;
            case sf::Keyboard::Key::Right:
                key[KEY_RIGHT] = is_pressed;
                break;
            }
        }
        }
    }
}
/**************************************************************************/
int keypressed()
{
    if (num_keypressed)
        return 1;

    sf::Event event;
    if (!window->pollEvent(event) || event.type != sf::Event::KeyPressed) {
        return 0;
    }

    num_keypressed = 1;

    return 1;
}
/**************************************************************************/
int readkey()
{
    sf::Event event;
    while (window->pollEvent(event) && event.type != sf::Event::KeyPressed) { }

    if (num_keypressed) {
        num_keypressed--;
    }

    switch (event.key.code) {
    case sf::Keyboard::Key::A:
        return KEY_A << 8;
    case sf::Keyboard::Key::B:
        return KEY_B << 8;
    case sf::Keyboard::Key::C:
        return KEY_C << 8;
    case sf::Keyboard::Key::D:
        return KEY_D << 8;
    case sf::Keyboard::Key::E:
        return KEY_E << 8;
    case sf::Keyboard::Key::F:
        return KEY_F << 8;
    case sf::Keyboard::Key::G:
        return KEY_G << 8;
    case sf::Keyboard::Key::H:
        return KEY_H << 8;
    case sf::Keyboard::Key::I:
        return KEY_I << 8;
    case sf::Keyboard::Key::J:
        return KEY_J << 8;
    case sf::Keyboard::Key::K:
        return KEY_K << 8;
    case sf::Keyboard::Key::L:
        return KEY_L << 8;
    case sf::Keyboard::Key::M:
        return KEY_M << 8;
    case sf::Keyboard::Key::N:
        return KEY_N << 8;
    case sf::Keyboard::Key::O:
        return KEY_O << 8;
    case sf::Keyboard::Key::P:
        return KEY_P << 8;
    case sf::Keyboard::Key::Q:
        return KEY_Q << 8;
    case sf::Keyboard::Key::R:
        return KEY_R << 8;
    case sf::Keyboard::Key::S:
        return KEY_S << 8;
    case sf::Keyboard::Key::T:
        return KEY_T << 8;
    case sf::Keyboard::Key::U:
        return KEY_U << 8;
    case sf::Keyboard::Key::V:
        return KEY_V << 8;
    case sf::Keyboard::Key::W:
        return KEY_W << 8;
    case sf::Keyboard::Key::X:
        return KEY_X << 8;
    case sf::Keyboard::Key::Y:
        return KEY_Y << 8;
    case sf::Keyboard::Key::Z:
        return KEY_Z << 8;
    case sf::Keyboard::Key::LControl:
        return KEY_LCONTROL << 8;
    case sf::Keyboard::Key::RControl:
        return KEY_RCONTROL << 8;
    case sf::Keyboard::Key::LShift:
        return KEY_LSHIFT << 8;
    case sf::Keyboard::Key::RShift:
        return KEY_RSHIFT << 8;
    case sf::Keyboard::Key::LAlt:
        return KEY_ALT << 8;
    case sf::Keyboard::Key::RAlt:
        return KEY_ALTGR << 8;
    case sf::Keyboard::Key::F1:
        return KEY_F1 << 8;
    case sf::Keyboard::Key::F2:
        return KEY_F2 << 8;
    case sf::Keyboard::Key::F3:
        return KEY_F3 << 8;
    case sf::Keyboard::Key::F4:
        return KEY_F4 << 8;
    case sf::Keyboard::Key::F5:
        return KEY_F5 << 8;
    case sf::Keyboard::Key::F6:
        return KEY_F6 << 8;
    case sf::Keyboard::Key::F7:
        return KEY_F7 << 8;
    case sf::Keyboard::Key::F8:
        return KEY_F8 << 8;
    case sf::Keyboard::Key::F9:
        return KEY_F9 << 8;
    case sf::Keyboard::Key::F10:
        return KEY_F10 << 8;
    case sf::Keyboard::Key::F11:
        return KEY_F11 << 8;
    case sf::Keyboard::Key::F12:
        return KEY_F12 << 8;
    case sf::Keyboard::Key::Num1:
        return KEY_1 << 8;
    case sf::Keyboard::Key::Num2:
        return KEY_2 << 8;
    case sf::Keyboard::Key::Num3:
        return KEY_3 << 8;
    case sf::Keyboard::Key::Num4:
        return KEY_4 << 8;
    case sf::Keyboard::Key::Num5:
        return KEY_5 << 8;
    case sf::Keyboard::Key::Num6:
        return KEY_6 << 8;
    case sf::Keyboard::Key::Num7:
        return KEY_7 << 8;
    case sf::Keyboard::Key::Num8:
        return KEY_8 << 8;
    case sf::Keyboard::Key::Num9:
        return KEY_9 << 8;
    case sf::Keyboard::Key::Num0:
        return KEY_0 << 8;
    case sf::Keyboard::Key::Escape:
        return KEY_ESC << 8;
    case sf::Keyboard::Key::Space:
        return KEY_SPACE << 8;
    case sf::Keyboard::Key::Up:
        return KEY_UP << 8;
    case sf::Keyboard::Key::Down:
        return KEY_DOWN << 8;
    case sf::Keyboard::Key::Left:
        return KEY_LEFT << 8;
    case sf::Keyboard::Key::Right:
        return KEY_RIGHT << 8;
    }
    return 0;
}
/**************************************************************************/
static int first = 1;
int allegro_init()
{
    first = 0;

    for (int i = 0; i < 256; i++)
        black_palette[i].r = black_palette[i].g = black_palette[i].b = 0;

    return 1;
}
/**************************************************************************/
int install_keyboard()
{
    if (first)
        allegro_init();

    num_keypressed = 0;

    return 1;
}
/**************************************************************************/
void keyboard_reset()
{
    memset(key, 0, 128);
}
/**************************************************************************/
int install_timer()
{
    memset(key, 0, 128);

    return 1;
}
/**************************************************************************/
void close_gfx()
{
    if (window == NULL)
        return;

    fprintf(stderr, "Closing ggi...");
    window->close();
    delete window;
    screen->vis = NULL;
    fprintf(stderr, "done\n");
}
/**************************************************************************/
int set_gfx_mode(int mode, int w, int h, int a, int b)
{
    int err;
    char* target;

    memset(key, 0, 128);
    screen = new BITMAP;
    screen->w = w;
    screen->h = h;

    window = new sf::RenderWindow();
    window->create(sf::VideoMode(w * 4, h * 4), "URBAN");
    screen->vis = new sf::Sprite;
    if (!screen->texture.create(w, h)) {
        fprintf(stderr, "Failed to create texture");
        exit(1);
    }
    screen->texture.setSrgb(false);
    screen->vis->setTexture(screen->texture);
    screen->vis->setScale(sf::Vector2f(4.f, 4.f));

    screen->dat = new char[screen->w * screen->h];
    screen->stride = screen->w;

    screen_offset_x = screen_offset_y = 0;

    screen->line = new char*[screen->h];

    for (int i = 0; i < screen->h; i++)
        screen->line[i] = screen->dat + i * screen->stride;

    have_directbuffer = 0;
    return 0;
}
/**************************************************************************/
float linear2srgb(float x)
{
    float s0 = sqrtf(x);
    float s1 = sqrtf(s0);
    float s2 = sqrtf(s1);
    return std::clamp(0.662002687f * s0 + 0.684122060f * s1 - 0.323583601f * s2 - 0.0225411470f * x, 0.0f, 1.0f);
}
/**************************************************************************/
int set_palette(PALETTE p)
{
    int i;

    for (i = 0; i < 256; i++) {

        screen->pal[i].r = p[i].r << 2;
        screen->pal[i].g = p[i].g << 2;
        screen->pal[i].b = p[i].b << 2;
    }

    return 0;
};
/**************************************************************************/
void get_palette(PALETTE p)
{
    int i;

    for (i = 0; i < 256; i++) {

        p[i].r = screen->pal[i].r >> 2;
        p[i].g = screen->pal[i].g >> 2;
        p[i].b = screen->pal[i].b >> 2;
    }
};
/**************************************************************************/
void stretch_line(char* src, char* dst, int s_w, int d_w)
{
    char* dst_end = dst + d_w;
    int i1, i2, dd;
    int x_inc = s_w / d_w;

    s_w %= d_w;

    i2 = (dd = (i1 = 2 * s_w) - d_w) - d_w;

    for (; dst < dst_end; dst++, src += x_inc) {

        *dst = *src;
        if (dd >= 0) {

            src++;
            dd += i2;
        } else
            dd += i1;
    }
}
/**************************************************************************/
void stretch_blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int s_w, int s_h, int dest_x, int dest_y, int d_w, int d_h)
{
    int start_x = dest_x;
    int start_y = dest_y;
    int w = d_w;
    int h = d_h;
    char* dst;
    char* src = source->dat + source->stride * source_y + source_x;
    int destwidth = dest->stride;
    int srcwidth = source->stride;

    /* Clipping */
    if (start_x < 0) {

        w += start_x;
        src -= start_x;
        start_x = 0;
    }
    if (w > dest->w) {
        w = dest->w;
    }
    if (start_y < 0) {

        h += start_y;
        src -= (start_y * source->stride);
        start_y = 0;
    }
    if ((start_y + h) > dest->h) {

        h -= (start_y + h) - dest->h;
    }
    if ((start_x + w) > dest->w) {

        w -= (start_x + w) - dest->w;
    }
    if (h < 0)
        return;
    if (w < 0)
        return;

    dst = dest->dat + (dest->stride * start_y) + start_x;

    if (h > dest->h) {
        h = dest->h;
    }
    /* Stretch blit */
    char* dst_end = dst + (destwidth * h);
    int j = s_h / d_h;
    int i1, i2, dd;

    s_h %= d_h;

    i2 = (dd = (i1 = 2 * s_h) - d_h) - d_h;

    for (; dst < dst_end; dst += destwidth, src += (j * srcwidth)) {
        stretch_line(src, dst, s_w, w);
        if (dd >= 0) {
            src += srcwidth;
            dd += i2;
        } else
            dd += i1;
    }
    if (dest == screen) {
        window->draw(*screen->vis);
        window->display();
    }
}
/**************************************************************************/
int ggiPutBox(sf::Texture& tex, int x, int y, int w, int h, const void* buf)
{
    std::vector<unsigned char> pixels;
    pixels.reserve(w * h * 4);
    auto col { reinterpret_cast<const unsigned char*>(buf) };
    for (auto i { 0 }; i != w * h; ++i) {
        pixels.push_back(screen->pal[*col].r);
        pixels.push_back(screen->pal[*col].g);
        pixels.push_back(screen->pal[*col].b);
        pixels.push_back(255);
        ++col;
    }
    tex.update(pixels.data(), w, h, x, y);
    return 0;
}
/**************************************************************************/
void blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height)
{
    int start_x = dest_x;
    int start_y = dest_y;
    int w = width;
    int h = height;
    char* dst;
    char* src = source->dat + source->stride * source_y + source_x;
    int destwidth = dest->stride;
    int srcwidth = source->stride;

    if (start_x < 0) {

        w += start_x;
        src -= start_x;
        start_x = 0;
    }
    if (w > dest->w) {
        w = dest->w;
    }
    if (start_y < 0) {

        h += start_y;
        src -= (start_y * source->stride);
        start_y = 0;
    }
    if ((start_y + h) > dest->h) {

        h -= (start_y + h) - dest->h;
    }
    if ((start_x + w) > dest->w) {

        w -= (start_x + w) - dest->w;
    }
    if (h < 0)
        return;
    if (w < 0)
        return;

    dst = dest->dat + (dest->stride * start_y) + start_x;

    if (h > dest->h) {
        h = dest->h;
    }
    int h2 = h;
    if (dest == screen && !have_directbuffer) {
        //		int lx = (int)src - ((int)source->dat + source_y * source->stride);
        int ly = source_y;
        while (h > 0) {

            ggiPutBox(screen->texture, start_x, ly++, w, 1, src);
            memcpy(dst, src, w);
            dst += destwidth;
            src += srcwidth;
            h--;
        }
    } else {
        while (h > 0) {
            memcpy(dst, src, w);
            dst += destwidth;
            src += srcwidth;
            h--;
        }
    }
    if (dest == screen) {
        window->draw(*screen->vis);
        window->display();
    }
}
/**************************************************************************/
void masked_blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height)
{
    int start_x = dest_x;
    int start_y = dest_y;
    int w = width;
    int h = height;
    char* dst;
    char* src = source->dat + source->stride * source_y + source_x;
    int destwidth = dest->stride;
    int srcwidth = source->stride;

    if (start_x < 0) {

        w += start_x;
        src -= start_x;
        start_x = 0;
    }
    if (w > dest->w) {
        w = dest->w;
    }
    if (start_y < 0) {

        h += start_y;
        src -= (start_y * source->stride);
        start_y = 0;
    }
    if ((start_y + h) > dest->h) {

        h -= (start_y + h) - dest->h;
    }
    if ((start_x + w) > dest->w) {

        w -= (start_x + w) - dest->w;
    }
    if (h < 0)
        return;
    if (w < 0)
        return;

    dst = dest->dat + (dest->stride * start_y) + start_x;

    if (h > dest->h) {
        h = dest->h;
    }
    int h2 = h;
    if (dest == screen && !have_directbuffer) {
        //		int lx = (int)dst - ((int)dest->dat + dest_y * dest->stride);
        int ly = dest_y;
        while (h > 0) {

            for (int i = 0; i < w; i++)
                if (src[i])
                    dst[i] = src[i];
            ggiPutBox(screen->texture, start_x, ly++, w, 1, dst);
            dst += destwidth;
            src += srcwidth;
            h--;
        }
    } else {
        while (h > 0) {
            for (int i = 0; i < w; i++)
                if (src[i])
                    dst[i] = src[i];
            src += srcwidth;
            dst += destwidth;
            h--;
        }
    }
    if (dest == screen) {
        window->draw(*screen->vis);
        window->display();
    }
}
/**************************************************************************/
int drawRectangle(sf::Texture& tex, int x, int y, int w, int h, int c)
{
    std::vector<unsigned char> pixels;
    pixels.reserve(w * h * 4);
    auto& col { screen->pal[c] };
    for (auto i { 0 }; i != w * h; ++i) {
        pixels.push_back(col.r);
        pixels.push_back(col.g);
        pixels.push_back(col.b);
        pixels.push_back(255);
    }
    tex.update(pixels.data(), w, h, x, y);
    return 0;
}
/**************************************************************************/
void line(BITMAP* bmp, int x1, int y1, int x2, int y2, int color)
{
}
/**************************************************************************/
void rect(BITMAP* bmp, int x1, int y1, int x2, int y2, int color)
{
}
/**************************************************************************/
void stretch_sprite(BITMAP* bmp, BITMAP* sprite, int x, int y, int w, int h)
{
}
/**************************************************************************/
void rectfill(BITMAP* bmp, int x1, int y1, int x2, int y2, int c)
{

    if (x1 < 0)
        x1 = 0;
    if (x2 < 0)
        return;
    if (x1 >= bmp->w)
        return;
    if (x2 >= bmp->w)
        x2 = bmp->w - 1;
    if (y1 < 0)
        y1 = 0;
    if (y2 < 0)
        return;
    if (y1 >= bmp->h)
        return;
    if (y2 >= bmp->h)
        y2 = bmp->h - 1;

    int w = x2 - x1 + 1;
    int h = y2 - y1 + 1;
    int h2 = h;

    char* ptr = bmp->dat + (y1 * bmp->stride) + x1;
    while (h) {

        memset(ptr, c, w);
        ptr += bmp->stride;
        h--;
    }
    if (bmp == screen) {
        if (!have_directbuffer) {
            drawRectangle(screen->texture, x1, y1, w, h2, c);
        }
        window->draw(*screen->vis);
        window->display();
    }
}
/**************************************************************************/
void destroy_bitmap(BITMAP* bmp)
{
    free(bmp->dat);

    free(bmp);
}
/**************************************************************************/
void clear_keybuf()
{
    sf::Event event;
    while (window->pollEvent(event)) { }

    keyboard_reset();

    num_keypressed = 0;
};
/**************************************************************************/
int install_int_ex(void (*proc)(), long speed)
{
    return 0;
}
/**************************************************************************/
int install_int(void (*proc)(), long speed)
{
    return 0;
}
/**************************************************************************/
void remove_int(void (*proc)())
{
}
/**************************************************************************/
int set_color(int i, RGB* rgb)
{
    screen->pal[i].r = rgb->r;
    screen->pal[i].g = rgb->g;
    screen->pal[i].b = rgb->b;
    screen->pal[i].a = 255;

    return 0;
}
/**************************************************************************/
int vsync()
{
    return 0;
};
/**************************************************************************/
void fade_interpolate(PALLETE src, PALETTE dest, PALETTE out, int pos, int start, int end)
{
    for (int i = start; i < end; i++) {

        out[i].r = ((src[i].r * (64 - pos)) + (dest[i].r * pos)) / 64;
        out[i].g = ((src[i].g * (64 - pos)) + (dest[i].g * pos)) / 64;
        out[i].b = ((src[i].b * (64 - pos)) + (dest[i].b * pos)) / 64;
    }
}
/**************************************************************************/
void fade_in(PALLETE p, int speed)
{
    PALETTE pal;
    PALETTE tmp_pal;
    int i;

    get_palette(tmp_pal);

    for (i = 0; i < 65; i += speed) {

        fade_interpolate(tmp_pal, p, pal, i, 0, 255);
        set_palette(pal);
        window->draw(*screen->vis);
        window->display();
        rest(20);
    }
    set_palette(pal);
    window->draw(*screen->vis);
    window->display();
}
/**************************************************************************/
void fade_out(int speed)
{
    PALETTE pal;
    PALETTE tmp_pal;
    int i;

    get_palette(tmp_pal);

    for (i = 0; i < 65; i += speed) {

        fade_interpolate(tmp_pal, black_palette, pal, i, 0, 255);
        set_palette(pal);
        window->draw(*screen->vis);
        window->display();
        rest(20);
    }
    set_palette(black_palette);
    window->draw(*screen->vis);
    window->display();
}
/**************************************************************************/
BITMAP* create_bitmap(int w, int h)
{
    BITMAP* bmp = new BITMAP;
    bmp->w = w;
    bmp->h = h;
    bmp->stride = w;

    bmp->vis = NULL;
    bmp->dat = new char[w * h];

    bmp->line = new char*[h];

    for (int i = 0; i < bmp->h; i++)
        bmp->line[i] = bmp->dat + i * bmp->stride;

    clear(bmp);

    return bmp;
}
/**************************************************************************/
void textout(BITMAP* bmp, FONT* f, char* str, int x, int y, int color) {};
/**************************************************************************/
void textout_centre(BITMAP* bmp, FONT* f, char* str, int x, int y, int color) {};
/**************************************************************************/
void putpixel(BITMAP* bmp, int x, int y, int c)
{
    bmp->dat[(bmp->stride * y) + x] = c;
};
/**************************************************************************/
void clear(BITMAP* bmp)
{
    int h = bmp->h;
    char* ptr = bmp->dat;
    int stride = bmp->stride;

    while (h--) {

        memset(ptr, 0, stride);
        ptr += stride;
    }

    if (bmp == screen) {
        if (!have_directbuffer) {
            drawRectangle(screen->texture, 0, 0, screen->w, screen->h, 0);
        }
        window->draw(*screen->vis);
        window->display();
    }
}
/**************************************************************************/
void clear_to_color(BITMAP* bmp, int color)
{
    int h = bmp->h;
    char* ptr = bmp->dat;
    int stride = bmp->stride;

    while (h--) {

        memset(ptr, color, stride);
        ptr += stride;
    }

    if (bmp == screen) {
        if (!have_directbuffer) {
            drawRectangle(screen->texture, 0, 0, screen->w, screen->h, color);
        }
        window->draw(*screen->vis);
        window->display();
    }
}
/**************************************************************************/
void xor_mode(int on)
{
}
