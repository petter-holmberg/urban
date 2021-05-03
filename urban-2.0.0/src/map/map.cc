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
    Thomas Nyberg					Jonas Borgstr�m
    Br�nnbergsv�gen 21b					Bondev�gen 6
    92136 Lycksele					92332 Storuman
    Sweden						Sweden

    or email:

    thomas.nyberg@usa.net				jonas_b@bitsmart.com
*****************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "icache.h"
#include "engine.h"

#define __CHECK_MAP_BOUNDRIES
#define BLUE_LEVEL 30
//#define _USE_EFFECTS
//#define _USE_DEKORATIONS

struct TileLoadInfo {
	long tile_info;
	char *filename;
} tiles_to_load[] = {
{0x0001, "tiles/bricks.pcx"},
{0x0002, "tiles/bricks2.pcx"},
{0x0003, "tiles/bricks3.pcx"},
{0x0004, "tiles/bricks4.pcx"},
{0x0005, "tiles/box.pcx"},
{0x0006, "tiles/box2.pcx"},
{0x0007, "tiles/box3.pcx"},
{0x0008, "tiles/box4.pcx"},
{0x1009, "tiles/sandbag.pcx"},
{0x100A, "tiles/barrel.pcx"},
{0x100B, "tiles/barrel2.pcx"},
{0x100C, "tiles/barrel3.pcx"},
{0x100D, "tiles/barrel4.pcx"},
{0x100E, "tiles/barrel5.pcx"},
{0x100F, "tiles/barrel6.pcx"},
{0x1010, "tiles/barrel7.pcx"},
{0x1011, "tiles/barrel8.pcx"},
{0x1012, "tiles/barrel9.pcx"},
{0x1013, "tiles/balk1.pcx"},
{0x1014, "tiles/balk2.pcx"},
{0x1015, "tiles/balk3.pcx"},
{0x1016, "tiles/balk4.pcx"},
{0x1017, "tiles/unknown1.pcx"},
{0x1018, "tiles/unknown2.pcx"},
{0x1019, "tiles/unknown3.pcx"},
{0x101A, "tiles/unknown4.pcx"},
{0x101B, "tiles/comp1.pcx"},
{0x101C, "tiles/comp2.pcx"},
{0x101D, "tiles/comp3.pcx"},
{0x101E, "tiles/comp4.pcx"},
{0x101F, "tiles/comp5.pcx"},
{0x1020, "tiles/comp6.pcx"},
{0x1021, "tiles/comp7.pcx"},
{0x1022, "tiles/comp8.pcx"},
{0x1023, "tiles/comp9.pcx"},
{0x1024, "tiles/comp10.pcx"},
{0x1025, "tiles/comp11.pcx"},
{0x1026, "tiles/comp12.pcx"},
{0x1027, "tiles/table1.pcx"},
{0x1028, "tiles/table2.pcx"},
{0x1029, "tiles/maskin1.pcx"},
{0x1030, "tiles/maskin2.pcx"},
{0x1031, "tiles/maskin3.pcx"},
{0x1032, "tiles/maskin4.pcx"},
{0x1033, "tiles/maskin5.pcx"},
{0x1034, "tiles/maskin6.pcx"},
{0x1035, "tiles/maskin7.pcx"},
{0x1036, "tiles/maskin8.pcx"},
{0x1037, "tiles/maskin9.pcx"},
{0x1038, "tiles/maskin10.pcx"},
{0x1039, "tiles/maskin11.pcx"},
{0x1040, "tiles/maskin12.pcx"},
{0x1041, "tiles/ror1.pcx"},
{0x1042, "tiles/ror2.pcx"},
{0x1043, "tiles/ror3.pcx"},
{0x1044, "tiles/ror4.pcx"},
{0x1045, "tiles/ror5.pcx"},
{0x1046, "tiles/ror6.pcx"},
{0x1047, "tiles/ror7.pcx"},
{0x1048, "tiles/ror8.pcx"},
{0x1049, "tiles/ror9.pcx"},
{0x1050, "tiles/ror10.pcx"},
{0x1051, "tiles/ror11.pcx"},
{0x1052, "tiles/ror12.pcx"},
{0x1053, "tiles/ror13.pcx"},
{0x1054, "tiles/ror14.pcx"},
{0x1055, "tiles/ror15.pcx"},
{0x1056, "tiles/ror16.pcx"},
{0x1057, "tiles/ror17.pcx"},
{0x1058, "tiles/ror18.pcx"},
{0x1059, "tiles/desk1.pcx"},
{0x1060, "tiles/desk2.pcx"},
{0x1061, "tiles/desk3.pcx"},
{0x1062, "tiles/desk4.pcx"},
{0x1063, "tiles/desk5.pcx"},
{0x1064, "tiles/desk6.pcx"},
{0x1065, "tiles/desk7.pcx"},
{0x1066, "tiles/desk8.pcx"},
{0x1067, "tiles/desk9.pcx"},
{0x1068, "tiles/tower.pcx"},
{0x1069, "tiles/skap1.pcx"},
{0x1070, "tiles/skap2.pcx"},
{0x1071, "tiles/skap3.pcx"},
{0x1072, "tiles/table3.pcx"},
{0x1073, "tiles/table4.pcx"},
{0x1074, "tiles/table5.pcx"},
{0x1075, "tiles/table6.pcx"},
{0x1076, "tiles/table7.pcx"},
{0x1077, "tiles/table8.pcx"},
{0x1078, "tiles/table9.pcx"},
{0x1079, "tiles/barrel10.pcx"},
{0x1080, "tiles/table10.pcx"},
{0x1081, "tiles/table11.pcx"},
{0x1082, "tiles/table12.pcx"},
{0x1083, "tiles/table13.pcx"},
{0x1084, "tiles/table14.pcx"},
{0x1085, "tiles/table15.pcx"},
{0x1086, "tiles/desk10.pcx"},
{0x1087, "tiles/balk5.pcx"},
{0x1000, "tiles/1.pcx"},
{0x1000, "tiles/2.pcx"},
{0x1000, "tiles/3.pcx"},
{0x1000, "tiles/4.pcx"},
{0x1000, "tiles/5.pcx"},
{0x1000, "tiles/6.pcx"},
{0x1000, "tiles/7.pcx"},
{0x1000, "tiles/8.pcx"},
{0x1000, "tiles/9.pcx"},
{0x1000, "tiles/10.pcx"},
{0x1000, "tiles/11.pcx"},
{0x1000, "tiles/12.pcx"},
{0x1000, "tiles/13.pcx"},
{0x1000, "tiles/14.pcx"},
{0x1000, "tiles/15.pcx"},
{0x1000, "tiles/16.pcx"},
{0x1000, "tiles/17.pcx"},
{0x1000, "tiles/18.pcx"},
{0x1000, "tiles/19.pcx"},
{0x1000, "tiles/20.pcx"},
{0x1000, "tiles/21.pcx"},
{0x1000, "tiles/22.pcx"},
{0x1000, "tiles/23.pcx"},
{0x1000, "tiles/24.pcx"},
{0x1000, "tiles/25.pcx"},
{0x1000, "tiles/26.pcx"},
{0x1000, "tiles/27.pcx"},
{0x1000, "tiles/29.pcx"},
{0x1000, "tiles/30.pcx"},
{0x1000, "tiles/31.pcx"},
{0x1000, "tiles/32.pcx"},
{0x1000, "tiles/33.pcx"},
{0x1000, "tiles/34.pcx"},
{0x1000, "tiles/35.pcx"},
{0x1000, "tiles/36.pcx"},
{0x1000, "tiles/37.pcx"},
{0x1000, "tiles/38.pcx"},
{0x1000, "tiles/39.pcx"},
{0x1000, "tiles/40.pcx"},
{0x1000, "tiles/41.pcx"},
{0x1000, "tiles/42.pcx"},
{0x1000, "tiles/43.pcx"},
{0x1000, "tiles/44.pcx"},
{0x1000, "tiles/45.pcx"},
{0x1000, "tiles/46.pcx"},
{0x1000, "tiles/47.pcx"},
{0x1000, "tiles/48.pcx"},
{0x1000, "tiles/49.pcx"},
{0x1000, "tiles/50.pcx"},
{0x1000, "tiles/51.pcx"},
{0x1000, "tiles/52.pcx"},
{0x1000, "tiles/53.pcx"},
{0x1000, "tiles/54.pcx"},
{0x1000, "tiles/55.pcx"},
{0x1000, "tiles/56.pcx"},
{0x1000, "tiles/57.pcx"},
{0x1000, "tiles/28.pcx"},
{0x1000, "tiles/58.pcx"},
{0x1000, "tiles/59.pcx"},
{0x1000, "tiles/60.pcx"},
{0x1000, "tiles/61.pcx"},
{0x1000, "tiles/fence1.pcx"},
{0x1000, "tiles/fence2.pcx"},
{0x1000, "tiles/fence3.pcx"},
{0x1000, "tiles/fence4.pcx"},
{0x1000, "tiles/fence5.pcx"},
{0x1000, "tiles/fence6.pcx"},
{0x1000, "tiles/fence7.pcx"},
{0x1000, "tiles/fence8.pcx"},
{0x1000, "tiles/fence9.pcx"},
{0x1000, "tiles/fence10.pcx"},
{0x1000, "tiles/fence11.pcx"},
{0x3000, "tiles/blod1.pcx"},
{0x1000, "tiles/62.pcx"},
{0x1000, "tiles/63.pcx"},
{0x1000, "tiles/64.pcx"},
{0x3000, "tiles/radio1.pcx"},
{0x3000, "tiles/oil1.pcx"},
{0x1000, "tiles/barrel11.pcx"},
{0x1000, "tiles/barrel12.pcx"},
{0x1000, "tiles/dricka1.pcx"},
{0x1000, "tiles/dricka2.pcx"},
{0x1000, "tiles/dricka3.pcx"},
{0x1000, "tiles/dricka4.pcx"},
{0x1000, "tiles/dricka5.pcx"},
{0x1000, "tiles/dricka6.pcx"},
{0x1000, "tiles/sand1.pcx"},
{0x1000, "tiles/sand2.pcx"},
{0x3000, "tiles/rock1.pcx"},
{0x3000, "tiles/plant1.pcx"},
{0x3000, "tiles/plant2.pcx"},
{0x3000, "tiles/sign1.pcx"},
{0x3000, "tiles/rock2.pcx"},
{0x1000, "tiles/metal1.pcx"},
{0x1000, "tiles/metal2.pcx"},
{0x1000, "tiles/metal3.pcx"},
{0x1000, "tiles/metal4.pcx"},
{0x1000, "tiles/metal5.pcx"},
{0x1000, "tiles/metal6.pcx"},
{0x0000, "tiles/65.pcx"},
{0x0000, "tiles/66.pcx"},
{0x3000, "tiles/net.pcx"},
{0x1000, "tiles/styrstav.pcx"},
{0x1000, "tiles/sand3.pcx"},
{0x1000, "tiles/rock3.pcx"},
{0x1000, "tiles/rock4.pcx"},
{0x3000, "tiles/stalagti.pcx"},
{0x3000, "tiles/papper1.pcx"},
{0x3000, "tiles/papper2.pcx"},

{0, ""}};
/***************************************************************************/
struct BGLoadInfo {
	long tile_info;
	char *filename;
} bg_to_load[] = {
{0x0001, "bakg/1.pcx"},
{0x0002, "bakg/2.pcx"},
{0x0003, "bakg/3.pcx"},
{0x0004, "bakg/4.pcx"},
{0x0005, "bakg/5.pcx"},
{0x0006, "bakg/6.pcx"},
{0x0007, "bakg/7.pcx"},
{0x0008, "bakg/8.pcx"},
{0x0009, "bakg/9.pcx"},
{0x000A, "bakg/10.pcx"},
{0x000B, "bakg/11.pcx"},
{0x000C, "bakg/12.pcx"},
{0x000D, "bakg/13.pcx"},
{0x000E, "bakg/14.pcx"},
{0x000F, "bakg/15.pcx"},
{0x0010, "bakg/16.pcx"},
{0x0011, "bakg/17.pcx"},
{0x0012, "bakg/18.pcx"},
{0x0013, "bakg/19.pcx"},
{0x0014, "bakg/20.pcx"},
{0x0015, "bakg/21.pcx"},
{0x0016, "bakg/22.pcx"},
{0x0017, "bakg/23.pcx"},
{0x0018, "bakg/24.pcx"},
{0x0019, "bakg/25.pcx"},
{0x001A, "bakg/26.pcx"},
{0x001C, "bakg/27.pcx"},
{0x001D, "bakg/28.pcx"},
{0x001E, "bakg/29.pcx"},
{0x001F, "bakg/30.pcx"},
{0x0020, "bakg/31.pcx"},
{0x0021, "bakg/32.pcx"},
{0x0022, "bakg/33.pcx"},
{0x0023, "bakg/34.pcx"},
{0x0024, "bakg/35.pcx"},
{0x0025, "bakg/36.pcx"},
{0x0026, "bakg/37.pcx"},
{0x0027, "bakg/38.pcx"},
{0x0028, "bakg/39.pcx"},
{0x0029, "bakg/40.pcx"},
{0x002A, "bakg/41.pcx"},
{0x002B, "bakg/42.pcx"},
/*{0x0001, "backgr1.pcx"},
{0x0002, "backgr2.pcx"},*/
{0, ""}};
/***************************************************************************/
Map::Map() {
	PALETTE Blue;

	dat = new datfile("levels.dat");
	sections[0] = sections[1] = sections[2]	= NULL;
	num_tiles = x_size = y_size = 0;
	bg_num_tiles = bg_x_size = bg_y_size = 0;
	Tiles =	NULL;
	Filename = NULL;
	TileInfo = NULL;
	objects[0] = objects[1]	= objects[2] = NULL;
	effect[0] = effect[1] =	effect[2] = NULL;
	dekor[0] = dekor[1] = dekor[2] = NULL;
	options = NULL;
	background = NULL;
	pal_type = PAL_AIR;

        pal_count = 0;
        // Get palette
        BITMAP *bmp = icache.GetImage("tiles/desk10.pcx", Pal_air);
        icache.FreeImage(bmp);

        memcpy(Pal, Pal_air, sizeof(PALETTE));

        // Create a blue palette
        for(int i=0;i<256;i++) {

        	Blue[i].r = Blue[i].g = 0;
        	Blue[i].b = 63;
        }
        fade_interpolate (Pal_air, Blue, Pal_water, BLUE_LEVEL, 0, 255);

}
/***************************************************************************/
void Map::NewLevel(int width, int height) {

	if (sections[0])
		delete [] sections[0];
	if (sections[1])
		delete [] sections[1];
	if (sections[2])
		delete [] sections[2];

	if (objects[0])
		delete [] objects[0];
	if (objects[1])
		delete [] objects[1];
	if (objects[2])
		delete [] objects[2];

	if (background)
		delete [] background;

	x_size = width;
	y_size = height;

	bg_x_size = ((width * TILE_WIDTH) / BG_TILE_WIDTH) + 1;
	bg_y_size = ((height * TILE_SIDE_HEIGHT) / BG_TILE_HEIGHT) + 1;
        if(bg_y_size < 4)
        	bg_y_size = 4;

	background = (int *)calloc(bg_x_size * bg_y_size, sizeof(int));

        // Default background
        for(int i = 0;i < (bg_x_size * bg_y_size); i++)
        	background[i] = 1;

	sections[0] = (int *)calloc(x_size * y_size, sizeof(int));
	sections[1] = (int *)calloc(x_size * y_size, sizeof(int));
	sections[2] = (int *)calloc(x_size * y_size, sizeof(int));
	objects[0] = (int *)calloc(x_size * y_size, sizeof(int));
	objects[1] = (int *)calloc(x_size * y_size, sizeof(int));
	objects[2] = (int *)calloc(x_size * y_size, sizeof(int));
#ifdef _USE_DEKORATIONS
	dekor[0] = (int	*)calloc(x_size	* y_size, sizeof(int));
	dekor[1] = (int	*)calloc(x_size	* y_size, sizeof(int));
	dekor[2] = (int	*)calloc(x_size	* y_size, sizeof(int));
#endif
#ifdef _USE_EFFECTS
	effect[0] = (int *)calloc(x_size * y_size, sizeof(int));
	effect[1] = (int *)calloc(x_size * y_size, sizeof(int));
	effect[2] = (int *)calloc(x_size * y_size, sizeof(int));
#endif
        options = (char **)calloc(MAX_OPTIONS, sizeof(char *));
}
/***************************************************************************/
Map::~Map() {
	if (sections[0])
		delete [] sections[0];
	if (sections[1])
		delete [] sections[1];
	if (sections[2])
		delete [] sections[2];
	/* OBS!!!!!!!!!!!! Fria allt minne, bitte! */
}
/***************************************************************************/
int Map::SaveMap (char *filename) {
	FILE *fd;
	int x =	0, y = 0, z = 0;

	if ((fd	= fopen(filename, "w"))	== NULL) {

		perror(filename);

		return 0;
	}
	fprintf(fd, "URBAN MAP 1.0\n");
	fprintf(fd, "X_SIZE %d\n", x_size);
	fprintf(fd, "Y_SIZE %d\n", y_size);

	fprintf(fd, "\n");

	for(z=0;z<3;z++) {
		fprintf(fd, "<SECTION_%d>\n", z);
		for(y=0;y<y_size;y++) {
			for(x=0; x < x_size ; x++) {

				fprintf(fd, "%X,", GetBG(x, y, z));
			}
			fprintf(fd, "\n");
	       }
	       fprintf(fd, "</SECTION_%d>\n\n",	z);
	}

	for(z=0;z<3;z++) {
		fprintf(fd, "<OBJECT_%d>\n", z);
		for(y=0;y<y_size;y++) {
			for(x=0; x < x_size ; x++) {

				fprintf(fd, "%X,", GetObj(x, y,	z));
			}
			fprintf(fd, "\n");
	       }
	       fprintf(fd, "</OBJECT_%d>\n\n", z);
	}
#ifdef _USE_DEKORATIONS
	for(z=0;z<3;z++) {
		fprintf(fd, "<DEKOR_%d>\n", z);
		for(y=0;y<y_size;y++) {
			for(x=0; x < x_size ; x++) {

				fprintf(fd, "%X,", GetDekor(x, y, z));
			}
			fprintf(fd, "\n");
	       }
	       fprintf(fd, "</DEKOR_%d>\n\n", z);
	}
#endif
#ifdef _USE_EFFECTS
	for(z=0;z<3;z++) {
		fprintf(fd, "<EFFECT_%d>\n", z);
		for(y=0;y<y_size;y++) {
			for(x=0; x < x_size ; x++) {

				fprintf(fd, "%X,", GetEffect(x,	y, z));
			}
			fprintf(fd, "\n");
	       }
	       fprintf(fd, "</EFFECT_%d>\n\n", z);
	}
#endif
	fprintf(fd, "<BACKGROUND>\n");
	for(y = 0; y < bg_y_size; y++) {
		for(x=0; x < bg_x_size ; x++) {

			fprintf(fd, "%X,", GetBackGround(x, y));
		}
		fprintf(fd, "\n");
	}
	fprintf(fd, "</BACKGROUND>\n\n");

	if (options) {
	        fprintf(fd, "<OPTIONS>\n");
        	x = 0;
	        while (options[x])
                	fprintf(fd, "%s\n", options[x++]);
		fprintf(fd, "</OPTIONS>\n");
	}

	fprintf(fd, "END\n\n");


	fclose(fd);

	return 1;
}
/***************************************************************************/
int Map::LoadMap (char *filename) {
	FILE *fd;
	char *line;
	int i;
        PALETTE tmppal;

	x_size = y_size	= 0;

	if (sections[0])
		free(sections[0]);
	if (sections[1])
		free(sections[1]);
	if (sections[2])
		free(sections[2]);
	if (objects[0])
		free(objects[0]);
	if (objects[1])
		free(objects[1]);
	if (objects[2])
		free(objects[2]);

	if (effect[0])
		free(effect[0]);
	if (effect[1])
		free(effect[1]);
	if (effect[2])
		free(effect[2]);
	if (dekor[0])
		free(dekor[0]);
	if (dekor[1])
		free(dekor[1]);
	if (dekor[2])
		free(dekor[2]);
	if (background)
		free(background);
        if (options)
        	free(options);
	effect[0] = effect[1] =	effect[2] = NULL;
	dekor[0] = dekor[1] = dekor[2] = NULL;
	background = NULL;
        options = NULL;

        int using_dat = 1;

        if((fd = dat->open_file(filename)) == NULL) {

		if ((fd	= fopen(filename, "r"))	== NULL) {

			perror(filename);

			return 0;
		}
                using_dat = 0;
        }
	line = GetLine(fd);

	if(strncmp(line, "URBAN MAP 1.0", 13))
		return 0;

	x_size = atoi(GetLine(fd) + 7);
	y_size = atoi(GetLine(fd) + 7);

	bg_x_size = (x_size * TILE_WIDTH / BG_TILE_WIDTH) + 1;
	bg_y_size = (y_size * TILE_SIDE_HEIGHT / BG_TILE_HEIGHT) + 1;

        if(bg_y_size < 4)
        	bg_y_size = 4;

	if(! (x_size * y_size))	{
		printf("x_size * y_size == 0\n");

		return 0;
	}

	for(i =	0; i < num_tiles; i++) {

		delete [] Filename[i];
		icache.FreeImage (Tiles[i]);
	}

	for(i =	0; i < bg_num_tiles; i++) {

		delete [] BGFilename[i];
		icache.FreeImage (BGTiles[i]);
	}

	if (Tiles)
		free(Tiles);
	if (Filename)
		free(Filename);
	if (TileInfo)
		free(TileInfo);

       	Tiles =	(BITMAP	**)malloc(MAX_TILES * sizeof(BITMAP *));
       	Filename = (char **)malloc(MAX_TILES * sizeof(char *));
       	TileInfo = (int	*)malloc(MAX_TILES * sizeof(int));

       	BGTiles =	(BITMAP	**)malloc(MAX_TILES * sizeof(BITMAP *));
       	BGFilename = (char **)malloc(MAX_TILES * sizeof(char *));

	i = 0;
	num_tiles = 0;
	while (strlen(tiles_to_load[i].filename)) {

		Filename[num_tiles] = strdup (tiles_to_load[i].filename);
		Tiles[num_tiles] = icache.GetImage(tiles_to_load[i].filename, tmppal);
		TileInfo[num_tiles] = tiles_to_load[i].tile_info;
		num_tiles++;
		i++;
     	}

	i = 0;
	bg_num_tiles = 0;
	while (strlen(bg_to_load[i].filename)) {

		BGFilename[bg_num_tiles] = strdup (bg_to_load[i].filename);
		BGTiles[bg_num_tiles] = icache.GetImage(bg_to_load[i].filename, tmppal);
		bg_num_tiles++;
		i++;
     	}

       	while((line = GetLine(fd))) {
		if(!strcmp(line, "END"))
                	break;

		if(!strcmp(line, "<TILES>")) {
			while(strcmp("</TILES>", (line = GetLine(fd))))	{

				Tiles[num_tiles] = icache.GetImage(line + 5, tmppal);
				Filename[num_tiles] = strdup(line + 5);
				TileInfo[num_tiles] = (int)strtol(line,	NULL, 16);

				if(!Tiles[num_tiles]) {

			       		printf("Can't load '%s'\n", line + 5);
					return 0;
				}
				num_tiles++;
			};
		}
		if(!strcmp(line, "<BGTILES>")) {
			while(strcmp("</BGTILES>", (line = GetLine(fd)))) {

				BGTiles[bg_num_tiles] =	icache.GetImage(line + 5, tmppal);
				BGFilename[bg_num_tiles] = strdup(line + 5);

				if(!BGTiles[bg_num_tiles]) {

			       		printf("Can't load '%s'\n", line + 5);
					return 0;
				}
				bg_num_tiles++;
			};
		}

		if(!strcmp(line, "<SECTION_0>")) {
			sections[0] = new maptype[x_size * y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					sections[0][(j++) + (i * x_size)] =
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</SECTION_0>"))
				return 0;
		}

		if(!strcmp(line, "<SECTION_1>")) {
			sections[1] = new maptype[x_size * y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					sections[1][(j++) + (i * x_size)] =
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</SECTION_1>"))
				return 0;
		}
		if(!strcmp(line, "<SECTION_2>")) {
			sections[2] = new maptype[x_size * y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					sections[2][(j++) + (i * x_size)] =
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</SECTION_2>"))
				return 0;
		}
		if(!strcmp(line, "<OBJECT_0>"))	{
			objects[0] = new maptype[x_size	* y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					objects[0][(j++) + (i *	x_size)] =
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</OBJECT_0>"))
				return 0;
		}
		if(!strcmp(line, "<OBJECT_1>"))	{
			objects[1] = new maptype[x_size	* y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					objects[1][(j++) + (i *	x_size)] =
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</OBJECT_1>"))
				return 0;
		}
		if(!strcmp(line, "<OBJECT_2>"))	{
			objects[2] = new maptype[x_size	* y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					objects[2][(j++) + (i *	x_size)] =
						(int)strtol(tok, NULL, 16);
/*                                        if(objects[2][(j-1) + (i * x_size)]) {
                                        	printf("");
                                        }*/
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</OBJECT_2>"))
				return 0;
		}
		if(!strcmp(line, "<BACKGROUND>")) {
			background = new maptype[bg_x_size * bg_y_size];


			for(int	i = 0;i < bg_y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok = strtok(NULL, " ,")) {

					background[(j++) + (i *	bg_x_size)] =
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</BACKGROUND>"))
				return 0;
		}
#ifdef _USE_DEKORATIONS
		if(!strcmp(line, "<DEKOR_0>")) {
			dekor[0] = new maptype[x_size *	y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					dekor[0][(j++) + (i * x_size)] =
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</DEKOR_0>"))
				return 0;
		}
		if(!strcmp(line, "<DEKOR_1>")) {
			dekor[1] = new maptype[x_size *	y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					dekor[1][(j++) + (i * x_size)] =
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</DEKOR_1>"))
				return 0;
		}
		if(!strcmp(line, "<DEKOR_2>")) {
			dekor[2] = new maptype[x_size *	y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					dekor[2][(j++) + (i * x_size)] =
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</DEKOR_2>"))
				return 0;
		}
#endif
#ifdef _USE_EFFECTS
		if(!strcmp(line, "<EFFECT_0>"))	{
			effect[0] = new	maptype[x_size * y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					effect[0][(j++)	+ (i * x_size)]	=
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</EFFECT_0>"))
				return 0;
		}
		if(!strcmp(line, "<EFFECT_1>"))	{
			effect[1] = new	maptype[x_size * y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					effect[1][(j++)	+ (i * x_size)]	=
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</EFFECT_1>"))
				return 0;
		}
		if(!strcmp(line, "<EFFECT_2>"))	{
			effect[2] = new	maptype[x_size * y_size];

			for(int	i = 0;i	< y_size;i++) {
				int j =	0;
				line = GetLine(fd);
				char *tok;
				for (tok = strtok(line,	" ,");
					tok;tok=strtok(NULL, " ,")) {

					effect[2][(j++)	+ (i * x_size)]	=
						(int)strtol(tok, NULL, 16);
				}
			}
			line = GetLine(fd);
			if(strcmp(strtok(line, " \n"), "</EFFECT_2>"))
				return 0;
		}
#endif
		if(!strcmp(line, "<OPTIONS>"))	{
                	options = (char **)calloc(MAX_OPTIONS, sizeof(char *));
			i = 0;
                        char *tok;
                        do {
				line = GetLine(fd);
        	                tok = strtok(line, "\n");
                                if (strcmp(tok, "</OPTIONS>")) {
                                	options[i++] = strdup(tok);
                                        if (i > MAX_OPTIONS)
                                        	return 0;
                                } else
                                	break;
                        } while (strcmp(tok, "</OPTIONS>"));
		}
	};
        if(!using_dat)
		fclose(fd);

	return 1;
}
/***************************************************************************/
char *Map::GetLine (FILE *fd) {
	static char buffer[3000];

	do {
		if(fgets(buffer,  3000,	fd) == NULL) {

			return NULL;
		}
	} while	((buffer[0] == '#') || (strlen(buffer) == 1));

	if(buffer[strlen(buffer) - 1] == '\n')
		buffer[strlen(buffer) -	1] = 0;
	if(buffer[strlen(buffer) - 1] == '\r')
		buffer[strlen(buffer) -	1] = 0;

	return buffer;
}


/***************************************************************************/
int Map::GetBackGround (int x, int y) {
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0)
        	return 0;
	if (x > bg_x_size || y > bg_y_size)
        	return 0;
#endif
	if (background == NULL)
		return 0;
	return background[x + (y * bg_x_size)];
}

/***************************************************************************/
int Map::SetBackGround (int x, int y, int v) {
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0)
        	return 0;
	if (x > bg_x_size || y > bg_y_size)
        	return 0;
#endif
	return (background[x + (y * bg_x_size)]	= v);
}

/***************************************************************************/
int Map::GetEffect (int	x, int y, int z) {
#ifdef _USE_EFFECTS
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0 || z < 0)
        	return 0;
	if (x > x_size || y > y_size || z > 2)
        	return 0;
#endif
	if (effect[z] == NULL)
		return 0;
	return effect[z][x + (y	* x_size)];
#else
	return 0;
#endif
}

/***************************************************************************/
int Map::SetEffect (int	x, int y, int z, int v)	{
#ifdef _USE_EFFECTS
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0 || z < 0)
        	return 0;
	if (x > x_size || y > y_size || z > 2)
        	return 0;
#endif
	return (effect[z][x + (y * x_size)] = v);
#else
	return 0;
#endif
}

/***************************************************************************/
int Map::GetDekor (int x, int y, int z)	{
#ifdef _USE_DEKORATIONS
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0 || z < 0)
        	return 0;
	if (x > x_size || y > y_size || z > 2)
        	return 0;
#endif
	if (dekor[z] ==	NULL)
		return 0;
	return dekor[z][x + (y * x_size)];
#else
	return 0;
#endif
}

/***************************************************************************/
int Map::SetDekor (int x, int y, int z,	int v) {
#ifdef _USE_DEKORATIONS
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0 || z < 0)
        	return 0;
	if (x > x_size || y > y_size || z > 2)
        	return 0;
#endif
	return (dekor[z][x + (y	* x_size)] = v);
#else
	return 0;
#endif
}

/***************************************************************************/
int Map::GetObj	(int x,	int y, int z) {
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0 || z < 0)
        	return 0;
	if (x >= x_size || y >= y_size || z > 2)
        	return 0;
#endif
	return objects[z][x + (y * x_size)];
}

/***************************************************************************/
int Map::SetObj	(int x,	int y, int z, int v) {
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0 || z < 0)
        	return 0;
	if (x >= x_size || y >= y_size || z > 2)
        	return 0;
#endif
	return (objects[z][x + (y * x_size)] = v);
}

/***************************************************************************/
int Map::SetBG (int x, int y, int z, int v) {
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0 || z < 0)
        	return 0;
	if (x >= x_size || y >= y_size || z > 2)
        	return 0;
#endif
	return (sections[z][x +	(y * x_size)] =	v);
}

/***************************************************************************/
int Map::GetBG (int x, int y, int z) {
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0 || z < 0)
        	return 0;
	if (x >= x_size || y >= y_size || z > 2)
        	return 0;
#endif
	return sections[z][x + (y * x_size)];
}
/***************************************************************************/
int Map::GetWidth () {

	return x_size;
}
/***************************************************************************/
int Map::GetHeight () {

	return y_size;
}
/***************************************************************************/

BITMAP * Map::GetTile (int num)	{

	if(num >= num_tiles) {
        	num = 0;
/*		printf("num to high\n");
		exit(2);*/
	}
	return Tiles[num];
}
/***************************************************************************/
BITMAP * Map::GetTile (int x, int y, int z) {
#ifdef __CHECK_MAP_BOUNDRIES
	if (x < 0 || y < 0 || z < 0)
        	return 0;
	if (x > x_size || y > y_size || z > 2)
        	return 0;
#endif
	return Tiles[sections[z][x + (y	* x_size)]];
//	return Tiles[background[x + (y * x_size)]];
}
/***************************************************************************/
BITMAP * Map::GetBGTile (int num) {
	if(num >= bg_num_tiles) {
        	num = 0; //OBS! HACK!!!
/*		printf("num to high\n");
		exit(3);**/
	}
	return BGTiles[num];
}
/***************************************************************************/
RGB *Map::GetPal () {
	return Pal;
}
/***************************************************************************/
void Map::SetPal (int type) {
	pal_type = type;
}
/***************************************************************************/
void Map::UpdatePal () {
	if((pal_type == PAL_AIR) && pal_count) {

	        fade_interpolate (Pal_air, Pal_water, Pal, pal_count, 0, 255);
                set_palette(Pal);
                pal_count--;
        }
	else if((pal_type == PAL_WATER) && (pal_count < 64)) {

	        fade_interpolate (Pal_air, Pal_water, Pal, pal_count, 0, 255);
                set_palette(Pal);
                pal_count++;
        }
}
/***************************************************************************/
char *Map::GetOption(char *name) {
	int i;
        char *tok;
        char *temp;
        if (options == NULL)
        	return NULL;

        for (i = 0;i < MAX_OPTIONS;i++) {
        	if (options[i]) {
                	temp = strdup(options[i]);
	                tok = strtok(temp, "=\n");
        	        if (!strcmp(tok, name))
                		return strtok(NULL, "=\n");
			free(temp);
		} else
                        break;
	}
        return NULL;
}
/***************************************************************************/
void Map::SetOption(char *name) {
	int i;

        if (options == NULL)
        	options = (char **)calloc(MAX_OPTIONS, sizeof(char *));

        for (i = 0;i < MAX_OPTIONS;i++)
        	if (options[i] == NULL) {
                	options[i] = strdup(name);
                        return;
		}
}
/***************************************************************************/


