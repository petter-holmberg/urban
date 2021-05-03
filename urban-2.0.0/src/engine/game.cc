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
#include <stdio.h>
#include <allegro.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "engine.h"
#include "outtro.h"
#include "ctrls.h"
#include "urbfont.h"
#include "icache.h"
#include "highscor.h"
#include "game.h"
#ifndef DJGPP
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <unistd.h>
#endif
/**************************************************************************/
// Global ImageCache
ImageCache icache;
// Credits
void showcredits();
/**************************************************************************/
Engine *engine = 0;
unsigned long _flags = 0xffffffff;

extern char lock_frame_count_to_60hz;

struct SaveGameData SavedGames[5];

const char *maps[] = {	"gamemaps/level11.map",
			"gamemaps/level12.map",
			"gamemaps/level13.map",
			"gamemaps/level21.map",
			"gamemaps/level22.map",
			"gamemaps/level23.map",
			"gamemaps/level31.map",
			"gamemaps/level32.map",
			"gamemaps/level33.map",
			"gamemaps/level41.map",
			"gamemaps/level42.map",
			"gamemaps/level43.map",
			"gamemaps/level51.map",
			"gamemaps/level52.map",
			"gamemaps/level53.map",
			"gamemaps/level61.map",
			"gamemaps/level62.map",
			"gamemaps/level63.map", NULL};

const char *extra_maps[] = {	"",
			"",
                        "",
                        "",
                        "",
                        "", NULL};

//#define	NUM_LEVELS 1
#define	NUM_LEVELS 18

int MaxLevelNum	= 1;

char *demo_filename;

struct PlayerData DefaultPData = {
	{3},
	{-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {300}, {0}
};


int GetLevelName(char *text);
/**************************************************************************/
int SaveGame(char *name) {
	char buffer[80];
        int pos;

        pos = Do_Menu("CONTINUE\nSAVE GAME", 2);

	if (pos != 2)
        	return 0;

        buffer[0] = 0;

        for(int i = 0;i < 5;i++) {
        	if(SavedGames[i].level) {
                        strcat(buffer, SavedGames[i].name);
                        strcat(buffer, "\n");
                } else
                	strcat(buffer, "EMPTY\n");

        }

        if((pos = Do_Menu(buffer, 5))) {
        	if(SavedGames[pos - 1].level)
                	strcpy(name, SavedGames[pos - 1].name);

                if(GetLevelName(name))
                	return pos;
		else
                	return 0;
	} else
	        return 0;
}

/**************************************************************************/
void display_level_info(int level) {
	char message[256];
	PALETTE Pal;
        UrbanFont fnt(LARGE_FONT);


	BITMAP *bmp = icache.GetImage ("ibild.pcx", Pal);
       	blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);
        icache.FreeImage(bmp);
	if(level == -1)
		sprintf(message, "GAME OVER");
	else if(level >	NUM_LEVELS) {

	        {
			// Run Outtro
	        	Outtro *outtro = new Outtro;

		        outtro->RunOuttro();
	        	delete outtro;

                        // Show credits
			showcredits();

		}
/*	        fnt.SetScale(65);
		sprintf(message, "MISSION");
		fnt.print_centre(message, 320 / 2, 190 / 2);
		sprintf(message, "COMPLETED");*/
	} else
		return;

