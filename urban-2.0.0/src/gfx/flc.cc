/******************************************************************************

    FLC-player
    Copyright (C) 1999  Thomas Nyberg

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

    Contact the author:
    Snailmail:
    Thomas Nyberg
    Br?nnbergsv?gen 21b
    92136 Lycksele
    Sweden

    or email:

    thomas.nyberg@usa.net


    This code is partly based on code privided by Shawn Hargreaves, found
    in Allegro. He based his work upon Jonathan Tarbox FLI/FLC-player.

*****************************************************************************/
#include "allegro.h"
#include <algorithm>

inline constexpr auto FLC_MAGIC = 0xaf12;
inline constexpr auto HEADER_FLAGS = 0x0003;
inline constexpr auto PREFIX_TYPE = 0xf100;
inline constexpr auto FRAME_TYPE = 0xf1fa;

inline constexpr auto FLI_COLOR256 = 0x0004;
inline constexpr auto FLI_SS2 = 0x0007;
inline constexpr auto FLI_COLOR = 0x000b;
inline constexpr auto FLI_LC = 0x000c;
inline constexpr auto FLI_BLACK = 0x000d;
inline constexpr auto FLI_BRUN = 0x000f;
inline constexpr auto FLI_COPY = 0x0010;
inline constexpr auto FLI_PSTAMP = 0x0012;

struct flc_header {
    uint32_t size;
    uint16_t magic;
    uint16_t n_frames;
    uint16_t width;
    uint16_t height;
    uint16_t depth;
    uint16_t flags;
    uint32_t speed;
    int16_t res0;
    int32_t c_time;
    int32_t res3;
    int32_t res4;
    int32_t res5;
    int16_t aspectx;
    int16_t aspecty;
    char res[38];
    int32_t oframe1;
    int32_t oframe2;
    char res2[40];
} __attribute__((packed));

struct prefix_chunk {
    int32_t size;
    int16_t type;
    int16_t n_chunks;
    char res[8];
} __attribute__((packed));

struct frame_chunk {
    int32_t size;
    int16_t type;
    int16_t n_chunks;
    char res[8];
} __attribute__((packed));

struct frame_header {
    int32_t size;
    int16_t type;
} __attribute__((packed));

inline constexpr auto FLC_NOT_OPEN = 0x00;
inline constexpr auto READ_FLC_FROM_DATFILE = 0x01;
inline constexpr auto READ_FLC_FROM_FILE = 0x02;
inline constexpr auto READ_FLC_FROM_MEMORY = 0x03;

char flc_status = FLC_NOT_OPEN;
FILE* flc_file = nullptr;
long flc_offset = 0;
char* flc_memory_buffer = nullptr;

flc_header flc_h;
prefix_chunk prefix_c;
frame_chunk frame_c;
prefix_chunk chunk;
frame_header frame_h;
BITMAP* flc_bitmap = nullptr;
BITMAP* target_bitmap = nullptr;
int (*flc_callback)() = nullptr;

static auto read_flc_data(char* buffer, int count) -> int
{
    int ret = 0;

    switch (flc_status) {
    case READ_FLC_FROM_DATFILE:
    case READ_FLC_FROM_FILE:
        if (flc_file == nullptr) {
            return -1;
        }
        ret = fread(buffer, 1, count, flc_file);
        return ret;
    case READ_FLC_FROM_MEMORY:
        if (flc_memory_buffer == nullptr) {
            return -1;
        }
        std::copy(flc_memory_buffer + flc_offset, flc_memory_buffer + flc_offset + count, buffer);
        flc_offset += count;
        break;
    case FLC_NOT_OPEN:
    default:
        break;
    }
    return 0;
}

static auto seek_flc_data(int offset, int mode) -> int
{
    switch (flc_status) {
    case READ_FLC_FROM_DATFILE:
    case READ_FLC_FROM_FILE:
        return fseek(flc_file, offset, mode);
    case READ_FLC_FROM_MEMORY:
        if (mode == SEEK_SET) {
            flc_offset = offset;
        } else if (mode == SEEK_CUR) {
            flc_offset += offset;
        }
        return flc_offset;
    case FLC_NOT_OPEN:
    default:
        break;
    }
    return 0;
}

static auto flc_read_header() -> int
{
    read_flc_data(reinterpret_cast<char*>(&flc_h), sizeof(flc_header));
    if (flc_h.magic != FLC_MAGIC) {
        return 1;
    }
    return 0;
}

static auto read_flc_palette(int type) -> int
{
    short num_packets = 0;
    int cc = 0;
    int i = 0;
    int j = 0;
    int num_c = 0;
    unsigned char skip = 0;
    RGB rgb;

    read_flc_data(reinterpret_cast<char*>(&num_packets), 2);
    for (i = 0; i < num_packets; i++) {
        read_flc_data(reinterpret_cast<char*>(&skip), 1);
        cc += skip;
        num_c = 0;
        read_flc_data(reinterpret_cast<char*>(&num_c), 1);
        num_c = num_c == 0 ? 256 : num_c;
        for (j = 0; j < num_c; j++) {
            read_flc_data(reinterpret_cast<char*>(&rgb.r), 1);
            read_flc_data(reinterpret_cast<char*>(&rgb.g), 1);
            read_flc_data(reinterpret_cast<char*>(&rgb.b), 1);
            if (type == FLI_COLOR256) {
                rgb.r;
                rgb.g;
                rgb.b;
            }
            set_color(cc++, &rgb);
        }
    }
    return 0;
}

