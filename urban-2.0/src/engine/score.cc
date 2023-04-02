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
#include "score.h"
#include "allegro.h"
#include "engine.h"
#include "urbfont.h"

inline constexpr auto BOSSHEALTH_HEIGHT = SCREEN_WIDTH - (2 * 20);
/*************************************************************************/
ScoreBoard::ScoreBoard()
{
    char filename[512];
    PALETTE pal;

    for (int i = 0; i < 12; i++) {

        sprintf(filename, "head/head%d.pcx", i + 1);
        face[i] = icache.GetImage(filename, pal);
    }
    for (int i = 0; i < 3; i++) {
        switch (i) {
        case 0:
            sprintf(filename, "items/bluecard.pcx");
            break;
        case 1:
            sprintf(filename, "items/greencar.pcx");
            break;
        case 2:
            sprintf(filename, "items/redcard.pcx");
            break;
        }
        cards[i] = icache.GetImage(filename, pal);
    }
    BossHealth = 0;
    BossMax = -1;

    Score = tmp_score = 0;
}
/*************************************************************************/
ScoreBoard::~ScoreBoard()
{
}
/*************************************************************************/
void ScoreBoard::Display(BITMAP* bitmap)
{
    PALETTE pal;
    static UrbanFont* fnt = nullptr;
    static BITMAP* board = nullptr;
    char buffer[80];

    if (board == nullptr) {
        board = icache.GetImage("scoreb.pcx", pal);
    }
    if (fnt == nullptr) {
        fnt = new UrbanFont(SMALL_FONT);
    }

    if ((Score - 99) > tmp_score) {
        tmp_score += 99;
    }

    if ((Score - 4) > tmp_score) {
        tmp_score += 4;
    }

    if (Score > tmp_score) {
        tmp_score++;
    }

    rectfill(bitmap, 26 + (44 * engine->player->GetEnergy() / 300), 23, 27 + 44, 34, 19);
    rectfill(bitmap, 26, 23, 27 + (44 * engine->player->GetEnergy() / 300), 34, 36);
    masked_blit(board, bitmap, 0, 0, 0, 0, board->w, board->h);

    sprintf(buffer, "Score:%6ld", tmp_score);
    fnt->print(buffer, 212, 10, bitmap);
    if ((dynamic_cast<player_o*>(engine->player))->HaveCard(blue) != 0) {
        blit(cards[0], bitmap, 0, 0, 237, 40, cards[0]->w, cards[0]->h);
    }
    if ((dynamic_cast<player_o*>(engine->player))->HaveCard(green) != 0) {
        blit(cards[1], bitmap, 0, 0, 212, 40, cards[1]->w, cards[1]->h);
    }
    if ((dynamic_cast<player_o*>(engine->player))->HaveCard(red) != 0) {
        blit(cards[2], bitmap, 0, 0, 262, 40, cards[2]->w, cards[2]->h);
    }

    sprintf(buffer, "%d", (dynamic_cast<player_o*>(engine->player))->GetLives());
    fnt->print(buffer, 32, 40, bitmap);

    if ((dynamic_cast<player_o*>(engine->player))->GetAmmo() < 0) {
        sprintf(buffer, "Inf");
    } else {
        sprintf(buffer, "%d", (dynamic_cast<player_o*>(engine->player))->GetAmmo());
    }

    fnt->print(buffer, 27, 4, bitmap);

    int head = (10 * (300 - engine->player->GetEnergy()) / 300);

    if ((dynamic_cast<player_o*>(engine->player))->IsImmortal() != 0) {
        head = 11;
    }

    masked_blit(face[head], bitmap, 0, 0, SCREEN_WIDTH / 2 - 27, 8, face[head]->w, face[head]->h);

    if (BossMax != -1) {
        /* Vertical bar on the right */
        /*	        rectfill(bitmap, bitmap->w - 20, 69, bitmap->w - 5, 72 + BOSSHEALTH_HEIGHT, 0);
	        rectfill(bitmap, bitmap->w - 19, 70 + BOSSHEALTH_HEIGHT - (BossHealth * BOSSHEALTH_HEIGHT / BossMax), bitmap->w - 6, 70 + BOSSHEALTH_HEIGHT, 36);*/
        /* Horizontal bar on the left */
        rectfill(bitmap, 20 - 1, 65 - 1, 20 + BOSSHEALTH_HEIGHT + 1, 65 + 7 + 1, 0);
        rectfill(bitmap, 20, 65, 20 + (BossHealth * BOSSHEALTH_HEIGHT / BossMax), 65 + 7, 36);
    }
}
/*************************************************************************/
void ScoreBoard::AddScore(long score)
{

    Score += score;
}
/*************************************************************************/
void ScoreBoard::InitBossHealth(int max)
{
    BossMax = max;
}
/*************************************************************************/
void ScoreBoard::SetBossHealth(int health)
{
    BossHealth = health;
}
/*************************************************************************/