	fnt.print_centre(message, 320 / 2, 240 / 2);
	fade_in(Pal, 2);
        rest(2000);
	clear_keybuf();
}
/**************************************************************************/
int GetLevelName(char *text) {
	char message[50];
	int key, pos = 1;
        UrbanFont fnt(SMALL_FONT2);

        if(strlen(text) == 0)
	        strcpy(text, "MYLEVEL");

        // Ritar ett rutnär
        for(int x = 0;x < 320; x+=2) {
	        for(int y = 0;y < 240; y+=2) {

			putpixel(screen, x, y, 0);
			putpixel(screen, x + 1, y + 1, 0);
		}
	}

	pos = strlen(text);

	BITMAP *textbmp	= create_bitmap(220, 55);

	clear_keybuf();

	sprintf(message, "Slot Name");

	do {
		rectfill(screen, 45, 55, 320 - 45, 240 - 120, 27);
		rectfill(screen, 50, 60, 320 - 50, 240 - 125, 25);

		clear(textbmp);
		fnt.print_centre(message, 110,  20, textbmp);
		fnt.print_centre(text, 110,  40, textbmp);

	       	masked_blit(textbmp, screen, 0,	0, 50, 60, 220,	55);

		key = readkey();

		if((key	>> 8) == KEY_ENTER) {

			text[pos] = 0;
/*                        strupr(text);*/
			return 1;
		}
		if((key	>> 8) == KEY_ESC) {

			destroy_bitmap(textbmp);
			return 0;
		}
		if((key	>> 8) == KEY_BACKSPACE) {
			if(pos == 0)
				continue;
			pos--;
			text[pos] = 0;
		}
		if((key	>> 8) == KEY_SPACE) {
			if(pos > 9)
				continue;
			text[pos++] = ' ';
		}
		else if(pos < 9){

			if(!((key & 0xff) < 'A'	|| (key	& 0xff)	> 'z') || ((key & 0xff) == ' '))	{

				text[pos++] = toupper((key & 0xff));
				text[pos] = 0;
			}
		}
	} while	(1);

	destroy_bitmap(textbmp);
	return 0;
}
/**************************************************************************/
int NewGame(int slot) {
	int quit = 0;
	HighScore *hs;
	const char *map_name = NULL;
        char name[80];
        char filename[1024];
	struct PlayerData pdat;
        int level = 1;

        FILE *fs;

	ENGINE.NewGame();

	display_level_info(level);

#ifdef DJGPP
	sprintf(filename, "savegame.dat");
#else
	sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));
#endif

        if ((fs = fopen(filename, "rb")) == NULL) {

        	for (int i = 0; i < 5;i++) {
                	SavedGames[i].level = 0;
                	SavedGames[i].name[0] = 0;
			memcpy(&SavedGames[i].dat, &DefaultPData, sizeof(struct PlayerData));
                }
#ifndef DJGPP
		/* Create dir */
		sprintf(filename, "%s/.urban", getenv("HOME"));
		mkdir(filename, S_IRUSR | S_IWUSR | S_IXUSR);

		sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));
#endif
                if((fs = fopen(filename, "wb")) != NULL)
	        	fwrite(SavedGames, 1, 5 * sizeof(struct SaveGameData), fs);
        } else
	        auto err = fread(SavedGames, 1, 5 * sizeof(struct SaveGameData), fs);

	if(fs)
	        fclose(fs);

        if (slot == 0 || SavedGames[slot - 1].level == 0) {
	        memcpy(&pdat, &DefaultPData, sizeof(struct PlayerData));
	} else {
        	memcpy(&pdat, &SavedGames[slot - 1].dat, sizeof(struct PlayerData));
                level = SavedGames[slot - 1].level;
	}

        map_name = maps[level - 1];

       	while(!quit) {
		switch(ENGINE.play_level(map_name, &pdat)) {
			case -1: // Game over
			        SOUND.PlaySFX("samples/haha.wav");

				quit = 1;
				display_level_info(-1);
				hs = new HighScore(ENGINE.score.GetScore(), level);
				delete hs;

				break;

			case 0:	// ESC pressed
				fade_out(5);
				quit = 1;
				break;
			case 2: //kör extrabana
                                fade_out(5);
                                map_name = extra_maps[level - 1];
                                if (*map_name != 0)
					break;
			case 1: // Level Clear
				level++;
                                fade_out(5);
				display_level_info(level);
				if(level > NUM_LEVELS) {
					quit = 1;
					level--;
					hs = new HighScore(ENGINE.score.GetScore(), level);
					delete hs;
				} else {
                                	slot = SaveGame(name);
                                        if (slot) {
				        	memcpy(&SavedGames[slot - 1].dat, &pdat, sizeof(struct PlayerData));
				                SavedGames[slot - 1].level = level;
				                strcpy(SavedGames[slot - 1].name, name);

#ifdef DJGPP
						sprintf(filename, "savegame.dat");
#else
						/* Create dir */
						sprintf(filename, "%s/.urban", getenv("HOME"));
						mkdir(filename, S_IRUSR | S_IWUSR | S_IXUSR);

						sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));
#endif
					        if ((fs = fopen(filename, "wb")) != NULL) {
					        	auto err = fwrite(SavedGames, 1, 5 * sizeof(struct SaveGameData), fs);
                                                        fclose(fs);
						}
					}
				}

				map_name = maps[level - 1];
				break;

		};

	};
	return 1;
}

