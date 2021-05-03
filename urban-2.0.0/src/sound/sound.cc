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
#include <allegro.h>
#ifdef DJGPP
  #include <jgmod.h>
#else
  #include <mikmod.h>
  #include <pthread.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include "sound.h"
#include "config.h"
#include "scache.h"

extern Config *config;
#ifndef DJGPP

static pthread_t playthread;

extern "C" {
	void *module_thread(void *arg) {
	   	while(1) {
	     		MikMod_Update();
			usleep(10);
		}
		return NULL;
	};
}
#endif

#define NUM_VOICES	32
#define NUM_MOD_VOICES	12
#define NUM_SFX_VOICES	(NUM_VOICES - NUM_MOD_VOICES)

Sound::Sound() {
        current_mod = NULL;

#ifdef DJGPP
       	soundcard = 1;
	reserve_voices(NUM_VOICES, -1);

        if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) < 0) {

        	soundcard = 0;
        }
        if (install_mod(NUM_MOD_VOICES) < 0) {

        	soundcard = 0;
	}
#else
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

	if(MikMod_Init("")) {
        	fprintf(stderr,"Could not initialize sound, reason: %s\n",
                	MikMod_strerror(MikMod_errno));

		soundcard = 0;
		return;
	}
        MikMod_SetNumVoices(12, 4);
   	MikMod_EnableOutput();

	pthread_create(&playthread, NULL, module_thread, NULL);
#endif
}

Sound::~Sound() {
	if(current_mod)
#ifdef DJGPP
        	destroy_mod(current_mod);
#else
        	Player_Free(current_mod);
#endif
}

int Sound::PlayMusic(char *filename) {
	if(soundcard) {
#ifdef DJGPP
		if (current_mod)
	        	destroy_mod(current_mod);

	        if ((current_mod = load_mod(filename)) == NULL)
	        	return -3;

		play_mod(current_mod, 1);
		set_mod_volume(config->keyconf.music_vol);
#else
		if (current_mod)
	        	Player_Free(current_mod);

	        if ((current_mod = Player_Load(filename,NUM_MOD_VOICES,0)) == NULL)
	        	return -3;
		current_mod->wrap = true;
		Player_Start(current_mod);
#endif
	}
	return 1;
}

void Sound::StopMusic() {
	if(soundcard) {
#ifdef DJGPP
		stop_mod();
#else
		Player_Stop();
#endif
		current_mod = NULL;
	}
}

void Sound::LoadSFX(const char *filename) {
	SAMPLE *s;

	if(soundcard) {

	        s = scache.GetSample(filename);
                if(s)
	                scache.FreeSample(s);
        }
}

void Sound::PlaySFX_Critical(const char *filename) {
	SAMPLE *s;

	if(soundcard) {

	        s = scache.GetSample(filename);
                if(s) {
#ifdef DJGPP
		        play_sample(s, config->keyconf.sfx_vol, 128, 1000, 0);
#else
			md_sndfxvolume = config->keyconf.sfx_vol;
		        Sample_Play(s, 0, SFX_CRITICAL);
#endif
		}
	}
}
void Sound::PlaySFX(const char *filename) {
	SAMPLE *s;

	if(soundcard) {

	        s = scache.GetSample(filename);
                if(s) {
#ifdef DJGPP
		        play_sample(s, config->keyconf.sfx_vol, 128, 1000, 0);
#else
			md_sndfxvolume = config->keyconf.sfx_vol;
		        Sample_Play(s, 0, 0);
#endif
		}
	}
}

