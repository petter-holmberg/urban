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
    BITMAP* load_pcx(const std::string& filename, PALETTE& pal);
    FILE* open_file(const std::string& filename);
    char* load_file_to_memory(const std::string& filename);
    int GetNumEntries() { return num_entries; };
    struct dat_entry** GetEntries() { return entries; };

private:
    struct dat_entry* entries[2000] {};
    FILE* datfd;
    int num_entries;
};
#endif