static auto read_flc_brun() -> int
{
    int i = 0;
    int x = 0;
    int cw = 0;
    unsigned char num_p = 0;
    char type = 0;
    unsigned char temp = 0;
    for (i = 0; i < flc_h.height; i++) {
        read_flc_data(reinterpret_cast<char*>(&num_p), 1); //skit-v?rde
        cw = 0;
        x = 0;
        while (x < flc_h.width) {
            read_flc_data(&type, 1);
            if (type < 0) {
                read_flc_data(reinterpret_cast<char*>(&flc_bitmap->dat[(flc_bitmap->w * i) + x]), -type);
                x -= type;
            } else if (type > 0) {
                read_flc_data(reinterpret_cast<char*>(&temp), 1);
                //                                        for (j = 0;j < type;j++, x++)
                std::fill(
                    reinterpret_cast<unsigned char*>(&flc_bitmap->dat[(flc_bitmap->w * i) + x]),
                    reinterpret_cast<unsigned char*>(&flc_bitmap->dat[(flc_bitmap->w * i) + x]) + type,
                    temp);
                //                                        	*(flc_bitmap->line[i] + x) = temp;
                x += type;
            }
        }
    }
    return 0;
}

auto read_flc_ss2() -> int
{
    short num_lines = 0;
    short num_packets = 0;
    int y = 0;
    int x = 0;
    int ret = 0;
    unsigned char temp = 0;
    char count = 0;
    short tmp2 = 0;
    int i = 0;

    read_flc_data(reinterpret_cast<char*>(&num_lines), 2);
    while (num_lines-- > 0) {
        read_flc_data(reinterpret_cast<char*>(&num_packets), 2);
        while (num_packets < 0) {
            if ((num_packets & 0x4000) != 0) {
                y -= num_packets;
            } else {
                flc_bitmap->dat[(flc_bitmap->w * y) + flc_bitmap->w - 1] = num_packets & 0xff;
            }
            ret = read_flc_data(reinterpret_cast<char*>(&num_packets), 2);
        }
        x = 0;
        while (num_packets-- > 0) {
            read_flc_data(reinterpret_cast<char*>(&temp), 1);
            x += temp;
            read_flc_data(reinterpret_cast<char*>(&count), 1);
            if (count > 0) {
                read_flc_data(reinterpret_cast<char*>(&flc_bitmap->dat[(flc_bitmap->w * y) + x]), count * 2);
                x += count * 2;
            } else if (count < 0) {
                read_flc_data(reinterpret_cast<char*>(&tmp2), 2);
                count = -count;
                for (i = 0; i < count * 2; i++) {
                    *((short*)(&flc_bitmap->dat[flc_bitmap->w * y] + x + i)) = tmp2;
                }
                x += count * 2;
            }
        }
        y++;
    }
    return 0;
}

static auto next_frame() -> int
{
    int j = 0;

    for (j = 0; j < chunk.n_chunks; j++) {
        read_flc_data(reinterpret_cast<char*>(&frame_h), sizeof(frame_header));
        switch (frame_h.type) {
        case FLI_COLOR256:
        /* fallthrough */
        case FLI_COLOR:
            read_flc_palette(frame_h.type);
            break;
        case FLI_BLACK:
            clear(screen);
            break;
        case FLI_COPY: //st?ds ej
            break;
        case FLI_BRUN:
            read_flc_brun();
            break;
        case FLI_LC: //st?ds ej
            break;
        case FLI_SS2:
            read_flc_ss2();
            break;
        default:
            break;
        }
    }
    return 0;
}

static void do_flc_play(int loop)
{
    int i = 0;
    int ret = 0;

    for (;;) {
        seek_flc_data(flc_h.oframe1, SEEK_SET);

        for (i = 0; i < flc_h.n_frames; i++) {
            read_flc_data(reinterpret_cast<char*>(&chunk), sizeof(frame_chunk));
            next_frame();
            blit(flc_bitmap, target_bitmap, 0, 0, 0, 0, 320, 240);

            if (flc_callback != nullptr) {
                if ((ret = flc_callback()) != 0) {
                    return;
                }
            }
        }
        if (loop == 0) {
            break;
        }
    }
}

auto play_fli(char* filename, BITMAP* bmp, int loop, int (*callback)()) -> int
{

    flc_file = fopen(filename, "rb");
    flc_status = READ_FLC_FROM_FILE;
    flc_offset = 0;
    flc_read_header();
    if (flc_bitmap == nullptr) {
        flc_bitmap = create_bitmap(320, 240);
    }
    flc_callback = callback;
    target_bitmap = bmp;
    if (target_bitmap == nullptr) {
        return -1;
    }

    do_flc_play(loop);

    return 0;
}

auto play_memory_fli(char* fli_data, BITMAP* bmp, int loop, int (*callback)()) -> int
{
    flc_status = READ_FLC_FROM_MEMORY;
    flc_memory_buffer = (char*)fli_data;
    flc_offset = 0;

    flc_read_header();
    if (flc_bitmap == nullptr) {
        flc_bitmap = create_bitmap(320, 240);
    }
    flc_callback = callback;
    target_bitmap = bmp;
    if (target_bitmap == nullptr) {
        return -1;
    }

    do_flc_play(loop);

    return 0;
}
