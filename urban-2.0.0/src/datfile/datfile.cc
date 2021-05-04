#include "datfile.h"
#include "game.h"
#include "icache.h"
#include <allegro.h>
#include <stdio.h>
#include <string.h>

//#ifdef DJGPP
typedef unsigned char uint8;
typedef unsigned long uint32;
typedef signed long sint32;
//#endif

struct pcx_header {
    uint8 manufacturer;
    uint8 version;
    uint8 encoding;
    uint8 bits_per_pixel;
    uint8 xmin[2], ymin[2];
    uint8 xmax[2], ymax[2];
    uint8 hres[2];
    uint8 vres[2];

    uint8 palette16[48];
    uint8 reserved;
    uint8 color_planes;
    uint8 bytes_per_line[2];
    uint8 palette_type[2];
    uint8 filler[58];
} __attribute__((packed));

/***************************************************************************/
char* datfile::load_file_to_memory(const char* filename)
{
    int found = 0;
    int i;

    for (i = 0; i < num_entries; i++) {
        if (!strcmp(filename, entries[i]->filename)) {
            found = 1;
            fseek(datfd, entries[i]->offset, SEEK_SET);
            break;
        }
    }
    if (!found) {
        /*        	printf("\nFile not found in dat:'%s'\n", filename);
                exit(2);*/
        return NULL;
    }
    char* buf = new char[entries[i]->size];

    if (buf == NULL)
        return NULL;

    if (fread(buf, 1, entries[i]->size, datfd) != (size_t)entries[i]->size) {

        delete[] buf;
        return NULL;
    }
    return buf;
}
/***************************************************************************/
FILE* datfile::open_file(const char* filename)
{
    int found = 0;

    for (int i = 0; i < num_entries; i++) {
        if (!strcmp(filename, entries[i]->filename)) {
            found = 1;
            fseek(datfd, entries[i]->offset, SEEK_SET);
            break;
        }
    }
    if (!found) {
        /*        	printf("\nFile not found in dat:'%s'\n", filename);
                exit(2);*/
        return NULL;
    }
    return datfd;
}
/***************************************************************************/
BITMAP* datfile::load_pcx(const char* filename, RGB* pal)
{
    struct pcx_header header;
    uint32 bpp;
    sint32 i, j, c, err;
    uint32 width, height, cpl;
    uint8 *lptr, *nextlptr, palbuf[4];
    BITMAP* bmp = NULL;
    char found = 0;

    err = 0;

    for (i = 0; i < num_entries; i++) {
        if (!strcmp(filename, entries[i]->filename)) {
            found = 1;
            fseek(datfd, entries[i]->offset, SEEK_SET);
            break;
        }
    }
    if (!found) {
        printf("\nFile not found in dat:'%s'\n", filename);
        exit(2);
    }

    auto err2 = fread((char*)&header, 1, sizeof(struct pcx_header), datfd);
    //printf("%s %d\n", err2, filename);
    width = get16(header.xmax) - get16(header.xmin) + 1;
    height = get16(header.ymax) - get16(header.ymin) + 1;

    bpp = 1;

    cpl = header.color_planes;

    if ((!err) && (bmp = create_bitmap(width, height))) {
        //	if ((!err) && (fbuf = (uint8 *) malloc (width * height * bpp)) ) {
        //        	bmp = create_bitmap(width, height);
        lptr = (uint8*)bmp->dat;

        for (i = 0; i < (signed)height; i++) {
            nextlptr = lptr + width;
            do {

                c = fgetc(datfd) & 0xFF;
                if ((c & 0xC0) == 0xC0) {

                    j = c & 0x3F;
                    c = fgetc(datfd);
                    while (j--)
                        *lptr++ = c;
                } else
                    *lptr++ = c;
            } while (lptr < nextlptr);
            lptr = nextlptr;
        }
        //		memcpy(bmp->dat, fbuf, width * height);

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
    struct header hdr;
    char buffer[512];

    num_entries = 0;

    sprintf(buffer, "%s/%s", DATPATH, filename);

    if ((datfd = fopen(buffer, "rb")) == NULL) {

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
/***************************************************************************/
//#define __TEST__
#ifdef __TEST__
#define _main main
#endif
#ifdef __TEST__

void _main()
{
    ImageCache icache;
    char buffer[512];
    int i, oi;
    //datfile dat("../urban.dat");
    allegro_init();
    install_keyboard();
    install_timer();
    set_gfx_mode(GFX_MODEX, 320, 240, 0, 0);
    PALETTE pal;
    BITMAP* test;
    FILE* f;
    char* urk;
    /*
	f = fopen("cp.pcx", "wb");
	for(i = 0; i < test_num_entries; i++) {
		if(!strcmp("gfx/items/xlife.pcx", test_ent[i]->filename)) {
                	fseek(f123, test_ent[i]->offset, SEEK_SET);
                        urk = (char *)malloc(test_ent[i]->size);
                        fread(urk, test_ent[i]->size, 1, f123);
                        fwrite(urk, test_ent[i]->size, 1, f);
			fclose(f);
                        exit(129);

                	break;
		}
        }
*/

    test = icache.GetImage("items/xlife.pcx", pal);
    set_palette(pal);
    clear(screen);
    blit(test, screen, 0, 0, 0, 0, test->w, test->h);
    while (!key[KEY_ESC])
        ;
    _exit(0);

    i = 0;
    oi = -1;
    while (!key[KEY_ESC]) {
        if (key[KEY_UP] && i < test_num_entries)
            i++;
        if (key[KEY_DOWN] && i > 0)
            i--;
        if (oi != i) {
            oi = i;
            test = icache.GetImage(test_ent[i]->filename + 4, pal);
            set_palette(pal);
            clear(screen);
            blit(test, screen, 0, 0, 0, 0, test->w, test->h);
        }
        rest(50);
    }
    //        for (int i = test_num_entries - 1;i;i--) {
    //        	sprintf(buffer, "../%s", test_ent[i]->filename);
    //	}
    /*        for(int i = 0; i < test_num_entries; i++) {
		test = dat.load_pcx(test_ent[i]->filename, pal);
	        set_palette(pal);

		blit(test, screen, 0, 0, 0, 0, test->w, test->h);
//	        rest(500);
//	        destroy_bitmap(test);
	}*/

    set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
}
#endif
