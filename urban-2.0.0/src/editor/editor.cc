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

/*
Tangenter:
F1 - Välj Tile
F2 - Välj Background
F3 - Välj Object
F4 - Välj Effect (ej färdigt)
F5 - Välj Dekoration (ej färdigt)
F12 - Save
F11 - Save As (ej färdigt)
F10 - Open (ej färdigt)
F9 - New (ej färdigt)
F8 - Visa status (antal object osv...)
F7 - Kör optimizern (ej färdigt)
F6 - Zooma out
1 - Visa/Göm Tile-lager 1
2 - Visa/Göm Tile-lager 2
3 - Visa/Göm Tile-lager 3
4 - Visa/Göm Tile-lagren (on=off, off=on!)
5 - Visa/Göm Object-lager 1
6 - Visa/Göm Object-lager 2
7 - Visa/Göm Object-lager 3
8 - Visa/Göm Object-lagren (on=off, off=on!)
TAB - visa senaste MODE-menyn
SHIFT+UP/DOWN - Håll in shift för ökad scrollhastighet
PILTANGENTERNA - Flytta markören
PGUP/PGDOWN - Flytta mellan lagren
HOME/END - Flytta till början/slutet av banan (ej färdigt)
SPACE/DEL - Placera ut/ta bort tiles/object/...

*/
#include "editor.h"

#define MAP_WIDTH map->GetWidth()
#define MAP_HEIGHT map->GetHeight()
#define NUM_Y_TILES (SCREEN_HEIGHT / TILE_SIDE_HEIGHT)
#define NUM_X_TILES (SCREEN_WIDTH / TILE_WIDTH)
#define NUM_Y_BGTILES (SCREEN_HEIGHT / BG_TILE_HEIGHT)
#define NUM_X_BGTILES (SCREEN_WIDTH / BG_TILE_WIDTH)
#define _SEL_Y_T_B (mode == MODE_BACKGROUND ? BG_TILE_HEIGHT : TILE_SIDE_HEIGHT)
#define _SEL_X_T_B (mode == MODE_BACKGROUND ? BG_TILE_WIDTH : TILE_WIDTH - 1)
#define _GET_Y_TH (mode == MODE_BACKGROUND ? NUM_Y_BGTILES : NUM_Y_TILES)
#define _GET_X_TH (mode == MODE_BACKGROUND ? NUM_X_BGTILES : NUM_X_TILES)
#define TRANSPARENT_TILE 4096
#define IS_TRANSPARENT(n) (map->GetTileInfo((n)-1) & TRANSPARENT_TILE)
#define KEY_OFF(x) \
    while (key[x]) \
        ;
#define MAX_ZOOM_LEVEL 10
#define IS_BOSS(x) \
    ((x) == TYPE_BOSS_SCIENTIST || (x) == TYPE_BOSS_SOLDIER || (x) == TYPE_BOSS_RAMBO || (x) == TYPE_BOSS_ARNOLD || (x) == TYPE_BOSS_DOLPH || (x) == TYPE_BOSS_EINSTEIN || (x) == TYPE_BOSS_CRYCHECK || (x) == TYPE_BOSS_DR_GREEN)

