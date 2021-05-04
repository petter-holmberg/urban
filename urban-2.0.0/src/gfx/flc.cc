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
#include <allegro.h>
#include <stdio.h>
#include <string.h>

//#define DEBUG_FLC_PLAYER
#ifdef DEBUG_FLC_PLAYER
#define SHOW_HEADERS
#define SHOW_CHUNKS
#define DEBUG_FILE

#ifdef DEBUG_FILE
#define PRINTF(x...) \
    fprintf(debugfile, x)
FILE* debugfile = NULL;
#else
#define PRINTF(x...) \
    printf(x)
#endif
#endif

#define FLC_MAGIC 0xaf12
#define HEADER_FLAGS 0x0003
#define PREFIX_TYPE 0xf100
#define FRAME_TYPE 0xf1fa

#define FLI_COLOR256 0x0004
#define FLI_SS2 0x0007
#define FLI_COLOR 0x000b
#define FLI_LC 0x000c
#define FLI_BLACK 0x000d
#define FLI_BRUN 0x000f
#define FLI_COPY 0x0010
#define FLI_PSTAMP 0x0012

struct flc_header {
    unsigned long size;
    unsigned short magic;
    unsigned short n_frames;
    unsigned short width;
    unsigned short height;
    unsigned short depth;
    unsigned short flags;
    unsigned long speed;
    short res0;
    long c_time;
    long res3;
    long res4;
    long res5;
    short aspectx;
    short aspecty;
    char res[38];
    long oframe1;
    long oframe2;
    char res2[40];
} __attribute__((packed));

struct prefix_chunk {
    long size;
    short type;
    short n_chunks;
    char res[8];
} __attribute__((packed));

struct frame_chunk {
    long size;
    short type;
    short n_chunks;
    char res[8];
} __attribute__((packed));

struct frame_header {
    long size;
    short type;
} __attribute__((packed));

#define FLC_NOT_OPEN 0x00
#define READ_FLC_FROM_DATFILE 0x01
#define READ_FLC_FROM_FILE 0x02
#define READ_FLC_FROM_MEMORY 0x03

char flc_status = FLC_NOT_OPEN;
FILE* flc_file = NULL;
long flc_offset = 0;
char* flc_memory_buffer = NULL;

struct flc_header flc_h;
struct prefix_chunk prefix_c;
struct frame_chunk frame_c;
struct prefix_chunk chunk;
struct frame_header frame_h;
BITMAP* flc_bitmap = NULL;
BITMAP* target_bitmap = NULL;
volatile unsigned long flc_counter = 0;
int (*flc_callback)() = NULL;

#define FRAME_SIZE (flc_h.width * flc_h.height)
#define FRAME_WIDTH (flc_h.width)
#define FRAME_HEIGHT (flc_h.height)

static int read_flc_data(void* buffer, int count)
{
    int ret;

    switch (flc_status) {
    case READ_FLC_FROM_DATFILE:
    case READ_FLC_FROM_FILE:
        if (flc_file == NULL)
            return -1;
        ret = fread(buffer, 1, count, flc_file);
        return ret;
    case READ_FLC_FROM_MEMORY:
        if (flc_memory_buffer == NULL)
            return -1;
        memcpy(buffer, flc_memory_buffer + flc_offset, count);
        flc_offset += count;
        break;
    case FLC_NOT_OPEN:
    default:
        break;
    }
    return 0;
}

static int seek_flc_data(int offset, int mode)
{
    switch (flc_status) {
    case READ_FLC_FROM_DATFILE:
    case READ_FLC_FROM_FILE:
        return fseek(flc_file, offset, mode);
    case READ_FLC_FROM_MEMORY:
        if (mode == SEEK_SET)
            flc_offset = offset;
        else if (mode == SEEK_CUR)
            flc_offset += offset;
        return flc_offset;
    case FLC_NOT_OPEN:
    default:
        break;
    }
    return 0;
}

static int flc_read_header()
{
    read_flc_data(&flc_h, sizeof(struct flc_header));
#ifdef SHOW_HEADERS
    PRINTF("Size: %d\n", flc_h.size);
    PRINTF("Magic: 0x%x\n", flc_h.magic);
#endif
    if (flc_h.magic != FLC_MAGIC) {
#ifdef SHOW_HEADERS
        PRINTF("Error: Not a valid flc-file\n");
#endif
        return 1;
    }
#ifdef SHOW_HEADERS
    PRINTF("Num frames: %d\n", flc_h.n_frames);
    PRINTF("Width: %d\n", flc_h.width);
    PRINTF("Height: %d\n", flc_h.height);
    PRINTF("Colordepth: %d\n", flc_h.depth);
    PRINTF("Flags: %d\n", flc_h.flags);
    PRINTF("Speed: %dms/frame\n", flc_h.speed);
    PRINTF("Aspect_x: %d\n", flc_h.aspectx);
    PRINTF("Aspect_y: %d\n", flc_h.aspecty);
    PRINTF("oFrame1: %d\n", flc_h.oframe1);
    PRINTF("oFrame2: %d\n", flc_h.oframe2);
#endif
    return 0;
}

