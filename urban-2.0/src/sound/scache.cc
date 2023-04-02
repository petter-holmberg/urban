/******************************************************************************
    URBAN 2.0
    Copyright (C) 2021  Petter Holmberg (petter.holmberg@usa.net)

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
*****************************************************************************/
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
#include "scache.h"
#include "game.h"
#include <allegro.h>
#include <mikmod.h>
#include <string_view>
#include <vector>

/***************************************************************************/
SoundCache::SoundCache()
{
    num_samples = 0;
    max_samples = 5;

    cache.resize(max_samples);
}

/***************************************************************************/
SoundCache::~SoundCache()
{
    for (auto& entry : cache) {
        if (entry.sample != nullptr) {
            ::Sample_Free(entry.sample);
        }
    }
}
/***************************************************************************/
auto SoundCache::FindEntry(std::string_view filename) -> SCacheEntry*
{
    for (int i = 0; i < num_samples; i++) {
        if (filename == cache[i].filename) {
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
        return entry->sample;

    } // Need more entries?
    if (num_samples == max_samples) {
        max_samples += 5;
        cache.resize(max_samples);
    }
    entry = &cache[num_samples++];
    entry->sample = ::Sample_Load(pathname);
    entry->filename = pathname;

    return entry->sample;
}
/***************************************************************************/
