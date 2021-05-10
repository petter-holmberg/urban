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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ggi/ggi.h>
#include <allegro.h>
/**************************************************************************/
/* BITMAP *create_bitmap(int w, int h); */
/**************************************************************************/
BITMAP *screen;
FONT *font;
PALETTE black_palette;
unsigned char key[128];
/**************************************************************************/
static int screen_offset_x, screen_offset_y;
static int num_keypressed;
static int have_directbuffer;
/**************************************************************************/
void textprintf(BITMAP *bmp, FONT *f, int x, int y, int color, char *format, ...) {
}
/**************************************************************************/
void rest(long t) {
	usleep(1000 * t);
}
/**************************************************************************/
void keyboard_update() {
	struct timeval tv={0, 0};
        ggi_event event;
        ggi_event_mask event_mask = (ggi_event_mask) (emKeyPress | emKeyRelease);

	while(ggiEventPoll(screen->vis, event_mask, &tv)) {

        	ggiEventRead(screen->vis, &event, event_mask);

#define CASE_KEY(x, y) \
	case x: \
		key[y] = (event.any.type == evKeyPress)?1:0; \
	        break;

		switch(event.key.label) {

                	CASE_KEY(GIIUC_A, KEY_A);
                	CASE_KEY(GIIUC_B, KEY_B);
                	CASE_KEY(GIIUC_C, KEY_C);
                	CASE_KEY(GIIUC_D, KEY_D);
                	CASE_KEY(GIIUC_E, KEY_E);
                	CASE_KEY(GIIUC_F, KEY_F);
                	CASE_KEY(GIIUC_G, KEY_G);
                	CASE_KEY(GIIUC_H, KEY_H);
                	CASE_KEY(GIIUC_I, KEY_I);
                	CASE_KEY(GIIUC_J, KEY_J);
                	CASE_KEY(GIIUC_K, KEY_K);
                	CASE_KEY(GIIUC_L, KEY_L);
                	CASE_KEY(GIIUC_M, KEY_M);
                	CASE_KEY(GIIUC_N, KEY_N);
                	CASE_KEY(GIIUC_O, KEY_O);
                	CASE_KEY(GIIUC_P, KEY_P);
                	CASE_KEY(GIIUC_Q, KEY_Q);
                	CASE_KEY(GIIUC_R, KEY_R);
                	CASE_KEY(GIIUC_S, KEY_S);
                	CASE_KEY(GIIUC_T, KEY_T);
                	CASE_KEY(GIIUC_U, KEY_U);
                	CASE_KEY(GIIUC_V, KEY_V);
                	CASE_KEY(GIIUC_W, KEY_W);
                	CASE_KEY(GIIUC_X, KEY_X);
                	CASE_KEY(GIIUC_Y, KEY_Y);
                	CASE_KEY(GIIUC_Z, KEY_Z);
			CASE_KEY(GIIK_CtrlL, KEY_LCONTROL);
			CASE_KEY(GIIK_CtrlR, KEY_RCONTROL);
			CASE_KEY(GIIK_ShiftL, KEY_LSHIFT);
			CASE_KEY(GIIK_ShiftR, KEY_RSHIFT);
			CASE_KEY(GIIK_AltL, KEY_ALT);
			CASE_KEY(GIIK_AltR, KEY_ALTGR);
			CASE_KEY(GIIK_CapsLock, KEY_CAPSLOCK);
		};
                switch(event.key.sym) {
                	CASE_KEY(GIIK_F1, KEY_F1);
                	CASE_KEY(GIIK_F2, KEY_F2);
                	CASE_KEY(GIIK_F3, KEY_F3);
                	CASE_KEY(GIIK_F4, KEY_F4);
                	CASE_KEY(GIIK_F5, KEY_F5);
                	CASE_KEY(GIIK_F6, KEY_F6);
                	CASE_KEY(GIIK_F7, KEY_F7);
                	CASE_KEY(GIIK_F8, KEY_F8);
                	CASE_KEY(GIIK_F9, KEY_F9);
                	CASE_KEY(GIIK_F10, KEY_F10);
                	CASE_KEY(GIIK_F11, KEY_F11);
                	CASE_KEY(GIIK_F12, KEY_F12);
                	CASE_KEY(GIIUC_1, KEY_1);
                	CASE_KEY(GIIUC_2, KEY_2);
                	CASE_KEY(GIIUC_3, KEY_3);
                	CASE_KEY(GIIUC_4, KEY_4);
                	CASE_KEY(GIIUC_5, KEY_5);
                	CASE_KEY(GIIUC_6, KEY_6);
                	CASE_KEY(GIIUC_7, KEY_7);
                	CASE_KEY(GIIUC_8, KEY_8);
                	CASE_KEY(GIIUC_9, KEY_9);
                	CASE_KEY(GIIUC_0, KEY_0);
                	CASE_KEY(GIIUC_Escape, KEY_ESC);
                	CASE_KEY(GIIUC_Space, KEY_SPACE);
                	CASE_KEY(GIIK_Up, KEY_UP);
                	CASE_KEY(GIIK_Down, KEY_DOWN);
                	CASE_KEY(GIIK_Left, KEY_LEFT);
                	CASE_KEY(GIIK_Right, KEY_RIGHT);
                };
#undef CASE_KEY
        }
}
/**************************************************************************/
int keypressed() {
	struct timeval tv={0, 0};
        ggi_event event;
        ggi_event_mask event_mask = (ggi_event_mask) (emKeyPress);

	if(num_keypressed)
		return 1;

	if(!ggiEventPoll(screen->vis, event_mask, &tv))
		return 0;

	num_keypressed = 1;

	return 1;
/*       	ggiEventRead(screen->vis, &event, event_mask);

	return (event.any.type == evKeyPress)?1:0;*/
}
/**************************************************************************/
int readkey() {
	struct timeval tv={0, 0};
        ggi_event event;
   	int Key;
        ggi_event_mask event_mask = (ggi_event_mask) (emKeyPress | emKeyRepeat);

	while(!ggiEventPoll(screen->vis, event_mask, &tv));

	if(num_keypressed)
		num_keypressed--;

       	ggiEventRead(screen->vis, &event, event_mask);

#define CASE_KEY(x, y) case x: \
	return y << 8 | (event.key.sym & 0xff);

	switch(event.key.label) {

               	CASE_KEY(GIIUC_A, KEY_A);
               	CASE_KEY(GIIUC_B, KEY_B);
               	CASE_KEY(GIIUC_C, KEY_C);
               	CASE_KEY(GIIUC_D, KEY_D);
               	CASE_KEY(GIIUC_E, KEY_E);
               	CASE_KEY(GIIUC_F, KEY_F);
               	CASE_KEY(GIIUC_G, KEY_G);
               	CASE_KEY(GIIUC_H, KEY_H);
               	CASE_KEY(GIIUC_I, KEY_I);
               	CASE_KEY(GIIUC_J, KEY_J);
               	CASE_KEY(GIIUC_K, KEY_K);
               	CASE_KEY(GIIUC_L, KEY_L);
               	CASE_KEY(GIIUC_M, KEY_M);
               	CASE_KEY(GIIUC_N, KEY_N);
               	CASE_KEY(GIIUC_O, KEY_O);
               	CASE_KEY(GIIUC_P, KEY_P);
               	CASE_KEY(GIIUC_Q, KEY_Q);
               	CASE_KEY(GIIUC_R, KEY_R);
               	CASE_KEY(GIIUC_S, KEY_S);
               	CASE_KEY(GIIUC_T, KEY_T);
               	CASE_KEY(GIIUC_U, KEY_U);
               	CASE_KEY(GIIUC_V, KEY_V);
               	CASE_KEY(GIIUC_W, KEY_W);
               	CASE_KEY(GIIUC_X, KEY_X);
               	CASE_KEY(GIIUC_Y, KEY_Y);
               	CASE_KEY(GIIUC_Z, KEY_Z);
		CASE_KEY(GIIK_CtrlL, KEY_LCONTROL);
		CASE_KEY(GIIK_CtrlR, KEY_RCONTROL);
		CASE_KEY(GIIK_ShiftL, KEY_LSHIFT);
		CASE_KEY(GIIK_ShiftR, KEY_RSHIFT);
		CASE_KEY(GIIK_AltL, KEY_ALT);
		CASE_KEY(GIIK_AltR, KEY_ALTGR);
	        CASE_KEY(GIIK_CapsLock, KEY_CAPSLOCK);

	};
	switch(event.key.sym) {
               	CASE_KEY(GIIK_F1, KEY_F1);
               	CASE_KEY(GIIK_F2, KEY_F2);
               	CASE_KEY(GIIK_F3, KEY_F3);
               	CASE_KEY(GIIK_F4, KEY_F4);
               	CASE_KEY(GIIK_F5, KEY_F5);
               	CASE_KEY(GIIK_F6, KEY_F6);
               	CASE_KEY(GIIK_F7, KEY_F7);
               	CASE_KEY(GIIK_F8, KEY_F8);
               	CASE_KEY(GIIK_F9, KEY_F9);
               	CASE_KEY(GIIK_F10, KEY_F10);
               	CASE_KEY(GIIK_F11, KEY_F11);
               	CASE_KEY(GIIK_F12, KEY_F12);
               	CASE_KEY(GIIUC_1, KEY_1);
               	CASE_KEY(GIIUC_2, KEY_2);
               	CASE_KEY(GIIUC_3, KEY_3);
               	CASE_KEY(GIIUC_4, KEY_4);
               	CASE_KEY(GIIUC_5, KEY_5);
               	CASE_KEY(GIIUC_6, KEY_6);
               	CASE_KEY(GIIUC_7, KEY_7);
               	CASE_KEY(GIIUC_8, KEY_8);
               	CASE_KEY(GIIUC_9, KEY_9);
               	CASE_KEY(GIIUC_0, KEY_0);
        	CASE_KEY(GIIUC_Space, KEY_SPACE);
        	CASE_KEY(GIIUC_Escape, KEY_ESC);
        	CASE_KEY(GIIUC_BackSpace, KEY_BACKSPACE);
        	CASE_KEY(GIIK_Up, KEY_UP);
        	CASE_KEY(GIIK_Down, KEY_DOWN);
        	CASE_KEY(GIIK_Left, KEY_LEFT);
        	CASE_KEY(GIIK_Right, KEY_RIGHT);
        	CASE_KEY(GIIK_Enter, KEY_ENTER);
	}
#undef CASE_KEY
	return event.key.sym;
}
/**************************************************************************/
static int first = 1;
int allegro_init() {
	ggiInit();
        first = 0;

        for(int i=0;i<256;i++)
        	black_palette[i].r = black_palette[i].g = black_palette[i].b = 0;

	return 1;
}
/**************************************************************************/
int install_keyboard() {
	if(first)
        	allegro_init();

	num_keypressed = 0;

	return 1;
}
/**************************************************************************/
void keyboard_reset() {
	memset(key, 0, 128);
}
/**************************************************************************/
int install_timer() {
	memset(key, 0, 128);

	return 1;
}
/**************************************************************************/
void close_gfx() {
	if(screen == NULL)
		return;
	if(screen->vis == NULL)
		return;

	fprintf(stderr, "Closing ggi...");
	ggiClose(screen->vis);
	fprintf(stderr, "done\n");
}
/**************************************************************************/
int set_gfx_mode(int mode, int w, int h, int a, int b) {
	const ggi_directbuffer *buf;
        int err;
        ggi_mode m;
	char *target;

	memset(key, 0, 128);
	screen = new BITMAP;
        screen->w = w;
        screen->h = h;

        if(!(screen->vis = ggiOpen(NULL))) {
        	ggiPanic("GGI Panic, Can't open default visual!\n");
        }

        ggiSetFlags(screen->vis, GGIFLAG_ASYNC);

        if((err = ggiSetSimpleMode(screen->vis, w, h, GGI_AUTO, GT_8BIT))) {

	        if((err = ggiSetSimpleMode(screen->vis, GGI_AUTO, GGI_AUTO, GGI_AUTO, GT_8BIT))) {

	        	fprintf(stderr, "Looks like you don't have a 256 color target\nTesting with palemu target (this makes URBAN slower)\n", w, h);

			ggiClose(screen->vis);

			if(getenv("GGI_DISPLAY") == NULL)
				target = strdup("palemu");
			else {
				target = new char[4096];
				sprintf(target, "palemu:%s", getenv("GGI_DISPLAY"));
			}

		        if(!(screen->vis = ggiOpen(target))) {
				delete [] target;
		        	ggiPanic("GGI Panic, Can't open palemu visual!\n");
		        }
			delete [] target;

		        ggiSetFlags(screen->vis, GGIFLAG_ASYNC);

		        if((err = ggiSetSimpleMode(screen->vis, w, h, GGI_AUTO, GT_8BIT))) {

			        if((err = ggiSetSimpleMode(screen->vis, GGI_AUTO, GGI_AUTO, GGI_AUTO, GT_8BIT))) {
			        	fprintf(stderr, "Urban need at least %dx%d with at least 256 colors\n", w, h);
					exit(1);
				}
			}
		}
	}
        ggiGetMode(screen->vis, &m);

        if(m.visible.x < w || m.visible.y < h) {
        	fprintf(stderr, "Urban need at least %dx%d with at least 256 colors\n", w, h);
        }

        if(((buf = ggiDBGetBuffer(screen->vis, 0)) == NULL)||(!(buf->type & GGI_DB_SIMPLE_PLB))) {

        	fprintf(stderr, "Can't get screen directbuffer (this makes URBAN slower)\n");

	} else {

	        screen->dat = (char *)buf->write;
	        screen->stride = buf->buffer.plb.stride;

	        screen_offset_x = screen_offset_y = 0;

	        if(m.visible.x > w) {
			screen->dat+=((m.visible.x - w) / 2);
        	        screen_offset_x =((m.visible.x - w) / 2);
        	}
        	if(m.visible.y > h) {
			screen->dat+=((m.visible.y - h) / 2) * screen->stride;
                	screen_offset_y =((m.visible.y - h) / 2);
        	}
		screen->line = new char *[screen->h];

		for(int i=0;i < screen->h;i++)
			screen->line[i] = screen->dat + i * screen->stride;

		have_directbuffer = 1;
		return err;
	}
	/* No direct buffer */
	screen->dat = new char[screen->w * screen->h];
	screen->stride = screen->w;

	screen_offset_x = screen_offset_y = 0;

	screen->line = new char *[screen->h];

	for(int i=0;i < screen->h;i++)
		screen->line[i] = screen->dat + i * screen->stride;

	have_directbuffer = 0;
}
/**************************************************************************/
int set_palette(PALETTE p) {
	ggi_color pal[256];
	int i;

        for(i=0;i<256;i++) {

        	pal[i].r = p[i].r << 10;
        	pal[i].g = p[i].g << 10;
        	pal[i].b = p[i].b << 10;
        }
        ggiSetPalette(screen->vis, 0, 1 << 8, pal);

	return 0;
};
/**************************************************************************/
void get_palette(PALETTE p) {
	ggi_color pal[256];
	int i;

        ggiGetPalette(screen->vis, 0, 1 << 8, pal);
        for(i=0;i<256;i++) {

        	p[i].r = pal[i].r >> 10;
        	p[i].g = pal[i].g >> 10;
        	p[i].b = pal[i].b >> 10;
        }
};
/**************************************************************************/
void stretch_line(char *src, char *dst, int s_w, int d_w) {
        char *dst_end = dst + d_w;
        int i1, i2, dd;
	int x_inc = s_w / d_w;

	s_w %= d_w;

	i2 = (dd = (i1 = 2 * s_w) - d_w) - d_w;

        for(;dst < dst_end;dst++,src+=x_inc) {

		*dst = *src;
                if(dd >= 0) {

                	src++;
                        dd += i2;
                } else
			dd += i1;
        }
}
/**************************************************************************/
void stretch_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int s_w, int s_h, int dest_x, int dest_y, int d_w, int d_h) {
	int start_x = dest_x;
	int start_y = dest_y;
        int w = d_w;
        int h = d_h;
        char *dst;
        char *src = source->dat + source->stride * source_y + source_x;
        int destwidth = dest->stride;
        int srcwidth = source->stride;

	/* Clipping */
        if(start_x < 0) {

        	w += start_x;
                src -= start_x;
                start_x = 0;
        }
        if(w > dest->w) {
        	w = dest->w;
        }
        if(start_y < 0) {

        	h += start_y;
                src -= (start_y * source->stride);
                start_y = 0;
        }
        if((start_y  + h)> dest->h) {

        	h -= (start_y  + h) - dest->h;
        }
        if((start_x  + w)> dest->w) {

        	w -= (start_x  + w) - dest->w;
        }
        if(h < 0)
        	return;
        if(w < 0)
        	return;

	dst = dest->dat + (dest->stride * start_y) + start_x;

        if(h > dest->h) {
        	h = dest->h;
        }
        /* Stretch blit */
        char *dst_end = dst + (destwidth * h);
        int j = s_h / d_h;
	int i1, i2, dd;

	s_h %= d_h;

	i2 = (dd = (i1 = 2 * s_h) - d_h) - d_h;

        for(;dst < dst_end;dst+=destwidth,src+=(j * srcwidth)) {
        	stretch_line(src, dst, s_w, w);
                if(dd >= 0) {
                	src+= srcwidth;
                        dd += i2;
                } else
			dd += i1;
        }
        if(dest == screen)
/*		ggiFlush(screen->vis);*/
        	ggiFlushRegion(screen->vis, start_x + screen_offset_x,
                	start_y + screen_offset_y, w, h);
}
/**************************************************************************/
void blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height) {
	int start_x = dest_x;
	int start_y = dest_y;
        int w = width;
        int h = height;
        char *dst;
        char *src = source->dat + source->stride * source_y + source_x;
        int destwidth = dest->stride;
        int srcwidth = source->stride;

        if(start_x < 0) {

        	w += start_x;
                src -= start_x;
                start_x = 0;
        }
        if(w > dest->w) {
        	w = dest->w;
        }
        if(start_y < 0) {

        	h += start_y;
                src -= (start_y * source->stride);
                start_y = 0;
        }
        if((start_y  + h)> dest->h) {

        	h -= (start_y  + h) - dest->h;
        }
        if((start_x  + w)> dest->w) {

        	w -= (start_x  + w) - dest->w;
        }
        if(h < 0)
        	return;
        if(w < 0)
        	return;

	dst = dest->dat + (dest->stride * start_y) + start_x;

        if(h > dest->h) {
        	h = dest->h;
        }
	int h2 = h;
	if(dest == screen && !have_directbuffer) {
//		int lx = (int)src - ((int)source->dat + source_y * source->stride);
		int ly = source_y;
		while (h > 0) {

			ggiPutBox(screen->vis, start_x, ly++, w, 1, src);
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
        if(dest == screen) {
/*		ggiPutBox(screen->vis, start_x + screen_offset_x,
			  start_y + screen_offset_y, w, h2, screen->dat);*/

	       	ggiFlushRegion(screen->vis, start_x + screen_offset_x,
	               	start_y + screen_offset_y, w, h2);

	}
}
/**************************************************************************/
void masked_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height) {
	int start_x = dest_x;
	int start_y = dest_y;
        int w = width;
        int h = height;
        register char *dst;
        register char *src = source->dat + source->stride * source_y + source_x;
        int destwidth = dest->stride;
        int srcwidth = source->stride;

        if(start_x < 0) {

        	w += start_x;
                src -= start_x;
                start_x = 0;
        }
        if(w > dest->w) {
        	w = dest->w;
        }
        if(start_y < 0) {

        	h += start_y;
                src -= (start_y * source->stride);
                start_y = 0;
        }
        if((start_y  + h)> dest->h) {

        	h -= (start_y  + h) - dest->h;
        }
        if((start_x  + w)> dest->w) {

        	w -= (start_x  + w) - dest->w;
        }
        if(h < 0)
        	return;
        if(w < 0)
        	return;

	dst = dest->dat + (dest->stride * start_y) + start_x;

        if(h > dest->h) {
        	h = dest->h;
        }
  	int h2 = h;
	if(dest == screen && !have_directbuffer) {
//		int lx = (int)dst - ((int)dest->dat + dest_y * dest->stride);
		int ly = dest_y;
		while (h > 0) {

	        	for(register int i = 0;i< w;i++)
	                	if(src[i])
	                        	dst[i] = src[i];
			ggiPutBox(screen->vis, start_x, ly++, w, 1, dst);
			dst += destwidth;
			src += srcwidth;
			h--;
		}
	} else {
		while (h > 0) {
	        	for(register int i = 0;i< w;i++)
	                	if(src[i])
	                        	dst[i] = src[i];
			src += srcwidth;
			dst += destwidth;
			h--;
		}
	}
        if(dest == screen)
/*		ggiFlush(screen->vis);*/
        	ggiFlushRegion(screen->vis, start_x + screen_offset_x,
                	start_y + screen_offset_y, w, h2);
}
/**************************************************************************/
void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int c) {

	if(x1 < 0)
        	x1 = 0;
        if(x2 < 0)
        	return;
	if(x1 >= bmp->w)
        	return;
        if(x2 >= bmp->w)
        	x2 = bmp->w - 1;
	if(y1 < 0)
        	y1 = 0;
        if(y2 < 0)
        	return;
	if(y1 >= bmp->h)
        	return;
        if(y2 >= bmp->h)
        	y2 = bmp->h - 1;

	int w = x2 - x1 + 1;
        int h = y2 - y1 + 1;
        int h2 = h;

	char *ptr = bmp->dat + (y1 * bmp->stride) + x1;
        while(h) {

	        memset(ptr, c, w);
                ptr += bmp->stride;
                h--;
	}
        if(bmp == screen) {
		if(!have_directbuffer) {
			ggiSetGCForeground(screen->vis, c);
			ggiDrawBox(screen->vis, x1, y1, w, h2);
		}
        	ggiFlushRegion(screen->vis, x1 + screen_offset_x,
                	y1 + screen_offset_y, w, h2);
	}
}
/**************************************************************************/
void destroy_bitmap(BITMAP *bmp) {
	free(bmp->dat);

        free(bmp);
}
/**************************************************************************/
void clear_keybuf() {
	struct timeval tv={0, 0};
        ggi_event event;
        ggi_event_mask event_mask = (ggi_event_mask) (emKeyPress | emKeyRelease | emKeyRepeat);

	while(ggiEventPoll(screen->vis, event_mask, &tv))
        	ggiEventRead(screen->vis, &event, event_mask);

	keyboard_reset();

	num_keypressed = 0;
};
/**************************************************************************/
int install_int_ex(void (*proc)(), long speed) {
	return 0;
}
/**************************************************************************/
int install_int(void (*proc)(), long speed) {
	return 0;
}
/**************************************************************************/
void remove_int(void (*proc)()) {
}
/**************************************************************************/
int set_color(int i, RGB *rgb) {
	ggi_color Color;

        Color.r = rgb->r << 10;
        Color.g = rgb->g << 10;
        Color.b = rgb->b << 10;
        ggiSetPalette(screen->vis, i, 1, &Color);

	return 0;
}
/**************************************************************************/
int vsync() {
	return 0;
};
/**************************************************************************/
void fade_interpolate(PALLETE src, PALETTE dest, PALETTE out, int pos, int start, int end) {
	for(int i = start; i < end; i++) {

        	out[i].r = ((src[i].r * (64 - pos)) + (dest[i].r * pos)) / 64;
        	out[i].g = ((src[i].g * (64 - pos)) + (dest[i].g * pos)) / 64;
        	out[i].b = ((src[i].b * (64 - pos)) + (dest[i].b * pos)) / 64;
        }
}
/**************************************************************************/
void fade_in(PALLETE p, int speed) {
	PALETTE pal;
	PALETTE tmp_pal;
	int i;

        get_palette(tmp_pal);

        for(i=0;i < 65; i+=speed) {

		fade_interpolate(tmp_pal, p, pal, i, 0, 255);
	        set_palette(pal);
                ggiFlush(screen->vis);
                rest(20);
        }
        set_palette(pal);
/*	ggiFlush(screen->vis);*/
}
/**************************************************************************/
void fade_out(int speed) {
	PALETTE pal;
	PALETTE tmp_pal;
	int i;

        get_palette(tmp_pal);

        for(i=0;i < 65; i+=speed) {

		fade_interpolate(tmp_pal, black_palette, pal, i, 0, 255);
	        set_palette(pal);
                ggiFlush(screen->vis);
                rest(20);
        }
	set_palette(black_palette);
/*	ggiFlush(screen->vis);*/
}
/**************************************************************************/
BITMAP *create_bitmap(int w, int h) {
	BITMAP *bmp = new BITMAP;
        bmp->w = w;
        bmp->h = h;
        bmp->stride = w;

        bmp->vis = NULL;
        bmp->dat = new char[w * h];

	bmp->line = new char *[h];

	for(int i=0;i < bmp->h;i++)
		bmp->line[i] = bmp->dat + i * bmp->stride;

        clear(bmp);

        return bmp;
}
/**************************************************************************/
void textout(BITMAP *bmp, FONT *f, char *str, int x, int y, int color) {
};
/**************************************************************************/
void textout_centre(BITMAP *bmp, FONT *f, char *str, int x, int y, int color) {
};
/**************************************************************************/
void putpixel(BITMAP *bmp, int x, int y, int c) {
	bmp->dat[(bmp->stride * y) + x] = c;
};
/**************************************************************************/
void clear(BITMAP *bmp) {
	register int h = bmp->h;
	register char *ptr = bmp->dat;
	register int stride = bmp->stride;

	while(h--) {

		memset(ptr, 0, stride);
		ptr += stride;
	}

	if(bmp == screen) {
		if(!have_directbuffer) {
			ggiSetGCForeground(screen->vis, 0);
			ggiDrawBox(screen->vis, 0, 0, screen->w, screen->h);
		}
		ggiFlush(screen->vis);
	}
}
/**************************************************************************/
void clear_to_color(BITMAP *bmp, register int color) {
	register int h = bmp->h;
	register char *ptr = bmp->dat;
	register int stride = bmp->stride;

	while(h--) {

		memset(ptr, color, stride);
		ptr += stride;
	}

	if(bmp == screen) {
		if(!have_directbuffer) {
			ggiSetGCForeground(screen->vis, color);
			ggiDrawBox(screen->vis, 0, 0, screen->w, screen->h);
		}
		ggiFlush(screen->vis);
	}

}
/**************************************************************************/

