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
#include "icache.h"
#include "datfile.h"
#include "object.h"
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __USE_DATFILE__

/***************************************************************************/
ImageCache::ImageCache()
{
    num_images = 0;
    max_images = 5;

    cache = (CacheEntry*)malloc(max_images * sizeof(CacheEntry));
#ifdef __USE_DATFILE__
    dat = new datfile("urban.dat");
#endif
}
/***************************************************************************/
void ImageCache::FreeImage(BITMAP* bitmap)
{

    for (int i = 0; i < num_images; i++) {
        if (bitmap == cache[i].bitmap) {

            cache[i].count--;
            /*                        if(cache[i].count == 0) {
                        	destroy_bitmap(cache[i].bitmap);
                                free(cache[i].filename);
                                memmove(&cache[i], &cache[i+1], sizeof(CacheEntry) * (num_images - i - 1));
                                num_images--;
                        }*/
            return;
        }
    }
}
/***************************************************************************/
ImageCache::~ImageCache()
{
    /*
	int i;
	FILE *f;
        f = fopen("icache.txt", "w");
        fprintf(f, "Total number of pictures: %d\n", num_images);
        for (i = 0;i < num_images;i++)
	        fprintf(f, "Filename: %s, Num users: %d\n", cache[i].filename, cache[i].count);
	fclose(f);*/
    /*	for(int i=0;i<num_images;i++) {
        	destroy_bitmap(cache[i].bitmap);
                free(cache[i].filename);
        }*/
}
/***************************************************************************/
CacheEntry* ImageCache::FindEntry(const char* filename)
{
    for (int i = 0; i < num_images; i++) {

        if (!strcmp(filename, cache[i].filename))
            return &cache[i];
    }
    return NULL;
}
/***************************************************************************/
BITMAP* ImageCache::GetImage(const char* filename, RGB* pal)
{
    char pathname[512];
    CacheEntry* entry;
    // Allready in cache
#ifndef __USE_DATFILE__
    sprintf(pathname, "%s/%s", GFX_PATH, filename);
#else
    sprintf(pathname, "gfx/%s", filename);
#endif

    if ((entry = FindEntry(pathname))) {

        entry->count++;
        memcpy(pal, entry->pal, 256 * sizeof(RGB));
        return entry->bitmap;

    } else {
        // Need more entires?
        if (num_images == max_images) {
            max_images += 5;
            cache = (CacheEntry*)
                realloc(cache, max_images * sizeof(CacheEntry));
        }
        entry = &cache[num_images++];
        entry->pal = new RGB[256];
#ifndef __USE_DATFILE__
        if ((entry->bitmap = load_bitmap(pathname, entry->pal)) == NULL) {

            set_gfx_mode(GFX_TEXT, 160, 25, 0, 0);
            printf("\nCan't load \"%s\"\n", pathname);
            exit(1);
        }
#else
        if ((entry->bitmap = dat->load_pcx(pathname, entry->pal)) == NULL) {

            set_gfx_mode(GFX_TEXT, 160, 25, 0, 0);
            printf("\nCan't load \"%s\"\n", pathname);
            exit(1);
        }
#endif
        memcpy(pal, entry->pal, 256 * sizeof(RGB));
        entry->filename = strdup(pathname);
        entry->count = 1;

        return entry->bitmap;
    }
}
/***************************************************************************/
