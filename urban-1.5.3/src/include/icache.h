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
#ifndef __ICACHE_H__
#define __ICACHE_H__

#include <allegro.h>
#include "datfile.h"

struct CacheEntry {
     	char *filename;
        BITMAP *bitmap;
        int count;
        RGB *pal;
};


class ImageCache {
	public:
        	ImageCache();
                BITMAP *GetImage(char *filename, RGB *pal);
                void FreeImage(BITMAP *bitmap);
                ~ImageCache();
        private:
        	CacheEntry *FindEntry(char *filename);
        	int num_images;
                int max_images;
                CacheEntry *cache;
		datfile *dat;

};

extern ImageCache icache;

#endif