struct ObjectTypes {
    unsigned long num;
    char filename[128];
    char description[128];
} Objects[] = {
    { TYPE_PLAYER, "cyborg5.pcx", "Player" },
    { TYPE_SCIENTIST, "urban/prof1.pcx", "Civilian" },
    { TYPE_SOLDIER, "soldier/soldv/5.pcx", "Soldier" },
    { TYPE_POWERUP_ENERGY, "items/medipak.pcx", "Medipak" },
    { TYPE_MINE, "mine1.pcx", "Mine" },
    { TYPE_SOLDIER2, "soldier2/v/5.pcx", "Soldier" },
    { TYPE_VAKTM, "vaktm/v/4.pcx", "Civilian" },
    { TYPE_WEAPON_FLAME_THROWER, "items/flame.pcx", "Flamethrower" },
    { TYPE_FLOORGUN, "auto-f/shot1.pcx", "NONE" },
    { TYPE_AMMO_FLAME_THROWER, "items/flamea.pcx", "Flame-Ammo" },
    { TYPE_WEAPON_ICEMAKER, "items/ice.pcx", "IceMaker (tm)" },
    { TYPE_AMMO_ICEMAKER, "items/icea.pcx", "Ice-Ammo" },
    { TYPE_WEAPON_GRENADE_LAUNCHER, "items/granadel.pcx", "Rocketl" },
    { TYPE_AMMO_GRENADE_LAUNCHER, "items/granade.pcx", "1 Rocket" },
    { TYPE_AMMO_GRENADE_LAUNCHER6, "items/granadea.pcx", "6 Rockets" },
    { TYPE_RAMBO, "specf/specv/5.pcx", "Special Forces" },
    { TYPE_ARNOLD, "specf2/specv/5.pcx", "Special Forces" },
    { TYPE_CHECKPOINT, "items/check.pcx", "Checkpoint" },
    { TYPE_EXTRA_LIFE, "items/xlife.pcx", "Extralife" },
    { TYPE_DOOR, "door1/1.pcx", "Door (green)" },
    { TYPE_BARREL, "tiles/barrel.pcx", "Explosive Tile" },
    { TYPE_DOLPH, "specf3/specv/5.pcx", "Special Forces" },
    { TYPE_RED_DOOR, "door1/1.pcx", "Red Door" },
    { TYPE_BLUE_DOOR, "door1/1.pcx", "Blue Door" },
    { TYPE_GREEN_DOOR, "door1/1.pcx", "Green Door" },
    { TYPE_EINSTEIN, "prof2/prof2v/1.pcx", "Civilian" },
    { TYPE_CRYCHECK, "inspecto/inspv/1.pcx", "CIA Agent" },
#ifdef __AT_THOMAS_HOME__
    { TYPE_LEFT_COMPUTER_SCREEN, "dekor/skÄrm1.pcx", "Item" },
    { TYPE_RIGHT_COMPUTER_SCREEN, "dekor/skÄrm2.pcx", "Item" },
#else
    { TYPE_LEFT_COMPUTER_SCREEN, "dekor/skarm1.pcx", "Item" },
    { TYPE_RIGHT_COMPUTER_SCREEN, "dekor/skarm2.pcx", "Item" },
#endif
    { TYPE_RED_CARD, "items/redcard.pcx", "Red Card" },
    { TYPE_BLUE_CARD, "items/bluecard.pcx", "Blue Card" },
    { TYPE_GREEN_CARD, "items/greencar.pcx", "Green Card" },
    { TYPE_DR_GREEN, "doc/docv/5.pcx", "Civilian" },
    { TYPE_BOSS_SCIENTIST, "urban/prof1.pcx", "BOSS - Civilian" },
    { TYPE_BOSS_SOLDIER, "soldier/soldv/5.pcx", "BOSS - Soldier" },
    { TYPE_BOSS_RAMBO, "specf/specv/5.pcx", "BOSS - Special Forces" },
    { TYPE_BOSS_ARNOLD, "specf2/specv/5.pcx", "BOSS - Special Forces" },
    { TYPE_BOSS_DOLPH, "specf3/specv/5.pcx", "BOSS - Special Forces" },
    { TYPE_BOSS_EINSTEIN, "prof2/prof2v/1.pcx", "BOSS - Civilian" },
    { TYPE_BOSS_CRYCHECK, "inspecto/inspv/1.pcx", "BOSS - CIA Agent" },
    { TYPE_BOSS_DR_GREEN, "doc/docv/5.pcx", "BOSS - Civilian" },
    { TYPE_DEKOR_ROBOT1, "dekor/robot1.pcx", "" },
    { TYPE_DEKOR_ROBOT2, "dekor/robot2.pcx", "" },
    { TYPE_DEKOR_ROBOT3, "dekor/robot3.pcx", "" },
    { TYPE_DEKOR_STOL1, "dekor/stol1.pcx", "" },
    { TYPE_DEKOR_STOL2, "dekor/stol2.pcx", "" },
    { TYPE_DEKOR_RITNING1, "dekor/ritning1.pcx", "" },
    { TYPE_DEKOR_RITNING2, "dekor/ritning2.pcx", "" },
    { TYPE_DEKOR_WEAPON1, "dekor/vapen.pcx", "" },
    { TYPE_DEKOR_HELMET1, "dekor/hjalm.pcx", "" },
    { TYPE_DEKOR_ROCKETPART1, "dekor/rockpart.pcx", "" },
    { TYPE_DEKOR_ROCKETPART2, "dekor/warhead.pcx", "" },
    { TYPE_DEKOR_ROCKETPART3, "dekor/rockp2.pcx", "" },
    { TYPE_SEAGAL, "specf4/specv/5.pcx", "Special Forces" },
    { TYPE_BOSS_SEAGAL, "specf4/specv/5.pcx", "BOSS - Special Forces" },
    { TYPE_AMMO_PLASMA, "items/plasmaa.pcx", "Plasma-Ammo" },
    { TYPE_WEAPON_PLASMA, "items/plasmal.pcx", "PlasmaGun" },
    { TYPE_AMMO_ELECTRIC, "items/ela.pcx", "Electric-Ammo" },
    { TYPE_WEAPON_ELECTRIC, "items/electric.pcx", "ElectricGun" },
    { TYPE_AMMO_MINIGUN, "items/miniguna.pcx", "Minigun-Ammo" },
    { TYPE_WEAPON_MINIGUN, "items/minigun.pcx", "Minigun" },
    { TYPE_DEKOR_FLAG, "dekor/flagga.pcx", "" },
    { TYPE_DOOR_EXTRA_LEVEL, "door1/1.pcx", "DOOR TO EXTRA LEVEL" },
    { TYPE_ELEVATOR, "elevator.pcx", "Elevator" },
    { TYPE_ELEVUP, "elepan1.pcx", "Elevator UP" },
    { TYPE_ELEVDOWN, "elepan1.pcx", "Elevator DOWN" },
    { TYPE_ELEVUP_GREEN, "elevup.pcx", "GREEN UP" },
    { TYPE_ELEVDOWN_GREEN, "elevdown.pcx", "GREEN DOWN" },
    { TYPE_ELEVWIRE, "connect.pcx", "Connect elevators" },
    { TYPE_SOLDIER4, "soldier2/v/5.pcx", "Soldier" },
    { TYPE_WATER, "water.pcx", "Water Surface" },
    { TYPE_BUBBLEMAKER, "bubble.pcx", "Bubble Maker" },
    { TYPE_AIRSTRIKER, "airstrik.pcx", "Airstrike" },
    { TYPE_TANK, "boss/h/skada0/1.pcx", "MegaBoss" },
    { 0, "", "" }
};

ImageCache icache;

/**************************************************************************/

Editor::Editor()
{
    PALETTE pal;
    unsigned int i = 0;

    map_x = 0;
    map_y = 0;
    cursor_x = 0;
    cursor_y = 0;
    cursor_z = 0;
    num_objects = 0;
    images = nullptr;
    map = new Map;
    mode = MODE_TILES;
    num = 0;
    filename = nullptr;
    draw_zoomed_out = 0;
    draw_scale = 1;

    layer_info[0].tiles_on = layer_info[1].tiles_on = layer_info[2].tiles_on = 1;
    layer_info[0].objects_on = layer_info[1].objects_on = layer_info[2].objects_on = 1;

    buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);

    while (*Objects[num_objects].filename != 0) {
        num_objects++; //hur många object är det?
    }

    images = new BITMAP*[num_objects];

    for (i = 0; i < num_objects; i++) {
        images[i] = icache.GetImage(Objects[i].filename, pal);
    }
}

/**************************************************************************/

Editor::~Editor()
    = default;

