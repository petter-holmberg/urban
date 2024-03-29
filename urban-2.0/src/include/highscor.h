/******************************************************************************
    URBAN 2.0
    Copyright (C) 2021  Petter Holmberg (petter.holmberg@usa.net)

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
*****************************************************************************/
#ifndef __HIGHSCORE_H__
#define __HIGHSCORE_H__

#include <array>
#include <string>

/***************************************************************************/
inline constexpr auto NUM_HIGHSCORES = 10;
/***************************************************************************/
struct Score_t {
    std::array<char, 64> Name {};
    int Score {};
    int Level {};
};
/***************************************************************************/
class HighScore {
public:
    HighScore(int score, int level);
    HighScore();
    void Reset();
    ~HighScore() = default;

protected:
    void Open();
    void Save();
    static std::string GetName();
    std::array<Score_t, NUM_HIGHSCORES> highscore {};
    FILE* fd {};
};
/***************************************************************************/
#endif
