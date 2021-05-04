#ifndef __INTRO_H__
#define __INTRO_H__

#include "icache.h"

class Intro {
public:
    Intro();
    ~Intro();
    void RunIntro();

private:
    BITMAP** images {};
    PALETTE palette {};
    PALETTE black {};
};

#endif