#define TILES_PER_ROW (SCREEN_WIDTH / (_TILE_WIDTH + 2))
#define TILES_PER_COL (SCREEN_HEIGHT / (_TILE_HEIGHT + 2))
void Editor::DrawTileSelectionMap(int start_row)
{
    int x = 0;
    int y = 0;
    int n = start_row * TILES_PER_ROW;
    clear(buffer);

    while (n < map->GetNumTiles()) {
        blit(map->GetTile(n), buffer,
            0, 0, x,
            y, _TILE_WIDTH, _TILE_HEIGHT);
        x += _TILE_WIDTH + 2;
        if (x > 320 - _TILE_WIDTH) {
            x = 0;
            y += _TILE_HEIGHT + 2;
        }
        n++;
    }
}

void Editor::SelectTile()
{
    static int x = 0;
    static int y = 0;
    static int row_y = 0;

    DrawTileSelectionMap(row_y);
    KEY_OFF(KEY_TAB);
    xor_mode(1);
    rect(buffer, x * (_TILE_WIDTH + 2) - 1, y * (_TILE_HEIGHT + 2) - 1,
        x * (_TILE_WIDTH + 2) + _TILE_WIDTH, y * (_TILE_HEIGHT + 2) + _TILE_HEIGHT,
        36);

    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    while (key[KEY_ESC] == 0u) {
        rest(50);
        if ((key[KEY_LEFT] != 0u) && (x != 0)) {
            rect(buffer, x * (_TILE_WIDTH + 2) - 1, y * (_TILE_HEIGHT + 2) - 1,
                x * (_TILE_WIDTH + 2) + _TILE_WIDTH, y * (_TILE_HEIGHT + 2) + _TILE_HEIGHT,
                36);
            x--;
            rect(buffer, x * (_TILE_WIDTH + 2) - 1, y * (_TILE_HEIGHT + 2) - 1,
                x * (_TILE_WIDTH + 2) + _TILE_WIDTH, y * (_TILE_HEIGHT + 2) + _TILE_HEIGHT,
                36);
        }
        if ((key[KEY_RIGHT] != 0u) && x + 1 < TILES_PER_ROW) {
            rect(buffer, x * (_TILE_WIDTH + 2) - 1, y * (_TILE_HEIGHT + 2) - 1,
                x * (_TILE_WIDTH + 2) + _TILE_WIDTH, y * (_TILE_HEIGHT + 2) + _TILE_HEIGHT,
                36);
            x++;
            rect(buffer, x * (_TILE_WIDTH + 2) - 1, y * (_TILE_HEIGHT + 2) - 1,
                x * (_TILE_WIDTH + 2) + _TILE_WIDTH, y * (_TILE_HEIGHT + 2) + _TILE_HEIGHT,
                36);
        }
        if (key[KEY_DOWN] != 0u) {
            if (y >= TILES_PER_COL) {
                if ((row_y + y + 1) * TILES_PER_ROW < map->GetNumTiles()) {
                    row_y++;
                }
                DrawTileSelectionMap(row_y);
            } else {
                rect(buffer, x * (_TILE_WIDTH + 2) - 1, y * (_TILE_HEIGHT + 2) - 1,
                    x * (_TILE_WIDTH + 2) + _TILE_WIDTH, y * (_TILE_HEIGHT + 2) + _TILE_HEIGHT,
                    36);
                if ((x + (row_y + y + 1) * TILES_PER_ROW) < map->GetNumTiles()) {
                    y++;
                }
            }
            rect(buffer, x * (_TILE_WIDTH + 2) - 1, y * (_TILE_HEIGHT + 2) - 1,
                x * (_TILE_WIDTH + 2) + _TILE_WIDTH, y * (_TILE_HEIGHT + 2) + _TILE_HEIGHT,
                36);
        }
        if (key[KEY_UP] != 0u) {
            if ((y == 0) && (row_y != 0)) {
                row_y--;
                DrawTileSelectionMap(row_y);
            } else {
                rect(buffer, x * (_TILE_WIDTH + 2) - 1, y * (_TILE_HEIGHT + 2) - 1,
                    x * (_TILE_WIDTH + 2) + _TILE_WIDTH, y * (_TILE_HEIGHT + 2) + _TILE_HEIGHT,
                    36);
                if (y != 0) {
                    y--;
                }
            }
            rect(buffer, x * (_TILE_WIDTH + 2) - 1, y * (_TILE_HEIGHT + 2) - 1,
                x * (_TILE_WIDTH + 2) + _TILE_WIDTH, y * (_TILE_HEIGHT + 2) + _TILE_HEIGHT,
                36);
        }
        if (key[KEY_SPACE] != 0u) {
            if (mode == MODE_BACKGROUND) {
                cursor_x = (cursor_x * BG_TILE_WIDTH) / (_TILE_WIDTH - 1);
                cursor_y = (cursor_y * BG_TILE_HEIGHT) / _TILE_SIDE_HEIGHT;
            }
            mode = MODE_TILES;
            num = (x + (row_y + y) * TILES_PER_ROW);
            while (key[KEY_SPACE] != 0u) {
                ; //se till så att knappen är uppsläppt
            }
            break;
        }
        if (key[KEY_TAB] != 0u) {
            KEY_OFF(KEY_TAB);
            break;
        }
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    xor_mode(0);
    while (key[KEY_ESC] != 0u) {
        ;
    }
}
#undef TILES_PER_ROW
#undef TILES_PER_COL

void Editor::DrawBackgroundSelectionMap(int start_row)
{
    int n = 0;
    int x = 1;
    int y = 1;
    n = start_row * 2;
    //n = 0;

    clear(buffer);
    while (n < map->GetBGNumTiles()) {
        blit(map->GetBGTile(n), buffer, 0, 0, x, y, _BG_TILE_WIDTH, _BG_TILE_HEIGHT);
        x += _BG_TILE_WIDTH + 1;
        if (x + _BG_TILE_WIDTH > 320) {
            x = 1;
            y += _BG_TILE_HEIGHT + 2;
        }
        n++;
    }
}

void Editor::SelectBackground()
{
    static int x = 0;
    static int y = 0;
    static int start_row = 0;

    if (map->GetBGNumTiles() == 0) { //inga bakgrunder tillgängliga
        return;
    }

    DrawBackgroundSelectionMap(start_row);
    KEY_OFF(KEY_TAB);
    xor_mode(1);
    rect(buffer, x * (_BG_TILE_WIDTH + 1), y * (_BG_TILE_HEIGHT + 1),
        x * (_BG_TILE_WIDTH + 1) + _BG_TILE_WIDTH + 1,
        y * (_BG_TILE_HEIGHT + 1) + _BG_TILE_HEIGHT + 1,
        36);
    while (key[KEY_ESC] == 0u) {
        rest(100);
        if (key[KEY_LEFT] != 0u) {
            rect(buffer, x * (_BG_TILE_WIDTH + 1), y * (_BG_TILE_HEIGHT + 1),
                x * (_BG_TILE_WIDTH + 1) + _BG_TILE_WIDTH + 1,
                y * (_BG_TILE_HEIGHT + 1) + _BG_TILE_HEIGHT + 1,
                36);
            if (x != 0) {
                x--;
            }
            rect(buffer, x * (_BG_TILE_WIDTH + 1), y * (_BG_TILE_HEIGHT + 1),
                x * (_BG_TILE_WIDTH + 1) + _BG_TILE_WIDTH + 1,
                y * (_BG_TILE_HEIGHT + 1) + _BG_TILE_HEIGHT + 1,
                36);
        }
        if (key[KEY_RIGHT] != 0u) {
            rect(buffer, x * (_BG_TILE_WIDTH + 1), y * (_BG_TILE_HEIGHT + 1),
                x * (_BG_TILE_WIDTH + 1) + _BG_TILE_WIDTH + 1,
                y * (_BG_TILE_HEIGHT + 1) + _BG_TILE_HEIGHT + 1,
                36);
            if (x < 1) {
                x++;
            }
            rect(buffer, x * (_BG_TILE_WIDTH + 1), y * (_BG_TILE_HEIGHT + 1),
                x * (_BG_TILE_WIDTH + 1) + _BG_TILE_WIDTH + 1,
                y * (_BG_TILE_HEIGHT + 1) + _BG_TILE_HEIGHT + 1,
                36);
        }
        if (key[KEY_UP] != 0u) {
            rect(buffer, x * (_BG_TILE_WIDTH + 1), y * (_BG_TILE_HEIGHT + 1),
                x * (_BG_TILE_WIDTH + 1) + _BG_TILE_WIDTH + 1,
                y * (_BG_TILE_HEIGHT + 1) + _BG_TILE_HEIGHT + 1,
                36);
            if (y == 0 && start_row > 0) {
                start_row--;
                DrawBackgroundSelectionMap(start_row);
            } else if (y > 0) {
                y--;
            }
            rect(buffer, x * (_BG_TILE_WIDTH + 1), y * (_BG_TILE_HEIGHT + 1),
                x * (_BG_TILE_WIDTH + 1) + _BG_TILE_WIDTH + 1,
                y * (_BG_TILE_HEIGHT + 1) + _BG_TILE_HEIGHT + 1,
                36);
        }
        if (key[KEY_DOWN] != 0u) {
            rect(buffer, x * (_BG_TILE_WIDTH + 1), y * (_BG_TILE_HEIGHT + 1),
                x * (_BG_TILE_WIDTH + 1) + _BG_TILE_WIDTH + 1,
                y * (_BG_TILE_HEIGHT + 1) + _BG_TILE_HEIGHT + 1,
                36);
            if (y == 1) {
                start_row++;
                DrawBackgroundSelectionMap(start_row);
            } else {
                y++;
            }
            rect(buffer, x * (_BG_TILE_WIDTH + 1), y * (_BG_TILE_HEIGHT + 1),
                x * (_BG_TILE_WIDTH + 1) + _BG_TILE_WIDTH + 1,
                y * (_BG_TILE_HEIGHT + 1) + _BG_TILE_HEIGHT + 1,
                36);
        }
        if (key[KEY_SPACE] != 0u) {
            num = x + (y + start_row) * 2;
            if (num > map->GetBGNumTiles()) {
                num = map->GetBGNumTiles();
            }
            if (mode != MODE_BACKGROUND) {
                cursor_x = (cursor_x * (TILE_WIDTH - 1)) / BG_TILE_WIDTH;
                cursor_y = (cursor_y * TILE_SIDE_HEIGHT) / BG_TILE_HEIGHT;
            }
            mode = MODE_BACKGROUND;
            KEY_OFF(KEY_SPACE);
            break;
        }
        if (key[KEY_TAB] != 0u) {
            KEY_OFF(KEY_TAB);
            break;
        }
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    xor_mode(0);
    KEY_OFF(KEY_ESC);
}

void Editor::DrawObjectSelectionMap(int start_row)
{
    int i = start_row;
    int y = 0;
    clear(buffer);
    while (*Objects[i].filename != 0) {
        blit(images[i], buffer, 0, 0, (SCREEN_WIDTH - images[i]->w) >> 1, y, images[i]->w, images[i]->h);
        y += images[i]->h + 1;
        i++;
    }
}

void Editor::SelectObject()
{
    int i = 0;
    static int y = 0;

    xor_mode(1);
    DrawObjectSelectionMap(y);
    rect(buffer, (SCREEN_WIDTH - images[y]->w) >> 1, 0,
        (SCREEN_WIDTH + images[y]->w) >> 1, images[y]->h,
        36);
    textprintf(buffer, font, 0, 0, 36, "%s", Objects[y].description);
    KEY_OFF(KEY_TAB);
    while (key[KEY_ESC] == 0u) {
        if ((key[KEY_RSHIFT] != 0u) || (key[KEY_LSHIFT] != 0u)) { //speeda upp scrollningen
            rest(10);
        } else {
            rest(100);
        }
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        if ((key[KEY_DOWN] != 0u) && (*Objects[y + 1].filename != 0)) {
            y++;
            DrawObjectSelectionMap(y);
            rect(buffer, (SCREEN_WIDTH - images[y]->w) >> 1, 0,
                (SCREEN_WIDTH + images[y]->w) >> 1, images[y]->h,
                36);
            textprintf(buffer, font, 0, 0, 36, "%s", Objects[y].description);
        }
        if ((key[KEY_UP] != 0u) && (y != 0)) {
            y--;
            DrawObjectSelectionMap(y);
            rect(buffer, (SCREEN_WIDTH - images[y]->w) >> 1, 0,
                (SCREEN_WIDTH + images[y]->w) >> 1, images[y]->h,
                36);
            textprintf(buffer, font, 0, 0, 36, "%s", Objects[y].description);
        }
        if (key[KEY_SPACE] != 0u) {
            num = y;
            if (mode == MODE_BACKGROUND) {
                cursor_x = (cursor_x * BG_TILE_WIDTH) / (TILE_WIDTH - 1);
                cursor_y = (cursor_y * BG_TILE_HEIGHT) / TILE_SIDE_HEIGHT;
            }
            mode = MODE_OBJECTS;
            while (key[KEY_SPACE] != 0u) {
                ;
            }
            break;
        }
        if (key[KEY_TAB] != 0u) {
            KEY_OFF(KEY_TAB);
            break;
        }
    }
    while (key[KEY_ESC] != 0u) {
        ;
    }
}

void Editor::ShowMapStatus()
{
    int i = 0;
    int j = 0;
    int k = 0;
    int temp = 0;
    int n_trans_tiles = 0;
    int n_tiles = 0;
    int n_objects = 0;
    int n_players = 0;
    int n_doors = 0;
    int n_checkp = 0;
    int n_exp_tiles = 0;
    int n_mines = 0;
    int n_boss = 0;
    int bg_exist = 0;
    int bg_warning = 0;

    clear(buffer);
    textprintf(buffer, font, 0, 0, 36, "Collecting data...");
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    for (k = 0; k < 3; k++) {
        for (j = 0; j < MAP_HEIGHT; j++) {
            for (i = 0; i < MAP_WIDTH; i++) {
                if ((temp = map->GetBG(i, j, k)) != 0) {
                    if (IS_TRANSPARENT(temp)) {
                        n_trans_tiles++;
                    } else {
                        n_tiles++;
                    }
                }
                if ((temp = map->GetObj(i, j, k)) != 0) {
                    n_objects++;
                    if (temp == TYPE_PLAYER) {
                        n_players++;
                    } else if (temp == TYPE_DOOR || temp == TYPE_RED_DOOR || temp == TYPE_BLUE_DOOR || temp == TYPE_GREEN_DOOR) {
                        n_doors++;
                    } else if (temp == TYPE_CHECKPOINT) {
                        n_checkp++;
                    } else if (temp == TYPE_BARREL) {
                        n_exp_tiles++;
                    } else if (temp == TYPE_MINE) {
                        n_mines++;
                    } else if (IS_BOSS(temp)) {
                        /*					else if (temp == TYPE_BOSS_SCIENTIST || temp == TYPE_BOSS_SOLDIER ||
                                                temp == TYPE_BOSS_RAMBO || temp == TYPE_BOSS_ARNOLD || temp == TYPE_BOSS_DOLPH ||
						temp == TYPE_BOSS_EINSTEIN || temp == TYPE_BOSS_CRYCHECK || temp == TYPE_BOSS_DR_GREEN)*/
                        n_boss++;
                    }
                }
            }
        }
    }
    clear(buffer);
    textprintf(buffer, font, 0, 0, 36, "Filename: %s [%dx%d]", filename, MAP_WIDTH, MAP_HEIGHT);
    textprintf(buffer, font, 0, 15, 36, "Num Tiles: %d", n_tiles);
    textprintf(buffer, font, 0, 30, 36, "Num Transparent Tiles: %d", n_trans_tiles);
    textprintf(buffer, font, 0, 45, 36, "Total Tiles: %d", n_tiles + n_trans_tiles);
    textprintf(buffer, font, 0, 60, 36, "Num Explosive Tiles: %d", n_exp_tiles);
    textprintf(buffer, font, 0, 75, 36, "Num Objects: %d", n_objects);
    textprintf(buffer, font, 0, 90, 36, "Num Players: %d [%s]", n_players, n_players != 1 ? "WARNING!" : "ok");
    textprintf(buffer, font, 0, 105, 36, "Num Doors: %d [%s]", n_doors, n_doors != 1 ? "WARNING!" : "ok");
    textprintf(buffer, font, 0, 120, 36, "Num Checkpoints: %d", n_checkp);
    textprintf(buffer, font, 0, 135, 36, "Num Mines: %d", n_mines);
    textprintf(buffer, font, 0, 150, 36, "Num Bosses: %d [%s]", n_boss, n_boss != 1 ? "WARNING!" : "ok");
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    while (key[KEY_ESC] == 0u) {
        ;
    }
    KEY_OFF(KEY_ESC);
}

/**************************************************************************/

auto Editor::EditLevel(char* filename, int xs, int ys) -> int
{
    unsigned long temp = 0;
    /*	if (!exists(filename)) {
        // new level
        	return 2;
        } else */
    if (map->LoadMap(filename) == 0) { //kunde inte hitta filen
        if (map->LoadMap("gamemaps/level11.map") == 0) { //ladda in tiles
            return 3;
        }
        map->NewLevel(xs, ys); //skapa en ny
    }
    Editor::filename = filename;

    set_palette(map->GetPal());

    //        draw_level(map_x, map_y, layers, objects);
    DrawScreen();
    while (key[KEY_ESC] == 0u) {
        rest(50);
        if ((key[KEY_PGDN] != 0u) && cursor_z < 2) {
            cursor_z++;
        }
        if ((key[KEY_PGUP] != 0u) && (cursor_z != 0)) {
            cursor_z--;
        }

        if (key[KEY_DOWN] != 0u) {
            if (cursor_y >= _GET_Y_TH - 1) {
                if (((map_y + _SEL_Y_T_B) / TILE_SIDE_HEIGHT) < (MAP_HEIGHT - NUM_Y_TILES - 2)) {
                    map_y += _SEL_Y_T_B;
                } else if (mode == MODE_BACKGROUND) {
                    //                                	map_y = (MAP_HEIGHT - NUM_Y_TILES) * TILE_SIDE_HEIGHT;
                    //map_y = MAP_HEIGHT - NUM_Y_TILES - 2
                    //map_y = (MAP_HEIGHT - NUM_Y_TILES - 2) * TILE_SIDE_HEIGHT;
                    //                                        map_y = (MAP_HEIGHT - NUM_Y_TILES - 2) * TILE_SIDE_HEIGHT;
                    //                                	cursor_y++;
                }
            } else {
                cursor_y++;
            }
        }
        if (key[KEY_UP] != 0u) {
            if (cursor_y == 0) {
                map_y -= _SEL_Y_T_B;
                if (map_y < 0) {
                    map_y = 0;
                }
            } else {
                cursor_y--;
            }
        }
        if (key[KEY_RIGHT] != 0u) {
            if (cursor_x >= _GET_X_TH - 1) {
                if (((map_x + _SEL_X_T_B) / TILE_WIDTH) < (MAP_WIDTH - NUM_X_TILES - 4)) {
                    map_x += _SEL_X_T_B;
                }
            } else {
                cursor_x++;
            }
        }
        if (key[KEY_LEFT] != 0u) {
            if (cursor_x == 0) {
                map_x -= _SEL_X_T_B;
                if (map_x < 0) {
                    map_x = 0;
                }
            } else if (cursor_x > 0) {
                cursor_x--;
            }
        }
        if (key[KEY_TAB] != 0u) {
            switch (mode) {
            case MODE_TILES:
                SelectTile();
                break;
            case MODE_OBJECTS:
                SelectObject();
                break;
            case MODE_BACKGROUND:
                SelectBackground();
                break;
            case MODE_EFFECTS:
            case MODE_DEKORATIONS:
            default:
                break;
            }
        }
        if (key[KEY_HOME] != 0u) {
            map_x = 0;
        }
        if (key[KEY_F1] != 0u) {
            SelectTile();
        }

        if (key[KEY_F2] != 0u) {
            SelectBackground();
        }

        if (key[KEY_F3] != 0u) {
            SelectObject();
        }
        if (key[KEY_F6] != 0u) {
            draw_zoomed_out ^= 1;
            KEY_OFF(KEY_F6);
        }
        /*if (key[KEY_F4]) {
                	SelectEffect();
                }
                if (key[KEY_F5]) {
                	SelectDekoration();
                }
                */
        if (key[KEY_F8] != 0u) {
            ShowMapStatus();
        }

        if ((key[KEY_F12] != 0u) && (filename != nullptr)) {
            map->SaveMap(filename);
        }

        if (key[KEY_SPACE] != 0u) {
            switch (mode) {
            case MODE_TILES:
                map->SetBG(cursor_x + (map_x / (TILE_WIDTH - 1)), cursor_y + (map_y / TILE_SIDE_HEIGHT), cursor_z, num + 1);
                break;
            case MODE_OBJECTS:
                map->SetObj(cursor_x + (map_x / (TILE_WIDTH - 1)), cursor_y + (map_y / TILE_SIDE_HEIGHT), cursor_z, Objects[num].num);
                break;
            case MODE_BACKGROUND:
                map->SetBackGround(cursor_x + (map_x / BG_TILE_WIDTH), cursor_y + (map_y / BG_TILE_HEIGHT), num + 1);
                break;
            case MODE_EFFECTS:
            case MODE_DEKORATIONS:
            default:
                break;
            }
        }
        if (key[KEY_DEL] != 0u) {
            switch (mode) {
            case MODE_TILES:
                map->SetBG(cursor_x + (map_x / (TILE_WIDTH - 1)), cursor_y + (map_y / TILE_SIDE_HEIGHT), cursor_z, 0);
                break;
            case MODE_OBJECTS:
                map->SetObj(cursor_x + (map_x / (TILE_WIDTH - 1)), cursor_y + (map_y / TILE_SIDE_HEIGHT), cursor_z, 0);
                break;
            case MODE_BACKGROUND:
                map->SetBackGround(cursor_x + (map_x / BG_TILE_WIDTH), cursor_y + (map_y / BG_TILE_HEIGHT), 0);
                break;
            case MODE_EFFECTS:
            case MODE_DEKORATIONS:
            default:
                break;
            }
        }
        if (key[KEY_PLUS_PAD] != 0u) {
            if (draw_zoomed_out != 0) {
                draw_scale++;
                if (draw_scale > MAX_ZOOM_LEVEL) {
                    draw_scale = MAX_ZOOM_LEVEL;
                }
            } else {
                num++;
                switch (mode) {
                case MODE_BACKGROUND:
                    num %= map->GetBGNumTiles();
                    break;
                case MODE_TILES:
                    num %= map->GetNumTiles();
                    break;
                case MODE_OBJECTS:
                    num %= num_objects;
                    break;
                }
            }
        }
        if (key[KEY_MINUS_PAD] != 0u) {
            if (draw_zoomed_out != 0) {
                draw_scale--;
                if (draw_scale <= 0) {
                    draw_scale = 1;
                }
            } else {
                num--;
                if (num < 0) {
                    switch (mode) {
                    case MODE_BACKGROUND:
                        num = map->GetBGNumTiles() - 1;
                        break;
                    case MODE_TILES:
                        num = map->GetNumTiles() - 1;
                        break;
                    case MODE_OBJECTS:
                        num = num_objects - 1;
                        break;
                    }
                }
            }
            //                        	num = map->GetNumTiles() - 1;
        }
        if (key[KEY_1] != 0u) { //tile lager 1
            layer_info[0].tiles_on ^= 1;
            KEY_OFF(KEY_1);
        }
        if (key[KEY_2] != 0u) { //tile lager 2
            layer_info[1].tiles_on ^= 1;
            KEY_OFF(KEY_2);
        }
        if (key[KEY_3] != 0u) { //tile lager 3
            layer_info[2].tiles_on ^= 1;
            KEY_OFF(KEY_3);
        }
        if (key[KEY_4] != 0u) { //alla tile lager
            layer_info[0].tiles_on ^= 1;
            layer_info[1].tiles_on ^= 1;
            layer_info[2].tiles_on ^= 1;
            KEY_OFF(KEY_4);
        }
        if (key[KEY_5] != 0u) { //object lager 0
            layer_info[0].objects_on ^= 1;
            KEY_OFF(KEY_5);
        }
        if (key[KEY_6] != 0u) { //object lager 1
            layer_info[1].objects_on ^= 1;
            KEY_OFF(KEY_6);
        }
        if (key[KEY_7] != 0u) { //object lager 2
            layer_info[2].objects_on ^= 1;
            KEY_OFF(KEY_7);
        }
        if (key[KEY_8] != 0u) { //alla object
            layer_info[0].objects_on ^= 1;
            layer_info[1].objects_on ^= 1;
            layer_info[2].objects_on ^= 1;
            KEY_OFF(KEY_8);
        }
        DrawScreen();
    };
    return 0;
}

/**************************************************************************/

void Editor::DrawCursor(BITMAP* buffer)
{
    int temp = 0;
    textprintf(buffer, font, 0, 0, 36, "X: %ld/%d", cursor_x + map_x / (TILE_WIDTH - 1), MAP_WIDTH);
    textprintf(buffer, font, 0, 15, 36, "Y: %ld/%d", cursor_y + map_y / TILE_SIDE_HEIGHT, MAP_HEIGHT);
    textprintf(buffer, font, 0, 30, 36, "Z: %d", cursor_z);

    if (mode == MODE_BACKGROUND) {
        xor_mode(1);
        rect(buffer, cursor_x * BG_TILE_WIDTH, cursor_y * BG_TILE_HEIGHT,
            (cursor_x + 1) * BG_TILE_WIDTH, (cursor_y + 1) * BG_TILE_HEIGHT, 36);
        line(buffer, cursor_x * BG_TILE_WIDTH, cursor_y * BG_TILE_HEIGHT,
            (cursor_x + 1) * BG_TILE_WIDTH, (cursor_y + 1) * BG_TILE_HEIGHT, 36);
        line(buffer, (cursor_x + 1) * BG_TILE_WIDTH, cursor_y * BG_TILE_HEIGHT,
            cursor_x * BG_TILE_WIDTH, (cursor_y + 1) * BG_TILE_HEIGHT, 36);

        xor_mode(0);
    } else {
        /* Ritar ut markören */
        int rect_xstart = cursor_x * (TILE_WIDTH - 1);
        int rect_ystart = cursor_y * TILE_SIDE_HEIGHT + (TILE_TOP_HEIGHT - 1) * cursor_z;

        xor_mode(1);
        rect(buffer, rect_xstart, rect_ystart,
            rect_xstart + TILE_WIDTH - 1,
            rect_ystart + TILE_TOP_HEIGHT, 36);
        rect(buffer, rect_xstart, rect_ystart + TILE_SIDE_HEIGHT,
            rect_xstart + TILE_WIDTH - 1,

            rect_ystart + TILE_TOP_HEIGHT + TILE_SIDE_HEIGHT - 1, 36);
        line(buffer, rect_xstart, rect_ystart + TILE_TOP_HEIGHT + 1,
            rect_xstart, rect_ystart + TILE_SIDE_HEIGHT - 1, 36);
        line(buffer, rect_xstart + TILE_WIDTH - 1, rect_ystart + TILE_TOP_HEIGHT + 1, rect_xstart + TILE_WIDTH - 1, rect_ystart + TILE_SIDE_HEIGHT - 1, 36);

        switch (mode) {
        case MODE_TILES:
            blit(map->GetTile(num), buffer,
                0, 0, 300 - _TILE_WIDTH,
                0, _TILE_WIDTH, _TILE_HEIGHT);
            break;
        case MODE_OBJECTS:
            masked_blit(images[num], buffer, 0, 0,
                300 - images[num]->w, 0,
                images[num]->w, images[num]->h);
#define CX (cursor_x + (map_x / (TILE_WIDTH - 1)))
#define CY (cursor_y + (map_y / TILE_SIDE_HEIGHT))
#define CZ (cursor_z)
            if ((temp = map->GetObj(CX, CY, CZ)) != 0) {
                temp = Obj2Num(temp);
                xor_mode(0);
                line(buffer, 0, cursor_y * (TILE_SIDE_HEIGHT) + (TILE_TOP_HEIGHT - 1) * cursor_z - (images[temp]->h >> 1),
                    SCREEN_WIDTH, cursor_y * (TILE_SIDE_HEIGHT) + (TILE_TOP_HEIGHT - 1) * cursor_z - (images[temp]->h >> 1), 36);
                line(buffer, cursor_x * (TILE_WIDTH - 1), 0,
                    cursor_x * (TILE_WIDTH - 1), SCREEN_HEIGHT, 36);
                xor_mode(1);
            }
            break;
        default:
            break;
        }
    }
#undef CX
#undef CY
#undef CZ

    //        textprintf(buffer, font, rect_xstart + TILE_WIDTH + 1,
    //        rect_ystart, 36, "%d", cursor_z);
    xor_mode(0);
}

/**************************************************************************/

auto Editor::Obj2Num(int o) -> int
{
    int i = 0;
    while ((*Objects[i].filename != 0) && Objects[i].num != o) {
        i++;
    }

    if (Objects[i].num == o) {
        return i;
    }

    return 0;
}
// draw_level: Ritar upp kartan på skärmen, med start på start_x och start_y.

void Editor::DrawScreen()
{
    int i = 0;
    int j = 0;
    int k = 0;
    int bgstartx = map_x / BG_TILE_WIDTH;
    int bgstarty = map_y / BG_TILE_HEIGHT;
    int bgoffsetx = (bgstartx * BG_TILE_WIDTH) - map_x;
    int bgoffsety = (bgstarty * BG_TILE_HEIGHT) - map_y;
    int kalle3 = 0;
    int temp = 0;
    int ki = 0;
    int numosc = 0;
    int tx = 0 = 0;
    int ty;
    int slut_x = 0;
    = 0 int slut_y;
    int scale = 0;
    //	BITMAP *buffer = screen;

    /*	if (draw_zoomed_out)
        	scale = draw_scale;
	else
        	scale = 1;*/
    scale = 1;
    int startx = map_x / ((TILE_WIDTH / scale) - 1);
    int starty = map_y / (TILE_SIDE_HEIGHT / scale);
    int offsetx = (startx * (TILE_WIDTH / scale)) - map_x;
    int offsety = (starty * TILE_SIDE_HEIGHT / scale) - map_y;

    clear(buffer);

    //	clear_to_color (buffer, 1);
    for (i = bgstartx; i < (bgstartx + draw_scale * 4); i++) {
        for (j = (bgstarty + draw_scale * 3); j >= bgstarty; j--) {
            if ((kalle3 = map->GetBackGround(i, j)) != 0) {
                stretch_sprite(buffer, map->GetBGTile(kalle3 - 1),
                    (i - bgstartx) * BG_TILE_WIDTH + bgoffsetx,
                    (j - bgstarty) * BG_TILE_HEIGHT + bgoffsety,
                    BG_TILE_WIDTH, BG_TILE_HEIGHT);
                /*				blit(map->GetBGTile(kalle3 - 1), buffer,
					0, 0, (i - bgstartx) * BG_TILE_WIDTH + bgoffsetx,
					(j - bgstarty) * BG_TILE_HEIGHT + bgoffsety, BG_TILE_WIDTH, BG_TILE_HEIGHT);*/
                //				background_availible = 1;
            }
        }
    }

    for (k = 0; k < 3; k++) {
        /*		if (key[KEY_F1] && k == 0)
			continue;
		if (key[KEY_F2] && k == 1)
			continue;
		if (key[KEY_F3] && k == 2)
			continue;*/
        ki = (TILE_TOP_HEIGHT / scale) * k - k;
        if (layer_info[k].tiles_on != 0) {
            for (i = startx; i < (startx + draw_scale * 14); i++) {
                for (j = (starty + draw_scale * 9); j >= starty; j--) {
                    if (i > MAP_WIDTH - 1) {
                        continue;
                    }
                    if (j > MAP_HEIGHT - 1) {
                        continue;
                    }
                    if ((kalle3 = map->GetBG(i, j, k)) != 0) {
                        stretch_sprite(buffer, map->GetTile(kalle3 - 1),
                            (i - startx) * (TILE_WIDTH / scale) + offsetx - i,
                            (j - starty) * (TILE_SIDE_HEIGHT / scale) + offsety + ki,
                            TILE_WIDTH / scale, TILE_HEIGHT / scale);
                    }
                }
            }
            /*							masked_blit(map->GetTile(kalle3 - 1), buffer,
								0, 0, (i - startx) * TILE_WIDTH + offsetx - i,
								(j - starty) * TILE_SIDE_HEIGHT + offsety + ki, TILE_WIDTH, TILE_HEIGHT);
							stretch_blit(map->GetTile(kalle3 - 1), buffer,
					                  	0, 0, TILE_WIDTH, TILE_HEIGHT,
								(i - startx) * TILE_WIDTH + offsetx - i,
								(j - starty) * TILE_SIDE_HEIGHT + offsety + ki,
                                                                TILE_WIDTH / scale, TILE_HEIGHT / scale);*/
            //					                  int dest_x, dest_y, dest_width, dest_height);
            /*							blit(map->GetTile(kalle3 - 1), buffer,
								0, 0, (i - startx) * TILE_WIDTH + offsetx - i,
								(j - starty) * TILE_SIDE_HEIGHT + offsety + ki, TILE_WIDTH, TILE_HEIGHT);*/
            //					}
        }
        if (layer_info[k].objects_on != 0) {
            for (i = startx; i < (startx + draw_scale * 14); i++) {
                for (j = (starty + draw_scale * 9); j >= starty; j--) {
                    if (i > MAP_WIDTH - 1) {
                        continue;
                    }
                    if (j > MAP_HEIGHT - 1) {
                        continue;
                    }
                    if ((kalle3 = map->GetObj(i, j, k)) != 0) {
                        kalle3 = Obj2Num(kalle3);
                        stretch_sprite(buffer, images[kalle3],
                            (tx = (i - startx) * TILE_WIDTH + offsetx - i - ((images[kalle3]->w / draw_scale) >> 1)),
                            (ty = (j - starty) * TILE_SIDE_HEIGHT + offsety + ki + (TILE_TOP_HEIGHT >> 1) - (images[kalle3]->h / draw_scale)),
                            images[kalle3]->w / draw_scale, images[kalle3]->h / draw_scale);
                        /*               	        	                masked_blit(images[kalle3], buffer, 0, 0,
							(tx = (i - startx) * TILE_WIDTH + offsetx - (images[kalle3]->w >> 1) - i),
      	                	                        (ty = (j - starty) * TILE_SIDE_HEIGHT + offsety + ki + (TILE_TOP_HEIGHT >> 1) - images[kalle3]->h),
       	                        	                images[kalle3]->w, images[kalle3]->h);*/
                        //						xor_mode(1);
                        if (draw_scale == 1) {
                            if (IS_BOSS(kalle3)) {
                                textprintf(buffer, font, tx, ty, 36, "%d%c", k, 'B');
                            } else {
                                textprintf(buffer, font, tx, ty, 36, "%d", k);
                            }
                        }
                        //       	                                        xor_mode(0);
                    }
                }
            }
        }
    }

    /*	textprintf(buffer, font, 200, 30, 15, "Objects %d", num_objects);
	textprintf(buffer, font, 200, 45, 15, "Decor %d", (ringbuffer) ? MAX_DEKORATION_OBJECTS : num_dekorations);
	textprintf(buffer, font, 200, 60, 15, "Effect %d", num_effects);
	textprintf(buffer, font, 200, 75, 15, "Numobjs %d", numosc);
	textprintf(buffer, font, 0, 45, 15, "Frames/s %ld", n);*/
    DrawCursor(buffer);
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}
