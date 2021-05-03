/******************************************************************************

    URBAN
    Copyright (C) 1999  Thomas Nyberg and Jonas Borgstr�m

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
    Thomas Nyberg                                       Jonas Borgstr�m
    Br�nnbergsv�gen 21b                                 Bondev�gen 6
    92136 Lycksele                                      92332 Storuman
    Sweden                                              Sweden

    or email:

    thomas.nyberg@usa.net                               jonas_b@bitsmart.com
*****************************************************************************/
#include <stdio.h>
#include <allegro.h>
#include <unistd.h>
#include <string.h>
#ifndef DJGPP
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <unistd.h>
void close_gfx();
#endif

#include "urbfont.h"
#include "engine.h"
#include "icache.h"
#include "game.h"
#include "highscor.h"
#include "intro.h"
#include "config.h"

#define MENU_MUSIC "snd/modules/nole.xm"
#define CREDITS_MUSIC "snd/modules/extrem.xm"
#define	MAXPOS 	6
#define	MINPOS 	1
#define	FONT_H	26
#define MENY_POS_X 60
#define MENY_POS_Y 85

/* From credits.cc */
void showcredits();
/*****************************************************************************************/
Config *config = NULL;
void LoadGame();
/*****************************************************************************************/
int MenuChoice(int alternative){
	HighScore *hs;

	switch (alternative) {
	       	case 1:
			NewGame(0);

			if (IS_SOUND_ON) {
				char buffer[1024];

			       	sprintf(buffer, "%s/%s", DATPATH, MENU_MUSIC);
				SOUND.PlayMusic(buffer);
			}
			break;

		case 2:
                	LoadGame();

			if (IS_SOUND_ON) {
				char buffer[1024];

			       	sprintf(buffer, "%s/%s", DATPATH, MENU_MUSIC);
				SOUND.PlayMusic(buffer);
			}
			clear_keybuf();
		        break;

		case 3:
			hs = new HighScore();
			readkey();
			delete hs;
			break;

		case 4:
			if (IS_SOUND_ON) {
				char buffer[1024];

			       	sprintf(buffer, "%s/%s", DATPATH, CREDITS_MUSIC);
				SOUND.PlayMusic(buffer);
			}
			fade_out(5);
		
			showcredits();

			if (IS_SOUND_ON) {
				char buffer[1024];

			       	sprintf(buffer, "%s/%s", DATPATH, MENU_MUSIC);
				SOUND.PlayMusic(buffer);
			}
                        break;

		case 5:
                	config->Start();
                        break;

		case 6:
			return 0;
	}
	return 1;
}
/*****************************************************************************************/
int Do_Menu(char *text, int num_items, int pos = 1) {
	PALETTE pal;
	UrbanFont m(LARGE_FONT);
        m.SetScale(65);
//	Controllers *ctrl;

	BITMAP *choice = icache.GetImage("pekare.pcx", pal);
        BITMAP *backg = icache.GetImage("ibild.pcx", pal);
        set_palette(pal);
        BITMAP *textbmp = m.print(text);
        int text_x = (SCREEN_WIDTH - textbmp->w) / 2;
        int text_y = (SCREEN_HEIGHT - textbmp->h) / 2;
        int looping = 1;

	blit(backg, screen, 0, 0, 0, 0, backg->w, backg->h);
	masked_blit(textbmp, screen, 0, 0, text_x, text_y, textbmp->w, textbmp->h);

	clear_keybuf();

	while(looping) {

		blit(backg, screen, text_x - 10 - choice->w, text_y - 20, text_x - 10 - choice->w, text_y - 20, choice->w, choice->h * 10);
		masked_blit(choice, screen, 0, 0, text_x - 10 - choice->w, text_y + FONT_H * (pos - 1), choice->w, choice->h);

		switch((readkey() >> 8)) {

                	case KEY_ENTER:
                	case KEY_SPACE:
                        	looping = 0;
                                break;

                	case KEY_ESC:
				pos = 0;
                                looping = 0;
                                break;

                	case KEY_UP:
                        	if(pos > 1)
                                	pos--;
				else
                                	pos = num_items;
                        	break;

                	case KEY_DOWN:
                        	if(pos < num_items)
                                	pos++;
				else
                                	pos = 1;
                        	break;
                }
        }

        destroy_bitmap(textbmp);
	icache.FreeImage(choice);
	icache.FreeImage(backg);

	clear_keybuf();

        return pos;
}
/*****************************************************************************************/
void LoadGame() {
	char buffer[80];
	char filename[512];
        FILE *fs;
        int slot;
	
#ifdef DJGPP	
	sprintf(filename, "savegame.dat");
#else	
	sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));
