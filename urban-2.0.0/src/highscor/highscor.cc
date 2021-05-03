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
#include <stdlib.h>
#include <allegro.h>
#include <string.h>
#include "highscor.h"
#include "engine.h"
#include "urbfont.h"
#ifndef DJGPP
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <unistd.h>
#endif
/***************************************************************************/
HighScore::~HighScore() {
}
/***************************************************************************/
HighScore::HighScore() {
	char buffer[256];
	Open();
        UrbanFont fnt(SMALL_FONT2);
        BITMAP *textbmp = create_bitmap(220, 210);
        clear (textbmp);
        int x, y;

        // Ritar ett rutnär
        for(x = 0;x < 320; x+=2) {
	        for(y = 0;y < 240; y+=2) {

			putpixel(screen, x, y, 0);
			putpixel(screen, x + 1, y + 1, 0);
		}
	}
        rectfill(screen, 45, 25, 320 - 45, 233, 27);
        rectfill(screen, 50, 30, 320 - 50, 228, 25);
        rectfill(screen, 50, 30, 320 - 50, 63, 23);

	fnt.print("***** HALL OF FAME *****", 60, 13, textbmp);
	fnt.print("  Name          Score Level", 0, 30, textbmp);

        for(int i = 0; i < NUM_HIGHSCORES; i++) {

        	sprintf(buffer, "%2d", i + 1);
		fnt.print(buffer, 15, 50 + i * 16, textbmp);

		if(highscore[i].Level)
	        	sprintf(buffer, " %-10s%6d   %1d:%1d", highscore[i].Name,
	        		highscore[i].Score, ((highscore[i].Level-1) / 3) + 1, ((highscore[i].Level-1) % 3) + 1);
		else
	        	sprintf(buffer, " %-10s%6d   0:0", "Empty", 0);

			
		fnt.print(buffer, 31, 50 + i * 16, textbmp);

        }
       	masked_blit(textbmp, screen, 0, 0, 50, 20, 220, 210);
        destroy_bitmap(textbmp);
}
/***************************************************************************/
char *HighScore::GetName() {
	static char Name[64] = "";
        int key, pos;
        PALETTE pal;
	BITMAP *backg = icache.GetImage("ibild.pcx", pal);
        BITMAP *textbmp = create_bitmap(220, 55);
        UrbanFont fnt(SMALL_FONT2);

        if(strlen(Name) == 0) {
#ifdef DJGPP
		strcpy(Name, "Unknown");
#else
		if(getenv("USER") != NULL)
			strcpy(Name, getenv("USER"));
		else
			strcpy(Name, "Unknown");
#endif
        }

	pos = strlen(Name);

	set_palette(pal);
	blit(backg, screen, 0, 0, 0, 0, 320, 240);

        icache.FreeImage(backg);

	clear_keybuf();

        do {
	        rectfill(screen, 45, 55, 320 - 45, 240 - 120, 27);
	        rectfill(screen, 50, 60, 320 - 50, 240 - 125, 25);
	        rectfill(screen, 50, 60, 320 - 50, 240 - 165, 23);

	        clear(textbmp);

                fnt.print_centre("HighScore", 110, 8, textbmp);
                fnt.print_centre("Please Enter Your Name:", 110, 24, textbmp);
                fnt.print_centre(Name, 110, 43, textbmp);

               	masked_blit(textbmp, screen, 0, 0, 50, 60, 220, 55);

        	key = readkey();

                if((key >> 8) == KEY_ENTER) {

                	Name[pos] = 0;
		        destroy_bitmap(textbmp);
                        return Name;
                }
                if((key >> 8) == KEY_ESC) {

		        destroy_bitmap(textbmp);
		        return "Unknown";
                }
                if((key >> 8) == KEY_BACKSPACE) {
			if(pos == 0)
                        	continue;
                	pos--;
                        Name[pos] = 0;
                }
                else if(pos < 10){

			if(!((key & 0xff) < 'A'	|| (key	& 0xff)	> 'z') || ((key & 0xff) == ' ')) {

		                Name[pos++] = (key & 0xff);
	                        Name[pos] = 0;
			}
                }
        } while (1);

        destroy_bitmap(textbmp);
        return "Unknown";
}
/***************************************************************************/
HighScore::HighScore(int score, int level) {
	Open();

        for(int i = 0; i < NUM_HIGHSCORES; i++) {

        	if(score > highscore[i].Score) {

                	memmove(&highscore[i + 1], &highscore[i],
                		sizeof(Score_t) * (NUM_HIGHSCORES - i - 1));

                        strcpy(highscore[i].Name, GetName());
                        highscore[i].Score = score;
                        highscore[i].Level = level;
                        break;
        	}
        }

        Save();
}
/***************************************************************************/
void HighScore::Open() {
	// Reset Score
	memset(highscore, 0, sizeof(Score_t) * NUM_HIGHSCORES);
#ifdef DJGPP
        if((fd = fopen("hs.dat", "rb")) == NULL)
#else
#ifdef HSFILENAME
        if((fd = fopen(HSFILENAME, "rb")) == NULL)
#else
	char filename[1024];

	sprintf(filename, "%s/.urban/hs.dat", getenv("HOME"));

        if((fd = fopen(filename, "rb")) == NULL)
#endif
#endif
        	return;

       	fread(highscore, sizeof(Score_t), NUM_HIGHSCORES, fd);

        fclose(fd);
}
/***************************************************************************/
void HighScore::Save() {

#ifdef DJGPP
        if((fd = fopen("hs.dat", "wb")) == NULL)
#else
#ifdef HSFILENAME
        if((fd = fopen(HSFILENAME, "wb")) == NULL)
#else
	char filename[1024];

	/* Create dir */
	sprintf(filename, "%s/.urban", getenv("HOME"));
	mkdir(filename, S_IRUSR | S_IWUSR | S_IXUSR);

	sprintf(filename, "%s/.urban/hs.dat", getenv("HOME"));

        if((fd = fopen(filename, "wb")) == NULL)
#endif
#endif
        	return;

       	fwrite(highscore, sizeof(Score_t), NUM_HIGHSCORES, fd);

        fclose(fd);
}
/***************************************************************************/
#if 0
void main() {
	int Score = 5000;
        int Level = 55;
        HighScore *test;
        PALETTE pal;
	BITMAP *backg = load_bitmap("../gfx/ibild.pcx", pal);

	// Init Graphics
	allegro_init();
	install_keyboard();

	set_gfx_mode(GFX_MODEX, 320, 240, 0, 0);
	set_palette(pal);
	blit(backg, screen, 0, 0, 0, 0, 320, 240);

	test = new HighScore(Score, Level);
        delete test;

	blit(backg, screen, 0, 0, 0, 0, 320, 240);

	test = new HighScore();
        readkey();
        delete test;

        readkey();
}
#endif
/***************************************************************************/





