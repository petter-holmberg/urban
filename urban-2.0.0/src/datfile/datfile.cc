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
#include "datfile.h"
#include "allegro.h"
#include "game.h"
#include "icache.h"
#include <string>

struct pcx_header {
    uint8_t manufacturer;
    uint8_t version;
    uint8_t encoding;
    uint8_t bits_per_pixel;
    uint8_t xmin[2], ymin[2];
    uint8_t xmax[2], ymax[2];
    uint8_t hres[2];
    uint8_t vres[2];

    uint8_t palette16[48];
    uint8_t reserved;
    uint8_t color_planes;
    uint8_t bytes_per_line[2];
    uint8_t palette_type[2];
    uint8_t filler[58];
} __attribute__((packed));

/***************************************************************************/
auto datfile::load_file_to_memory(const std::string& filename) -> char*
{
    int found = 0;
    int i = 0;

    for (i = 0; i < num_entries; i++) {
        if (filename == entries[i]->filename) {
            found = 1;
            fseek(datfd, entries[i]->offset, SEEK_SET);
            break;
        }
    }
    if (found == 0) {
        /*        	printf("\nFile not found in dat:'%s'\n", filename);
                exit(2);*/
        return nullptr;
    }
    char* buf = new char[entries[i]->size];

    if (buf == nullptr) {
        return nullptr;
    }

    if (fread(buf, 1, entries[i]->size, datfd) != (size_t)entries[i]->size) {

        delete[] buf;
        return nullptr;
    }
    return buf;
}
/***************************************************************************/
auto datfile::open_file(const std::string& filename) -> FILE*
{
    int found = 0;

    for (int i = 0; i < num_entries; i++) {
        if (filename == entries[i]->filename) {
            found = 1;
            fseek(datfd, entries[i]->offset, SEEK_SET);
            break;
        }
    }
    if (found == 0) {
        /*        	printf("\nFile not found in dat:'%s'\n", filename);
                exit(2);*/
        return nullptr;
    }
    return datfd;
}
/***************************************************************************/
auto datfile::load_pcx(const std::string& filename, PALETTE& pal) -> BITMAP*
{
    pcx_header header {};
    uint32_t bpp = 0;
    int32_t i = 0;
    int32_t j = 0;
    int32_t c = 0;
    int32_t err = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t cpl = 0;
    uint8_t* lptr = nullptr;
    uint8_t* nextlptr = nullptr;
    uint8_t palbuf[4];
    BITMAP* bmp = nullptr;
    char found = 0;

    err = 0;

    for (i = 0; i < num_entries; i++) {
        if (filename == entries[i]->filename) {
            found = 1;
            fseek(datfd, entries[i]->offset, SEEK_SET);
            break;
        }
    }
    if (found == 0) {
        printf("\nFile not found in dat:'%s'\n", filename.c_str());
        exit(2);
    }

    auto err2 = fread((char*)&header, 1, sizeof(pcx_header), datfd);

    width = get16(header.xmax) - get16(header.xmin) + 1;
    height = get16(header.ymax) - get16(header.ymin) + 1;

    bpp = 1;

    cpl = header.color_planes;

    if ((err == 0) && ((bmp = create_bitmap(width, height)) != nullptr)) {
        lptr = (uint8_t*)bmp->dat;

        for (i = 0; i < (signed)height; i++) {
            nextlptr = lptr + width;
            do {

                c = fgetc(datfd) & 0xFF;
                if ((c & 0xC0) == 0xC0) {

                    j = c & 0x3F;
                    c = fgetc(datfd);
                    while ((j--) != 0) {
                        *lptr++ = c;
                    }
                } else {
                    *lptr++ = c;
                }
            } while (lptr < nextlptr);
            lptr = nextlptr;
        }

        if (fgetc(datfd) == 12) {
            for (i = 0; i < 256; i++) {

                err = fread(palbuf, 1, 3, datfd);
                pal[i].r = palbuf[0] >> 2;
                pal[i].g = palbuf[1] >> 2;
                pal[i].b = palbuf[2] >> 2;
            }
        }
    }
    return bmp;
}
/***************************************************************************/
datfile::datfile(const char* filename)
{
    header hdr {};
    char buffer[512];

    num_entries = 0;

    sprintf(buffer, "%s/%s", DATPATH, filename);

    if ((datfd = fopen(buffer, "rb")) == nullptr) {

        perror(buffer);
        exit(6);
    }
    auto err = fread(&hdr, sizeof(header), 1, datfd);

    if (hdr.magic != 0x5552424E) {

        printf("Not a URBAN datfile\n");
        exit(7);
    }
    num_entries = hdr.num_entries;

    fseek(datfd, hdr.dat_start, SEEK_SET);

    for (int i = 0; i < hdr.num_entries; i++) {

        entries[i] = new dat_entry;
        err = fread(entries[i], sizeof(dat_entry), 1, datfd);
    }
}
/***************************************************************************/
datfile::~datfile()
{
    for (int i = 0; i < num_entries; i++) {

        delete entries[i];
    }
    fclose(datfd);
}
