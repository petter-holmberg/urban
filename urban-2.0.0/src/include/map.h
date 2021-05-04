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
#ifndef __MAP_H__
#define __MAP_H__

#include "datfile.h"
#include <allegro.h>
#include <stdio.h>

inline constexpr auto MAX_TILES = 300;
inline constexpr auto MAX_OPTIONS = 50;
inline constexpr auto BG_TILE_WIDTH = 120;
inline constexpr auto BG_TILE_HEIGHT = 120;
inline constexpr auto PAL_AIR = 0;
inline constexpr auto PAL_WATER = 1;

typedef int maptype;

class Map {
public:
    Map();
    ~Map();
    int LoadMap(const char* filename);
    int SaveMap(const char* filename);
    int GetDekor(int x, int y, int z);
    int GetEffect(int x, int y, int z);
    int GetBackGround(int x, int y);
    int GetBG(int x, int y, int z);
    int GetObj(int x, int y, int z);
    int SetDekor(int x, int y, int z, int v);
    int SetEffect(int x, int y, int z, int v);
    int SetBackGround(int x, int y, int v);
    int SetObj(int x, int y, int z, int v);
    int SetBG(int x, int y, int z, int v); //<- ta bort
    int GetWidth() const;
    int GetHeight() const;
    int GetNumTiles() { return num_tiles; }
    int GetBGNumTiles() { return bg_num_tiles; }
    BITMAP* GetTile(int num);
    int GetTileInfo(int num) const { return TileInfo[num]; };
    BITMAP* GetTile(int x, int y, int z);
    BITMAP* GetBGTile(int num);
    PALETTE& GetPal();
    void SetPal(int type);
    void UpdatePal();
    char* GetOption(const char* name);
    void SetOption(const char* name);
    void NewLevel(int width, int height);

private:
    static char* GetLine(FILE* fd);
    maptype* sections[3] {};
    maptype* objects[3] {};
    maptype* background;
    maptype* dekor[3] {};
    maptype* effect[3] {};
    BITMAP** Tiles;
    BITMAP** BGTiles {};
    datfile* dat;
    int* TileInfo;
    char** Filename;
    char** BGFilename {};
    char** options;

    int x_size, y_size, bg_x_size, bg_y_size;
    int num_tiles, bg_num_tiles;
    int pal_count;
    int pal_type;
    PALETTE Pal {};
    PALETTE Pal_water {};
    PALETTE Pal_air {};
};

#endif
