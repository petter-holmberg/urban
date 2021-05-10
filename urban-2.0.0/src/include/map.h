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

#include "allegro.h"
#include "datfile.h"
#include <array>
#include <memory>
#include <optional>
#include <string>
#include <vector>

inline constexpr auto MAX_TILES = 300;
inline constexpr auto MAX_OPTIONS = 50;
inline constexpr auto BG_TILE_WIDTH = 120;
inline constexpr auto BG_TILE_HEIGHT = 120;
inline constexpr auto PAL_AIR = 0;
inline constexpr auto PAL_WATER = 1;

using maptype = int;

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
    std::array<std::vector<maptype>, 3> sections {};
    std::array<std::vector<maptype>, 3> objects {};
    std::vector<maptype> background;
    std::array<std::vector<maptype>, 3> dekor {};
    std::array<std::vector<maptype>, 3> effect {};
    std::array<BITMAP*, MAX_TILES> Tiles {};
    std::array<BITMAP*, MAX_TILES> BGTiles {};
    std::unique_ptr<datfile> dat;
    std::array<int, MAX_TILES> TileInfo {};
    std::array<std::optional<std::string>, MAX_TILES> Filename {};
    std::array<std::optional<std::string>, MAX_TILES> BGFilename {};
    std::array<std::optional<std::string>, MAX_OPTIONS> options {};

    int x_size, y_size, bg_x_size, bg_y_size;
    int num_tiles, bg_num_tiles;
    int pal_count;
    int pal_type;
    PALETTE Pal {};
    PALETTE Pal_water {};
    PALETTE Pal_air {};
};

#endif
