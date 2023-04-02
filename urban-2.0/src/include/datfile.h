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
#ifndef __DATFILE_H__
#define __DATFILE_H__

#include "allegro.h"
#include <string>

struct dat_entry {
    char filename[120];
    int32_t size, offset;
} __attribute__((packed));

struct header {
    int32_t magic;
    int32_t dat_start;
    int32_t num_entries;
} __attribute__((packed));

class datfile {
public:
    datfile(const char* filenam);
    ~datfile();
    datfile(const datfile&) = delete;
    datfile& operator=(const datfile&) = delete;
    BITMAP* load_pcx(const std::string& filename, PALETTE& pal);
    FILE* open_file(const std::string& filename);
    char* load_file_to_memory(const std::string& filename);
    int GetNumEntries() { return num_entries; };
    struct dat_entry** GetEntries() { return entries; };

private:
    dat_entry* entries[2000] {};
    FILE* datfd {};
    int num_entries {};
};
#endif
