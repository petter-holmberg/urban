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
#ifndef __SCORE_H__
#define __SCORE_H__

#include "allegro.h"

class ScoreBoard {
	public:
		ScoreBoard();
                void Display(BITMAP *bitmap);
                void AddScore(long score);
                int GetScore() { return Score; };
                void ResetScore() { Score = tmp_score = 0; };
                void SetScore(long score) {Score = score;};
                void InitBossHealth(int max);
                void SetBossHealth(int health);
	        ~ScoreBoard();
        private:
        	long Score;
                long tmp_score;
                long BossHealth;
                long BossMax;
                BITMAP *face[20]{};
                BITMAP *cards[3]{};
};
#endif
