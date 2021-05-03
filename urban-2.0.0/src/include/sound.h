/******************************************************************************

    URBAN
    Copyright (C) 1999  Thomas Nyberg and Jonas Borgstr�m

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
    Thomas Nyberg					Jonas Borgstr�m
    Br�nnbergsv�gen 21b					Bondev�gen 6
    92136 Lycksele					92332 Storuman
    Sweden						Sweden

    or email:

    thomas.nyberg@usa.net				jonas_b@bitsmart.com
*****************************************************************************/
#ifndef __SOUND_H__
#define __SOUND_H__

#include "scache.h"

class Sound {
	public:
		Sound();
        	~Sound();

                int PlayMusic(char *);
                void StopMusic();
#ifdef DJGPP
                void SetMusicVolume(int vol) {set_mod_volume(vol);}
                int GetMusicVolume() {return get_mod_volume();}
#else
                void SetMusicVolume(int vol) {md_musicvolume = vol / 2;}
                int GetMusicVolume() {return md_musicvolume * 2;}
#endif
                void PlaySFX_Critical(char *);
                void PlaySFX(char *);
                void LoadSFX(char *);

	private:
#ifdef DJGPP
        	JGMOD *current_mod;
#else
        	MODULE *current_mod;
#endif
                SoundCache scache;
                int soundcard;
};
#endif