#endif
        if ((fs = fopen(filename, "rb")) == NULL) {
        	for (int i = 0; i < 5;i++) {
                	SavedGames[i].level = 0;
			memcpy(&SavedGames[i].dat, &DefaultPData, sizeof(struct PlayerData));
                }
#ifndef DJGPP
		/* Create dir */
		sprintf(filename, "%s/.urban", getenv("HOME"));
		mkdir(filename, S_IRUSR | S_IWUSR | S_IXUSR);

		sprintf(filename, "%s/.urban/savegame.dat", getenv("HOME"));
#endif
                if((fs = fopen(filename, "wb")) != NULL) {

	        	fwrite(SavedGames, 1, 5 * sizeof(struct SaveGameData), fs);
		        fclose(fs);
                }
        } else {
	        fread(SavedGames, 1, 5 * sizeof(struct SaveGameData), fs);
	        fclose(fs);
	}

        buffer[0] = 0;

        for(int i = 0;i < 5;i++) {
        	if(SavedGames[i].level) {
                        strcat(buffer, SavedGames[i].name);
                        strcat(buffer, "\n");
                } else
                	strcat(buffer, "EMPTY\n");

        }

        if ((slot = Do_Menu(buffer, 5)) > 0)
        	if (SavedGames[slot - 1].level)
                	NewGame(slot);
}


void UrbanSetGFXMode() {
	set_gfx_mode(GFX_MODEX,	SCREEN_WIDTH, SCREEN_HEIGHT, 0,	0);

        set_palette (black_palette);
}

extern char *special_thanks;

/*****************************************************************************************/
int main(int argc, char	**argv)	{
//	int looping = 1;
	int pos	= 1;
	int c;
        char *meny_text = "START GAME\nLOAD GAME\nHIGHSCORE\nCREDITS\nCONFIG\nQUIT";
	config = new Config;


	allegro_init();
	install_keyboard();
	install_timer();

        engine = new Engine;

	opterr = 0;
        _SET_SPEC_LEVEL_OFF;
        _SET_VERT_RETRACE_OFF;
/*
M�jliga v�xlar:
-f<mapname> Namn p� karta att spela
-x Sl� av ljudeffekterna
-v Sl� av vertical retrace
-b Sl� av bakgrunderna
-s Sl� av musiken
-i Visa inte intron
-r Sl� p� vertical Retrace
-h visa hj�lp
*/
	while ((c = getopt(argc, argv, "hrisbvxf:")) != -1) {
		switch (c) {
                	case 'h':
                        	printf("Urban v1.2\n");
                                printf("Options:\n");
                                printf("-f<mapname>   Play a specific map\n");
                                printf("-x            Disable sound effects\n");
                                printf("-s            Disable music\n");
                                printf("-i            Don't show intro\n");
                                printf("-h            Show this\n");
                        	return 0;
                        	break;
			case 'f':
                        	_SET_SPEC_LEVEL_ON;
                                UrbanSetGFXMode();
				ENGINE.play_level(optarg);
				return 0;
				break;
			case 's':
                        	_SET_SOUND_OFF;
                                break;
			case 'b':
                        	_SET_BACKGROUND_OFF;
                                break;
			case 'v':
                        	_SET_VERT_RETRACE_OFF;
                                break;
			case 'r':
                        	_SET_VERT_RETRACE_ON;
                                break;
			case 'x':
                        	_SET_SOUNDFX_OFF;
                                break;
			case 'i':
                        	_SET_INTRO_OFF;
                                break;
		}
	}

	UrbanSetGFXMode();

        if (IS_INTRO_ON) {
		// Run intro
        	Intro *intro = new Intro;

	        intro->RunIntro();
        	delete intro;
	}
	clear_keybuf();

	if (IS_SOUND_ON) {
		char buffer[1024];

	       	sprintf(buffer, "%s/%s", DATPATH, MENU_MUSIC);
		SOUND.PlayMusic(buffer);
	}

	while((pos = Do_Menu(meny_text, 6, pos)) && pos != 6)
		MenuChoice(pos);

//        fade_out(6);
	{	
		UrbanFont L(LARGE_FONT);
		UrbanFont S(SMALL_FONT);
	/*	clear_to_color(screen, 200);*/
		PALETTE pal;
	        BITMAP *backg = icache.GetImage("ibild.pcx", pal);
		blit(backg, screen, 0, 0, 0, 0, backg->w, backg->h);
		icache.FreeImage(backg);
		
		rest(500);
	
		L.print_centre("THE URBAN", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 50, screen);
		L.print_centre("HOMEPAGE", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, screen);
		S.print_centre("urban.home.dhs.org", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 40, screen);
  		int i = 600;
		
		while(!keypressed() && i)rest(100), i--;
	}
	fade_out(6);
	
#ifndef DJGPP
	close_gfx();
#endif
	return 0;
}
