#ifndef __OUTTRO_H__
#define __OUTTRO_H__

#include "icache.h"

class Outtro {
public:
    Outtro();
    ~Outtro();
    void RunOuttro();

private:
    BITMAP** images;
    RGB palette[256];
    RGB black[256];
};

#endif
