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
#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "icache.h"
#include "map.h"
#include "otypes.h"
#include <allegro.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define TILE_SIZE 1250
#define _TILE_WIDTH 26
#define _TILE_HEIGHT 40
#define _TILE_SIDE_HEIGHT 24
#define _TILE_TOP_HEIGHT (_TILE_HEIGHT - _TILE_SIDE_HEIGHT)
#define TILE_WIDTH (26 / draw_scale)
#define TILE_HEIGHT (40 / draw_scale)
#define TILE_SIDE_HEIGHT (24 / draw_scale)
#define TILE_TOP_HEIGHT (TILE_HEIGHT - TILE_SIDE_HEIGHT)
#undef BG_TILE_WIDTH
#undef BG_TILE_HEIGHT
#define BG_TILE_WIDTH (120 / draw_scale)
#define BG_TILE_HEIGHT (120 / draw_scale)
#define _BG_TILE_WIDTH 120
#define _BG_TILE_HEIGHT 120

#define MAX_Z (TILE_TOP_HEIGHT >> 1)
#define MIN_Z (3 * TILE_TOP_HEIGHT - 5)

#define MODE_NONE 0x00
#define MODE_TILES 0x01
#define MODE_OBJECTS 0x02
#define MODE_BACKGROUND 0x03
#define MODE_EFFECTS 0x04
#define MODE_DEKORATIONS 0x05

struct Layerinfo {
    char tiles_on;
    char objects_on;
    char effects_on;
    char dekorations_on;
};

class Editor {
public:
    Editor();
    ~Editor() = default;
    int EditLevel(char* filename, int, int);

private:
    void DrawScreen();
    void DrawCursor(BITMAP* buffer);
    void SelectTile();
    void SelectObject();
    void SelectEffect();
    void SelectDekor();
    void SelectBackground();
    void DrawTileSelectionMap(int start_row);
    void DrawBackgroundSelectionMap(int start_row);
    void DrawObjectSelectionMap(int start_row);
    void ShowMapStatus();
    static int Obj2Num(int o);
    Map* map;
    int map_x, map_y;
    int x {}, y {}, z {};
    int cursor_x, cursor_y, cursor_z;
    BITMAP** images;
    BITMAP* buffer;

    char* filename;

    struct Layerinfo layer_info[3] {};

    long num;
    long mode;
    long draw_zoomed_out;
    long draw_scale;
    unsigned int num_objects;
};

/*
class Editor {
	public:
        	Editor();
                ~Editor();
                int edit_level(char *level_name="");
                void draw_level(int start_x, int start_y, int *show_layer, int *show_layers);
                void set_level_name(char *);
                int handle_input();
                int edit_tiles();
                void draw_tiles();
                int edit_objects();
                void draw_objects();
                int edit_background();
                void draw_background();
		void NewLevelDialog();
       private:
                Map map;

	        int map_x, map_y;
                int cursor_x, cursor_y, cursor_z;
                int tile_cursor_x, tile_cursor_y;
                int object_cursor;
                int tile_editor_width, tile_editor_height;
                int block_number;
                int object_number;
                int bakgr_number;
                int mode;
                char file_name[60];
	 	BITMAP *buffer;
                BITMAP **images;
                int num_images;
};
*/
#endif