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
#include "allegro.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <thread>

/**************************************************************************/
/* BITMAP *create_bitmap(int w, int h); */
/**************************************************************************/
sf::RenderWindow* window;
BITMAP* screen;
FONT* font;
PALETTE black_palette;
std::array<bool, 128> key;
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
    std::this_thread::sleep_for(std::chrono::microseconds { 1000 * t });
}
/**************************************************************************/
void keyboard_update()
{
    sf::Event event {};
    while (window->pollEvent(event)) {
        switch (event.type) {
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased: {
            bool is_pressed = (event.type == sf::Event::KeyPressed);
            switch (event.key.code) {
            case sf::Keyboard::Key::A:
                key[static_cast<size_t>(scan_code::KEY_A)] = is_pressed;
                break;
            case sf::Keyboard::Key::B:
                key[static_cast<size_t>(scan_code::KEY_B)] = is_pressed;
                break;
            case sf::Keyboard::Key::C:
                key[static_cast<size_t>(scan_code::KEY_C)] = is_pressed;
                break;
            case sf::Keyboard::Key::D:
                key[static_cast<size_t>(scan_code::KEY_D)] = is_pressed;
                break;
            case sf::Keyboard::Key::E:
                key[static_cast<size_t>(scan_code::KEY_E)] = is_pressed;
                break;
            case sf::Keyboard::Key::F:
                key[static_cast<size_t>(scan_code::KEY_F)] = is_pressed;
                break;
            case sf::Keyboard::Key::G:
                key[static_cast<size_t>(scan_code::KEY_G)] = is_pressed;
                break;
            case sf::Keyboard::Key::H:
                key[static_cast<size_t>(scan_code::KEY_H)] = is_pressed;
                break;
            case sf::Keyboard::Key::I:
                key[static_cast<size_t>(scan_code::KEY_I)] = is_pressed;
                break;
            case sf::Keyboard::Key::J:
                key[static_cast<size_t>(scan_code::KEY_J)] = is_pressed;
                break;
            case sf::Keyboard::Key::K:
                key[static_cast<size_t>(scan_code::KEY_K)] = is_pressed;
                break;
            case sf::Keyboard::Key::L:
                key[static_cast<size_t>(scan_code::KEY_L)] = is_pressed;
                break;
            case sf::Keyboard::Key::M:
                key[static_cast<size_t>(scan_code::KEY_M)] = is_pressed;
                break;
            case sf::Keyboard::Key::N:
                key[static_cast<size_t>(scan_code::KEY_N)] = is_pressed;
                break;
            case sf::Keyboard::Key::O:
                key[static_cast<size_t>(scan_code::KEY_O)] = is_pressed;
                break;
            case sf::Keyboard::Key::P:
                key[static_cast<size_t>(scan_code::KEY_P)] = is_pressed;
                break;
            case sf::Keyboard::Key::Q:
                key[static_cast<size_t>(scan_code::KEY_Q)] = is_pressed;
                break;
            case sf::Keyboard::Key::R:
                key[static_cast<size_t>(scan_code::KEY_R)] = is_pressed;
                break;
            case sf::Keyboard::Key::S:
                key[static_cast<size_t>(scan_code::KEY_S)] = is_pressed;
                break;
            case sf::Keyboard::Key::T:
                key[static_cast<size_t>(scan_code::KEY_T)] = is_pressed;
                break;
            case sf::Keyboard::Key::U:
                key[static_cast<size_t>(scan_code::KEY_U)] = is_pressed;
                break;
            case sf::Keyboard::Key::V:
                key[static_cast<size_t>(scan_code::KEY_V)] = is_pressed;
                break;
            case sf::Keyboard::Key::W:
                key[static_cast<size_t>(scan_code::KEY_W)] = is_pressed;
                break;
            case sf::Keyboard::Key::X:
                key[static_cast<size_t>(scan_code::KEY_X)] = is_pressed;
                break;
            case sf::Keyboard::Key::Y:
                key[static_cast<size_t>(scan_code::KEY_Y)] = is_pressed;
                break;
            case sf::Keyboard::Key::Z:
                key[static_cast<size_t>(scan_code::KEY_Z)] = is_pressed;
                break;
            case sf::Keyboard::Key::LControl:
                key[static_cast<size_t>(scan_code::KEY_LCONTROL)] = is_pressed;
                break;
            case sf::Keyboard::Key::RControl:
                key[static_cast<size_t>(scan_code::KEY_RCONTROL)] = is_pressed;
                break;
            case sf::Keyboard::Key::LShift:
                key[static_cast<size_t>(scan_code::KEY_LSHIFT)] = is_pressed;
                break;
            case sf::Keyboard::Key::RShift:
                key[static_cast<size_t>(scan_code::KEY_RSHIFT)] = is_pressed;
                break;
            case sf::Keyboard::Key::LAlt:
                key[static_cast<size_t>(scan_code::KEY_ALT)] = is_pressed;
                break;
            case sf::Keyboard::Key::RAlt:
                key[static_cast<size_t>(scan_code::KEY_ALTGR)] = 1;
                break;
            case sf::Keyboard::Key::F1:
                key[static_cast<size_t>(scan_code::KEY_F1)] = is_pressed;
                break;
            case sf::Keyboard::Key::F2:
                key[static_cast<size_t>(scan_code::KEY_F2)] = is_pressed;
                break;
            case sf::Keyboard::Key::F3:
                key[static_cast<size_t>(scan_code::KEY_F3)] = is_pressed;
                break;
            case sf::Keyboard::Key::F4:
                key[static_cast<size_t>(scan_code::KEY_F4)] = is_pressed;
                break;
            case sf::Keyboard::Key::F5:
                key[static_cast<size_t>(scan_code::KEY_F5)] = is_pressed;
                break;
            case sf::Keyboard::Key::F6:
                key[static_cast<size_t>(scan_code::KEY_F6)] = is_pressed;
                break;
            case sf::Keyboard::Key::F7:
                key[static_cast<size_t>(scan_code::KEY_F7)] = is_pressed;
                break;
            case sf::Keyboard::Key::F8:
                key[static_cast<size_t>(scan_code::KEY_F8)] = is_pressed;
                break;
            case sf::Keyboard::Key::F9:
                key[static_cast<size_t>(scan_code::KEY_F9)] = is_pressed;
                break;
            case sf::Keyboard::Key::F10:
                key[static_cast<size_t>(scan_code::KEY_F10)] = is_pressed;
                break;
            case sf::Keyboard::Key::F11:
                key[static_cast<size_t>(scan_code::KEY_F11)] = is_pressed;
                break;
            case sf::Keyboard::Key::F12:
                key[static_cast<size_t>(scan_code::KEY_F12)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num1:
                key[static_cast<size_t>(scan_code::KEY_1)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num2:
                key[static_cast<size_t>(scan_code::KEY_2)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num3:
                key[static_cast<size_t>(scan_code::KEY_3)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num4:
                key[static_cast<size_t>(scan_code::KEY_4)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num5:
                key[static_cast<size_t>(scan_code::KEY_5)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num6:
                key[static_cast<size_t>(scan_code::KEY_6)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num7:
                key[static_cast<size_t>(scan_code::KEY_7)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num8:
                key[static_cast<size_t>(scan_code::KEY_8)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num9:
                key[static_cast<size_t>(scan_code::KEY_9)] = is_pressed;
                break;
            case sf::Keyboard::Key::Num0:
                key[static_cast<size_t>(scan_code::KEY_0)] = is_pressed;
                break;
            case sf::Keyboard::Key::Escape:
                key[static_cast<size_t>(scan_code::KEY_ESC)] = is_pressed;
                break;
            case sf::Keyboard::Key::Space:
                key[static_cast<size_t>(scan_code::KEY_SPACE)] = is_pressed;
                break;
            case sf::Keyboard::Key::Up:
                key[static_cast<size_t>(scan_code::KEY_UP)] = is_pressed;
                break;
            case sf::Keyboard::Key::Down:
                key[static_cast<size_t>(scan_code::KEY_DOWN)] = is_pressed;
                break;
            case sf::Keyboard::Key::Left:
                key[static_cast<size_t>(scan_code::KEY_LEFT)] = is_pressed;
                break;
            case sf::Keyboard::Key::Right:
                key[static_cast<size_t>(scan_code::KEY_RIGHT)] = is_pressed;
                break;
            }
        }
        }
    }
}
/**************************************************************************/
auto keypressed() -> int
{
    if (num_keypressed != 0) {
        return 1;
    }

    sf::Event event {};
    if (!window->pollEvent(event) || event.type != sf::Event::KeyPressed) {
        return 0;
    }

    num_keypressed = 1;

    return 1;
}
/**************************************************************************/
auto readkey() -> scan_code
{
    sf::Event event {};
    while (true) {
        if (window->pollEvent(event) && event.type == sf::Event::KeyPressed) {
            break;
        }
    }

    if (num_keypressed != 0) {
        num_keypressed--;
    }

    switch (event.key.code) {
    case sf::Keyboard::Key::A:
        return scan_code::KEY_A;
    case sf::Keyboard::Key::B:
        return scan_code::KEY_B;
    case sf::Keyboard::Key::C:
        return scan_code::KEY_C;
    case sf::Keyboard::Key::D:
        return scan_code::KEY_D;
    case sf::Keyboard::Key::E:
        return scan_code::KEY_E;
    case sf::Keyboard::Key::F:
        return scan_code::KEY_F;
    case sf::Keyboard::Key::G:
        return scan_code::KEY_G;
    case sf::Keyboard::Key::H:
        return scan_code::KEY_H;
    case sf::Keyboard::Key::I:
        return scan_code::KEY_I;
    case sf::Keyboard::Key::J:
        return scan_code::KEY_J;
    case sf::Keyboard::Key::K:
        return scan_code::KEY_K;
    case sf::Keyboard::Key::L:
        return scan_code::KEY_L;
    case sf::Keyboard::Key::M:
        return scan_code::KEY_M;
    case sf::Keyboard::Key::N:
        return scan_code::KEY_N;
    case sf::Keyboard::Key::O:
        return scan_code::KEY_O;
    case sf::Keyboard::Key::P:
        return scan_code::KEY_P;
    case sf::Keyboard::Key::Q:
        return scan_code::KEY_Q;
    case sf::Keyboard::Key::R:
        return scan_code::KEY_R;
    case sf::Keyboard::Key::S:
        return scan_code::KEY_S;
    case sf::Keyboard::Key::T:
        return scan_code::KEY_T;
    case sf::Keyboard::Key::U:
        return scan_code::KEY_U;
    case sf::Keyboard::Key::V:
        return scan_code::KEY_V;
    case sf::Keyboard::Key::W:
        return scan_code::KEY_W;
    case sf::Keyboard::Key::X:
        return scan_code::KEY_X;
    case sf::Keyboard::Key::Y:
        return scan_code::KEY_Y;
    case sf::Keyboard::Key::Z:
        return scan_code::KEY_Z;
    case sf::Keyboard::Key::LControl:
        return scan_code::KEY_LCONTROL;
    case sf::Keyboard::Key::RControl:
        return scan_code::KEY_RCONTROL;
    case sf::Keyboard::Key::LShift:
        return scan_code::KEY_LSHIFT;
    case sf::Keyboard::Key::RShift:
        return scan_code::KEY_RSHIFT;
    case sf::Keyboard::Key::LAlt:
        return scan_code::KEY_ALT;
    case sf::Keyboard::Key::RAlt:
        return scan_code::KEY_ALTGR;
    case sf::Keyboard::Key::F1:
        return scan_code::KEY_F1;
    case sf::Keyboard::Key::F2:
        return scan_code::KEY_F2;
    case sf::Keyboard::Key::F3:
        return scan_code::KEY_F3;
    case sf::Keyboard::Key::F4:
        return scan_code::KEY_F4;
    case sf::Keyboard::Key::F5:
        return scan_code::KEY_F5;
    case sf::Keyboard::Key::F6:
        return scan_code::KEY_F6;
    case sf::Keyboard::Key::F7:
        return scan_code::KEY_F7;
    case sf::Keyboard::Key::F8:
        return scan_code::KEY_F8;
    case sf::Keyboard::Key::F9:
        return scan_code::KEY_F9;
    case sf::Keyboard::Key::F10:
        return scan_code::KEY_F10;
    case sf::Keyboard::Key::F11:
        return scan_code::KEY_F11;
    case sf::Keyboard::Key::F12:
        return scan_code::KEY_F12;
    case sf::Keyboard::Key::Num1:
        return scan_code::KEY_1;
    case sf::Keyboard::Key::Num2:
        return scan_code::KEY_2;
    case sf::Keyboard::Key::Num3:
        return scan_code::KEY_3;
    case sf::Keyboard::Key::Num4:
        return scan_code::KEY_4;
    case sf::Keyboard::Key::Num5:
        return scan_code::KEY_5;
    case sf::Keyboard::Key::Num6:
        return scan_code::KEY_6;
    case sf::Keyboard::Key::Num7:
        return scan_code::KEY_7;
    case sf::Keyboard::Key::Num8:
        return scan_code::KEY_8;
    case sf::Keyboard::Key::Num9:
        return scan_code::KEY_9;
    case sf::Keyboard::Key::Num0:
        return scan_code::KEY_0;
    case sf::Keyboard::Key::Space:
        return scan_code::KEY_SPACE;
    case sf::Keyboard::Key::Escape:
        return scan_code::KEY_ESC;
    case sf::Keyboard::Key::Backspace:
        return scan_code::KEY_BACKSPACE;
    case sf::Keyboard::Key::Up:
        return scan_code::KEY_UP;
    case sf::Keyboard::Key::Down:
        return scan_code::KEY_DOWN;
    case sf::Keyboard::Key::Left:
        return scan_code::KEY_LEFT;
    case sf::Keyboard::Key::Right:
        return scan_code::KEY_RIGHT;
    case sf::Keyboard::Key::Enter:
        return scan_code::KEY_ENTER;
    }

    return scan_code::NONE;
}
/**************************************************************************/
auto readtext() -> char
{
    sf::Event event {};
    while (true) {
        if (window->pollEvent(event)) {
            if (event.type == sf::Event::TextEntered) {
                break;
            } else if (event.text.unicode == 59) { // backspace
                break;
            }
        }
    }

    if (event.text.unicode < 128) {
        return static_cast<char>(event.text.unicode);
    } else {
        return 0;
    }
}
/**************************************************************************/
static int first = 1;
auto allegro_init() -> int
{
    first = 0;

    for (auto& i : black_palette) {
        i.r = i.g = i.b = 0;
    }

    return 1;
}
/**************************************************************************/
auto install_keyboard() -> int
{
    if (first != 0) {
        allegro_init();
    }

    num_keypressed = 0;

    return 1;
}
/**************************************************************************/
void keyboard_reset()
{
    key.fill(false);
}
/**************************************************************************/
void close_gfx()
{
    if (window == nullptr) {
        return;
    }

    window->close();
    delete window;
    screen->vis = nullptr;
    fprintf(stderr, "done\n");
}
/**************************************************************************/
auto set_gfx_mode(int /*mode*/, int w, int h, int /*a*/, int /*b*/) -> int
{
    int err = 0;
    char* target = nullptr;

    keyboard_reset();
    screen = new BITMAP;
    screen->w = w;
    screen->h = h;

    sf::err().rdbuf(nullptr);
    window = new sf::RenderWindow();
    window->create(sf::VideoMode(w * 4, h * 4), "URBAN");
    screen->vis = new sf::Sprite;
    if (!screen->texture.create(w, h)) {
        fprintf(stderr, "Failed to create texture");
        exit(1);
    }
    screen->texture.setSrgb(false);
    screen->vis->setTexture(screen->texture);
    screen->vis->setScale(sf::Vector2f(4.F, 4.F));

    screen->dat = new char[screen->w * screen->h];
    screen->stride = screen->w;

    screen_offset_x = screen_offset_y = 0;

    screen->line = new char*[screen->h];

    for (int i = 0; i < screen->h; i++) {
        screen->line[i] = screen->dat + i * screen->stride;
    }

    have_directbuffer = 0;
    return 0;
}
/**************************************************************************/
auto linear2srgb(float x) -> float
{
    float s0 = sqrtf(x);
    float s1 = sqrtf(s0);
    float s2 = sqrtf(s1);
    return std::clamp(0.662002687F * s0 + 0.684122060F * s1 - 0.323583601F * s2 - 0.0225411470F * x, 0.0F, 1.0F);
}
/**************************************************************************/
auto set_palette(const PALETTE& p) -> int
{
    int i = 0;

    for (i = 0; i < 256; i++) {

        screen->pal[i].r = p[i].r << 2;
        screen->pal[i].g = p[i].g << 2;
        screen->pal[i].b = p[i].b << 2;
    }

    return 0;
};
/**************************************************************************/
void get_palette(PALETTE& p)
{
    int i = 0;

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
    int i1 = 0;
    int i2 = 0;
    int dd = 0;
    int x_inc = s_w / d_w;

    s_w %= d_w;

    i2 = (dd = (i1 = 2 * s_w) - d_w) - d_w;

    for (; dst < dst_end; dst++, src += x_inc) {

        *dst = *src;
        if (dd >= 0) {

            src++;
            dd += i2;
        } else {
            dd += i1;
        }
    }
}
/**************************************************************************/
void stretch_blit(BITMAP* source, BITMAP* dest, int source_x, int source_y, int s_w, int s_h, int dest_x, int dest_y, int d_w, int d_h)
{
    int start_x = dest_x;
    int start_y = dest_y;
    int w = d_w;
    int h = d_h;
    char* dst = nullptr;
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
    if (h < 0) {
        return;
    }
    if (w < 0) {
        return;
    }

    dst = dest->dat + (dest->stride * start_y) + start_x;

    if (h > dest->h) {
        h = dest->h;
    }
    /* Stretch blit */
    char* dst_end = dst + (destwidth * h);
    int j = s_h / d_h;
    int i1 = 0;
    int i2 = 0;
    int dd = 0;

    s_h %= d_h;

    i2 = (dd = (i1 = 2 * s_h) - d_h) - d_h;

    for (; dst < dst_end; dst += destwidth, src += (j * srcwidth)) {
        stretch_line(src, dst, s_w, w);
        if (dd >= 0) {
            src += srcwidth;
            dd += i2;
        } else {
            dd += i1;
        }
    }
    if (dest == screen) {
        window->draw(*screen->vis);
        window->display();
    }
}
/**************************************************************************/
auto ggiPutBox(sf::Texture& tex, int x, int y, int w, int h, const void* buf) -> int
{
    std::vector<unsigned char> pixels;
    pixels.reserve(w * h * 4);
    const auto* col { reinterpret_cast<const unsigned char*>(buf) };
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
    char* dst = nullptr;
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
    if (h < 0) {
        return;
    }
    if (w < 0) {
        return;
    }

    dst = dest->dat + (dest->stride * start_y) + start_x;

    if (h > dest->h) {
        h = dest->h;
    }
    int h2 = h;
    if (dest == screen && (have_directbuffer == 0)) {
        //		int lx = (int)src - ((int)source->dat + source_y * source->stride);
        int ly = source_y;
        while (h > 0) {

            ggiPutBox(screen->texture, start_x, ly++, w, 1, src);
            std::copy(src, src + w, dst);
            dst += destwidth;
            src += srcwidth;
            h--;
        }
    } else {
        while (h > 0) {
            std::copy(src, src + w, dst);
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
    char* dst = nullptr;
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
    if (h < 0) {
        return;
    }
    if (w < 0) {
        return;
    }

    dst = dest->dat + (dest->stride * start_y) + start_x;

    if (h > dest->h) {
        h = dest->h;
    }
    int h2 = h;
    if (dest == screen && (have_directbuffer == 0)) {
        //		int lx = (int)dst - ((int)dest->dat + dest_y * dest->stride);
        int ly = dest_y;
        while (h > 0) {

            for (int i = 0; i < w; i++) {
                if (src[i] != 0) {
                    dst[i] = src[i];
                }
            }
            ggiPutBox(screen->texture, start_x, ly++, w, 1, dst);
            dst += destwidth;
            src += srcwidth;
            h--;
        }
    } else {
        while (h > 0) {
            for (int i = 0; i < w; i++) {
                if (src[i] != 0) {
                    dst[i] = src[i];
                }
            }
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
auto drawRectangle(sf::Texture& tex, int x, int y, int w, int h, int c) -> int
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

    if (x1 < 0) {
        x1 = 0;
    }
    if (x2 < 0) {
        return;
    }
    if (x1 >= bmp->w) {
        return;
    }
    if (x2 >= bmp->w) {
        x2 = bmp->w - 1;
    }
    if (y1 < 0) {
        y1 = 0;
    }
    if (y2 < 0) {
        return;
    }
    if (y1 >= bmp->h) {
        return;
    }
    if (y2 >= bmp->h) {
        y2 = bmp->h - 1;
    }

    int w = x2 - x1 + 1;
    int h = y2 - y1 + 1;
    int h2 = h;

    char* ptr = bmp->dat + (y1 * bmp->stride) + x1;
    while (h != 0) {
        std::fill(ptr, ptr + w, c);
        ptr += bmp->stride;
        h--;
    }
    if (bmp == screen) {
        if (have_directbuffer == 0) {
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
    sf::Event event {};
    while (window->pollEvent(event)) { }

    keyboard_reset();

    num_keypressed = 0;
};
/**************************************************************************/
auto install_int_ex(void (*/*proc*/)(), long /*speed*/) -> int
{
    return 0;
}
/**************************************************************************/
auto install_int(void (*/*proc*/)(), long /*speed*/) -> int
{
    return 0;
}
/**************************************************************************/
void remove_int(void (*proc)())
{
}
/**************************************************************************/
auto set_color(int i, RGB* rgb) -> int
{
    screen->pal[i].r = rgb->r;
    screen->pal[i].g = rgb->g;
    screen->pal[i].b = rgb->b;
    screen->pal[i].a = 255;

    return 0;
}
/**************************************************************************/
auto vsync() -> int
{
    return 0;
};
/**************************************************************************/
void fade_interpolate(const PALETTE& src, const PALETTE& dest, PALETTE& out, int pos, int start, int end)
{
    for (int i = start; i < end; i++) {

        out[i].r = ((src[i].r * (64 - pos)) + (dest[i].r * pos)) / 64;
        out[i].g = ((src[i].g * (64 - pos)) + (dest[i].g * pos)) / 64;
        out[i].b = ((src[i].b * (64 - pos)) + (dest[i].b * pos)) / 64;
    }
}
/**************************************************************************/
void fade_in(const PALETTE& p, int speed)
{
    PALETTE pal;
    PALETTE tmp_pal;
    int i = 0;

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
    int i = 0;

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
auto create_bitmap(int w, int h) -> BITMAP*
{
    auto* bmp = new BITMAP;
    bmp->w = w;
    bmp->h = h;
    bmp->stride = w;

    bmp->vis = nullptr;
    bmp->dat = new char[w * h];

    bmp->line = new char*[h];

    for (int i = 0; i < bmp->h; i++) {
        bmp->line[i] = bmp->dat + i * bmp->stride;
    }

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

    while ((h--) != 0) {

        std::fill(ptr, ptr + stride, 0);
        ptr += stride;
    }

    if (bmp == screen) {
        if (have_directbuffer == 0) {
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

    while ((h--) != 0) {

        std::fill(ptr, ptr + stride, color);
        ptr += stride;
    }

    if (bmp == screen) {
        if (have_directbuffer == 0) {
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