static int read_flc_palette(int type)
{
    short num_packets;
    int cc = 0;
    int i, j;
    int num_c;
    unsigned char skip;
    RGB rgb;

    read_flc_data(&num_packets, 2);
#ifdef SHOW_CHUNKS
    PRINTF("Palette-chunk\n");
    PRINTF("Num packets: %d\n", num_packets);
#endif
    for (i = 0; i < num_packets; i++) {
        read_flc_data(&skip, 1);
#ifdef SHOW_CHUNKS
        PRINTF("Skip: %d\n", skip);
#endif
        cc += skip;
        num_c = 0;
        read_flc_data(&num_c, 1);
        num_c = num_c == 0 ? 256 : num_c;
#ifdef SHOW_CHUNKS
        PRINTF("Num colors: %d\n", num_c);
#endif
        for (j = 0; j < num_c; j++) {
            read_flc_data(&rgb.r, 1);
            read_flc_data(&rgb.g, 1);
            read_flc_data(&rgb.b, 1);
            if (type == FLI_COLOR256) {
                rgb.r >>= 2;
                rgb.g >>= 2;
                rgb.b >>= 2;
            }
#ifdef SHOW_CHUNKS
            PRINTF("R: %d\n", rgb.r);
            PRINTF("G: %d\n", rgb.g);
            PRINTF("B: %d\n", rgb.b);
#endif
            set_color(cc++, &rgb);
        }
    }
    return 0;
}

static int read_flc_brun()
{
    int i;
    int x;
    int cw = 0;
    unsigned char num_p;
    char type;
    unsigned char temp;
    for (i = 0; i < FRAME_HEIGHT; i++) {
        read_flc_data(&num_p, 1); //skit-v?rde
        cw = 0;
        x = 0;
        while (x < FRAME_WIDTH) {
            read_flc_data(&type, 1);
            if (type < 0) {
                read_flc_data(&flc_bitmap->dat[(flc_bitmap->w * i) + x], -type);
                x -= type;
            } else if (type > 0) {
                read_flc_data(&temp, 1);
                //                                        for (j = 0;j < type;j++, x++)
                memset(&flc_bitmap->dat[(flc_bitmap->w * i) + x], temp, type);
                //                                        	*(flc_bitmap->line[i] + x) = temp;
                x += type;
            }
        }
    }
    return 0;
}

int read_flc_ss2()
{
    short num_lines;
    short num_packets;
    int y = 0;
    int x;
    int ret;
    unsigned char temp;
    char count;
    short tmp2;
    int i;

    read_flc_data(&num_lines, 2);
#ifdef SHOW_CHUNKS
    PRINTF("SS2\n");
    PRINTF("Num lines: %d\n", num_lines);
#endif
    while (num_lines-- > 0) {
        read_flc_data(&num_packets, 2);
#ifdef SHOW_CHUNKS
        PRINTF("Num packets: %d\n", num_packets);
#endif
        while (num_packets < 0) {
            if (num_packets & 0x4000)
                y -= num_packets;
            else
                flc_bitmap->dat[(flc_bitmap->w * y) + flc_bitmap->w - 1] = num_packets & 0xff;
            ret = read_flc_data(&num_packets, 2);
#ifdef SHOW_CHUNKS
            PRINTF("-Num packets: %d\n", num_packets);
            PRINTF("Return: %d\n", ret);
#endif
        }
        x = 0;
        while (num_packets-- > 0) {
            read_flc_data(&temp, 1);
            x += temp;
            read_flc_data(&count, 1);
            if (count > 0) {
                read_flc_data(&flc_bitmap->dat[(flc_bitmap->w * y) + x], count * 2);
                x += count * 2;
            } else if (count < 0) {
                read_flc_data(&tmp2, 2);
                count = -count;
                for (i = 0; i < count * 2; i++)
                    *((short*)(&flc_bitmap->dat[flc_bitmap->w * y] + x + i)) = tmp2;
                x += count * 2;
            }
        }
        y++;
    }
    return 0;
}

static int next_frame()
{
    int j;

    for (j = 0; j < chunk.n_chunks; j++) {
        read_flc_data(&frame_h, sizeof(struct frame_header));
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
    int i;
    int ret;

    for (;;) {
        seek_flc_data(flc_h.oframe1, SEEK_SET);

        for (i = 0; i < flc_h.n_frames; i++) {
            flc_counter = 0;
            read_flc_data(&chunk, sizeof(struct frame_chunk));
            next_frame();
            blit(flc_bitmap, target_bitmap, 0, 0, 0, 0, 320, 240);

            if (flc_callback != NULL) {
                if ((ret = flc_callback()))
                    return;
            }
        }
        if (!loop)
            break;
    }
}

int play_fli(char* filename, BITMAP* bmp, int loop, int (*callback)())
{

    flc_file = fopen(filename, "rb");
    flc_status = READ_FLC_FROM_FILE;
    flc_offset = 0;
    flc_read_header();
    if (flc_bitmap == NULL)
        flc_bitmap = create_bitmap(320, 240);
    LOCK_VARIABLE(flc_counter);
    LOCK_FUNCTION(flc_timer_callback);
    flc_callback = callback;
    target_bitmap = bmp;
    if (!target_bitmap)
        return -1;

    do_flc_play(loop);

    return 0;
}

int play_memory_fli(void* fli_data, BITMAP* bmp, int loop, int (*callback)())
{
    flc_status = READ_FLC_FROM_MEMORY;
    flc_memory_buffer = (char*)fli_data;
    flc_offset = 0;

    flc_read_header();
    if (flc_bitmap == NULL)
        flc_bitmap = create_bitmap(320, 240);
    LOCK_VARIABLE(flc_counter);
    flc_callback = callback;
    target_bitmap = bmp;
    if (!target_bitmap)
        return -1;

    do_flc_play(loop);

    return 0;
}
