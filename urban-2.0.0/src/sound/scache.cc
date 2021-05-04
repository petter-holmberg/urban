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
#include <allegro.h>
#ifdef DJGPP
#include <jgmod.h>
#else
#include <mikmod.h>
#endif
#include "game.h"
#include "scache.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

/***************************************************************************/
SoundCache::SoundCache()
{
    num_samples = 0;
    max_samples = 5;

    cache = (SCacheEntry*)malloc(max_samples * sizeof(SCacheEntry));
}

/***************************************************************************/
void SoundCache::FreeSample(SAMPLE* sample)
{

    for (int i = 0; i < num_samples; i++) {
        if (sample == cache[i].sample) {

            cache[i].count--;
            return;
        }
    }
}
/***************************************************************************/
SoundCache::~SoundCache()
{
    /*	for(int i=0;i<num_images;i++) {
        	destroy_bitmap(cache[i].bitmap);
                free(cache[i].filename);
        }*/
}
/***************************************************************************/
auto SoundCache::FindEntry(const char* filename) -> SCacheEntry*
{
    for (int i = 0; i < num_samples; i++) {

        if (strcmp(filename, cache[i].filename) == 0) {
            return &cache[i];
        }
    }
    return nullptr;
}
/***************************************************************************/
auto SoundCache::GetSample(const char* filename) -> SAMPLE*
{
    char pathname[512];
    // Allready in cache
    sprintf(pathname, "%s/snd/%s", DATPATH, filename);
    SCacheEntry* entry = nullptr;
    // Allready in cache
    if ((entry = FindEntry(pathname)) != nullptr) {
        entry->count++;
        return entry->sample;

    } // Need more entires?
    if (num_samples == max_samples) {
        max_samples += 5;
        cache = (SCacheEntry*)
            realloc(cache, max_samples * sizeof(SCacheEntry));
    }
    entry = &cache[num_samples++];
#ifdef DJGPP
    entry->sample = load_wav(pathname);
#else
    entry->sample = Sample_Load(pathname);
#endif
    entry->filename = strdup(pathname);
    entry->count = 1;

    return entry->sample;
}
/***************************************************************************/
