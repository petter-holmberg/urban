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
#ifndef __SCACHE_H__
#define __SCACHE_H__

#include <allegro.h>

#ifdef DJGPP
	#include <jgmod.h>
#else
	#include <mikmod.h>
#endif

struct SCacheEntry {
	char *filename;
        SAMPLE *sample;
        int count;
};

class SoundCache {
	public:
        	SoundCache();
                SAMPLE *GetSample(char *filename);
                void FreeSample(SAMPLE *bitmap);
                ~SoundCache();
        private:
        	SCacheEntry *FindEntry(char *filename);
        	int num_samples;
                int max_samples;
                SCacheEntry *cache;
};

extern SoundCache scache;

#endif

