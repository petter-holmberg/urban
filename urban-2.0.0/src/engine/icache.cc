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
#include "allegro.h"
#include "datfile.h"
#include "object.h"
#include <algorithm>
#include <string>
#include <string_view>

inline constexpr bool USE_DATFILE = true;

/***************************************************************************/
ImageCache::ImageCache()
{
    num_images = 0;
    max_images = 5;

    cache = (CacheEntry*)malloc(max_images * sizeof(CacheEntry));
    if constexpr (USE_DATFILE) {
        dat = new datfile("urban.dat");
    }
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
auto ImageCache::FindEntry(const char* filename) -> CacheEntry*
{
    for (int i = 0; i < num_images; i++) {

        if (std::string_view(filename) == std::string_view(std::begin(cache[i].filename))) {
            return &cache[i];
        }
    }
    return nullptr;
}
/***************************************************************************/
auto ImageCache::GetImage(const char* filename, PALETTE& pal) -> BITMAP*
{
    CacheEntry* entry = nullptr;
    // Allready in cache
    std::string pathname;
    if constexpr (USE_DATFILE) {
        //sprintf(pathname, "%s/%s", GFX_PATH, filename);
        pathname = std::string { "gfx/" } + filename;
    } else {
        pathname = std::string { "gfx/" } + filename;
    }

    if ((entry = FindEntry(pathname.c_str())) != nullptr) {
        entry->count++;
        pal = entry->pal;
        return entry->bitmap;

    } // Need more entires?
    if (num_images == max_images) {
        max_images += 5;
        cache = (CacheEntry*)
            realloc(cache, max_images * sizeof(CacheEntry));
    }
    entry = &cache[num_images++];

    if constexpr (USE_DATFILE) {
        if ((entry->bitmap = dat->load_pcx(pathname.c_str(), entry->pal)) == nullptr) {
            set_gfx_mode(GFX_TEXT, 160, 25, 0, 0);
            printf("\nCan't load \"%s\"\n", pathname.c_str());
            exit(1);
        }
    } else {
        if ((entry->bitmap = load_bitmap(pathname.c_str(), entry->pal)) == nullptr) {
            set_gfx_mode(GFX_TEXT, 160, 25, 0, 0);
            printf("\nCan't load \"%s\"\n", pathname.c_str());
            exit(1);
        }
    }

    pal = entry->pal;
    std::copy(pathname.c_str(), pathname.c_str() + pathname.size(), std::begin(entry->filename));
    entry->count = 1;

    return entry->bitmap;
}
/***************************************************************************/
