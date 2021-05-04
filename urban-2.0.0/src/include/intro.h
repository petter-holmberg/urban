#ifndef __INTRO_H__
#define __INTRO_H__

#include "icache.h"

class Intro {
public:
    Intro();
    ~Intro();
    void RunIntro();

private:
    BITMAP** images;
    RGB palette[256];
    RGB black[256];
};

#endif
