#ifndef __DATFILE_H__
#define __DATFILE_H__

#include <cstdint>
#include <allegro.h>
#include <stdio.h>

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
	        datfile(const char *filenam);
	        ~datfile();
	        BITMAP *load_pcx(const char *filename, RGB *pal);
		FILE *open_file (const char *filename);
		char *load_file_to_memory (const char *filename);
                int GetNumEntries() {return num_entries;};
                struct dat_entry **GetEntries() {return entries;};
        private:
		struct dat_entry *entries[2000];
		FILE *datfd;
                int num_entries;
};
#endif
