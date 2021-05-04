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
#include "sound.h"
#include "config.h"
#include "scache.h"
#include <allegro.h>
#include <cstdio>
#include <mikmod.h>
#include <pthread.h>
#include <unistd.h>

extern Config* config;

static pthread_t playthread;

extern "C" {
auto module_thread(void* /*arg*/) -> void*
{
    while (1) {
        MikMod_Update();
        usleep(10);
    }
    return nullptr;
};
}

inline constexpr auto NUM_VOICES = 32;
inline constexpr auto NUM_MOD_VOICES = 12;
inline constexpr auto NUM_SFX_VOICES = NUM_VOICES - NUM_MOD_VOICES;

Sound::Sound()
{
    current_mod = nullptr;

    soundcard = 1;
    /* register all the drivers */
    MikMod_RegisterAllDrivers();
    /*MikMod_RegisterDriver(&drv_oss);*/

    setenv("MM_FRAGSIZE", "10", 1);

    /* register all the module loaders */
    MikMod_RegisterAllLoaders();

    /* initialize the library */
    md_mode = DMODE_SOFT_SNDFX | DMODE_SOFT_MUSIC | DMODE_16BITS;
    md_mixfreq = 22050;

    if (MikMod_Init("") != 0) {
        fprintf(stderr, "Could not initialize sound, reason: %s\n",
            MikMod_strerror(MikMod_errno));

        soundcard = 0;
        return;
    }
    MikMod_SetNumVoices(12, 4);
    MikMod_EnableOutput();

    pthread_create(&playthread, nullptr, module_thread, nullptr);
}

Sound::~Sound()
{
    if (current_mod != nullptr) {
        Player_Free(current_mod);
    }
}

auto Sound::PlayMusic(char* filename) -> int
{
    if (soundcard != 0) {
        if (current_mod != nullptr) {
            Player_Free(current_mod);
        }

        if ((current_mod = Player_Load(filename, NUM_MOD_VOICES, 0)) == nullptr) {
            return -3;
        }
        current_mod->wrap = 1;
        Player_Start(current_mod);
    }
    return 1;
}

void Sound::StopMusic()
{
    if (soundcard != 0) {
        Player_Stop();
        current_mod = nullptr;
    }
}

void Sound::LoadSFX(const char* filename)
{
    SAMPLE* s = nullptr;

    if (soundcard != 0) {

        s = scache.GetSample(filename);
        if (s != nullptr) {
            scache.FreeSample(s);
        }
    }
}

void Sound::PlaySFX_Critical(const char* filename)
{
    SAMPLE* s = nullptr;

    if (soundcard != 0) {

        s = scache.GetSample(filename);
        if (s != nullptr) {
            md_sndfxvolume = config->keyconf.sfx_vol;
            Sample_Play(s, 0, SFX_CRITICAL);
        }
    }
}
void Sound::PlaySFX(const char* filename)
{
    SAMPLE* s = nullptr;

    if (soundcard != 0) {

        s = scache.GetSample(filename);
        if (s != nullptr) {
            md_sndfxvolume = config->keyconf.sfx_vol;
            Sample_Play(s, 0, 0);
        }
    }
}
