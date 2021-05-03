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
#include <string.h>
#include <stdlib.h>
#include <allegro.h>
#include "game.h"
#include "engine.h"
#include "object2.h"
#include "object.h"

#define WARNING_SAMPLE "samples/airnuke.wav"
#define MAX_CLUSTER_COUNTER	5
#define MAX_ENERGY		1000
#define NUM_SKADA		4

#if (MAX_ENERGY % NUM_SKADA) != 0
#	error MAX_ENERGY must be a multiple of NUM_SKADA!
#endif

char *comments[] = {
"samples/oldare.wav",
"samples/olneverd.wav",
"samples/olskills.wav",
"samples/olquit.wav",
"samples/olvictor.wav"};


#define __URK ((NUM_SKADA - 1) - (energy <? (MAX_ENERGY - 1)) / (MAX_ENERGY / NUM_SKADA))

#define IMAGE(x) ((x) + 3 * (__URK <? 3))
#define FROMIMAGE(x) ((x) % 3)

#define STATE_NONE		0
#define STATE_MOVE_LEFT		1
#define STATE_MOVE_RIGHT	2
#define STATE_FIRE_MINIGUN	4
#define STATE_FIRE_CLUSTER	8
#define STATE_DEAD		16

#define FIRE_REP		2
#define FIRE_DELAY		1

#define CLUSTER_REP		4
#define CLUSTER_DELAY		20

#define MAX_X_SPEED	2 //4
#define X_FRICTION	1
#define MIN_X_SPEED	-MAX_X_SPEED //-4
#define MAX_Y_SPEED	16
#define MIN_Y_SPEED	-12
#define X_ACCEL		2 //2
#define Y_ACCEL		1
#define Z_ACCEL		2
#define MAX_Z_SPEED	2
#define MIN_Z_SPEED	-MAX_Z_SPEED
#define Z_FRICTION	1

#define COLL_X (coll_x + coll_width)
#define COLL_Y (coll_y + coll_height)


#define MOVE_LEFT		1
#define MOVE_RIGHT		2
#define MOVE_STOP		3

#define FIRE_MINIGUN		4
#define FIRE_CLUSTER		5
#define FIRE_STOP		6

struct AIInfo {
	int what;
        int num;
};

struct AIInfo aimove[] = {
{MOVE_LEFT, 300},
{MOVE_STOP, 200},
{MOVE_RIGHT, 100},
{MOVE_STOP, 50},
{MOVE_RIGHT, 200},
{MOVE_STOP, 200},
};

struct AIInfo aifire[] = {
{FIRE_MINIGUN, 200},
{FIRE_STOP, 200},
{FIRE_CLUSTER, 100},
{FIRE_STOP, 100},
{FIRE_CLUSTER, 80},
{FIRE_MINIGUN, 170},
{FIRE_STOP, 200},
};

#define NUM_AIMOVE ((signed)(sizeof(aimove) / sizeof(aimove[0])))
#define NUM_AIFIRE ((signed)(sizeof(aifire) / sizeof(aifire[0])))

/**************************************************************************/
Tank_o::Tank_o(int X, int Y, int Z) : Object(X, Y, Z) {
	RGB pal[256];
        char filename[512];
        int i;

        anim.reset();

	images = new BITMAP*[26];

        for (i = 0;i < 3;i++) {
        	sprintf(filename, "boss/v/skada0/%d.pcx", i + 1);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 3;i < 6;i++) {
        	sprintf(filename, "boss/v/skada1/%d.pcx", i - 2);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 6;i < 9;i++) {
        	sprintf(filename, "boss/v/skada2/%d.pcx", i - 5);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 9;i < 12;i++) {
        	sprintf(filename, "boss/v/skada3/%d.pcx", i - 8);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 12;i < 15;i++) {
        	sprintf(filename, "boss/h/skada0/%d.pcx", i  - 11);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 15;i < 18;i++) {
        	sprintf(filename, "boss/h/skada1/%d.pcx", i - 14);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 18;i < 21;i++) {
        	sprintf(filename, "boss/h/skada2/%d.pcx", i - 17);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
        for (i = 21;i < 24;i++) {
        	sprintf(filename, "boss/h/skada3/%d.pcx", i - 20);
                images[i] = icache.GetImage(filename, pal);
                if (images[i])
                	num_images++;
	}
       	sprintf(filename, "boss/v/dead.pcx");
	images[24] = icache.GetImage(filename, pal);
	if (images[24])
		num_images++;
	sprintf(filename, "boss/h/dead.pcx");
	images[25] = icache.GetImage(filename, pal);
	if (images[25])
		num_images++;

        current_image = 0;

        height = images[0]->h - 19;
        width = images[0]->w;
        coll_x = 21;
        coll_y = 0;
        coll_width = width - coll_x;
        coll_height = height;

        //stå med fötterna
        y -= images[0]->h;

        ENGINE.create_object((wheel1 = new TankWheel_o(x, y, z - TILE_TOP_HEIGHT)));
        ENGINE.create_object((wheel2 = new TankWheel_o(x, y, z + TILE_TOP_HEIGHT)));

        energy = MAX_ENERGY;
        strength = 10;
        speed_x = 0;
        speed_y = 0;
        speed_z = 0;
	direction = LEFT_DIR;
        state = STATE_NONE;
        counter = 0;
        counter2 = 0;

        counter3 = 0; //aimove
        counter4 = 0; //aifire

        counter5 = 0; //har vi inte startat för första gången?

        score = 0;
	friends = FRIEND_TANK;
        enemies = ~0;
        me = FRIEND_TANK;
//k * TILE_TOP_HEIGHT + (TILE_TOP_HEIGHT >> 1)
}


int Tank_o::update() {
	static int aim = 0;
        static int aif = 0;
        static int cluster_counter = 0;
//	int r;

        if (!counter5) {
        	char buf2[2048];
        	ENGINE.score.InitBossHealth(MAX_ENERGY);
                ENGINE.score.SetBossHealth(MAX_ENERGY);
                counter5 = 1;
                ENGINE.SetMaxUpdateX(2000);
                ENGINE.create_alwaysupdate(this);

		sprintf(buf2, "%s/snd/modules/boss.xm", DATPATH);
                SOUND.PlayMusic(buf2);
		SOUND.PlaySFX_Critical(comments[0]);
                return REMOVE_ME;
        } else
        	score = 50000;

	if (counter)
        	counter--;

        if (!counter && state & STATE_DEAD) {
                wheel1->DestroyTank();
                wheel2->DestroyTank();
        	// Hide HealthMeter
        	ENGINE.score.InitBossHealth(-1);
                ENGINE.create_dekoration(this);
		ENGINE.ClearLevel();
                return REMOVE_ME;
	}

	if (state & STATE_DEAD) {
               	ENGINE.create_effect(new SmallExplosion_o(x + random() % width, y + random() % height, z));

                if ((random() % 14) == 0) {
		       	ENGINE.create_object(new explosion_o(x + random() % width, y + random() % height, z));
                }
                if (counter == 200) {
			ENGINE.PushMessage("Airstrike alert!");
			SOUND.PlaySFX_Critical(WARNING_SAMPLE);

                }
                if (counter < 25) {
			ENGINE.create_object(new explosion_o(x + random() % width, y + random() % height, z));
			ENGINE.EnableEarthquake();
			ENGINE.EnableEarthquake();
			ENGINE.EnableEarthquake();
		}
		
                return 0;
        }

	if (energy <= 0) {
        	strength = 0;
		state = STATE_DEAD;
                counter = 300;
                current_image = direction == LEFT_DIR ? 24 : 25;
                return 0;
        }

	if (counter2)
        	counter2--;
	if (counter3)
        	counter3--;
	if (counter4)
        	counter4--;

	if (!counter3) {
        	state &= ~(STATE_MOVE_LEFT | STATE_MOVE_RIGHT);

        	switch (aimove[aim].what) {
                	case MOVE_LEFT:
                        	state |= STATE_MOVE_LEFT;
                                break;
                        case MOVE_RIGHT:
                        	state |= STATE_MOVE_RIGHT;
                                break;
                        case MOVE_STOP:
                        /* fallthrough */
                        default:
                        	break;
		}
                counter3 = aimove[aim].num;
                aim++;
                if (aim >= NUM_AIMOVE)
                	aim = 0;
	}
        if (!counter4) {
        	state &= ~(STATE_FIRE_MINIGUN | STATE_FIRE_CLUSTER);
		current_image = IMAGE(0);
                switch (aifire[aif].what) {
                	case FIRE_MINIGUN:
                        	state |= STATE_FIRE_MINIGUN;
                        	break;
                        case FIRE_CLUSTER:
                        	state |= STATE_FIRE_CLUSTER;
                        	break;
                        case FIRE_STOP:
                        /* fallthrough */
                        default:
                        	break;
		}
                counter4 = aifire[aif].num;
                aif++;
                if (aif >= NUM_AIFIRE)
                	aif = 0;
	}

        if (state & STATE_MOVE_RIGHT) {
               	current_image = IMAGE(0);
               	switch (direction) {
                       	case LEFT_DIR:
				x++;
				wheel1->MoveRight(x);
                                wheel2->MoveRight(x);
				break;
                        case RIGHT_DIR:
                        default:
                               	break;
		};
        } else if (state & STATE_MOVE_LEFT) {
		current_image = IMAGE(0);
		switch (direction) {
			case LEFT_DIR:
				x--;
				wheel1->MoveLeft(x);
				wheel2->MoveLeft(x);
				break;
			case RIGHT_DIR:
			default:
				break;
		};
        }

        if (state & STATE_FIRE_MINIGUN) {
		switch (direction) {
			case LEFT_DIR:
				if (counter2 == 0 && current_image == IMAGE(0)) {
					current_image = IMAGE(2);
					counter2 = FIRE_REP;
					ENGINE.create_alwaysupdate(new HighSpeed_Bullet_o(x + 20, y + 54, z, direction, me, 5));
				} else if (counter2 == 0 && current_image == IMAGE(2)) {
					counter2 = FIRE_DELAY;
					current_image = IMAGE(0);
				}
				break;
			case RIGHT_DIR:
				break;
			default:
				break;
		}
        } else if (state & STATE_FIRE_CLUSTER) {
		switch (direction) {
			case LEFT_DIR:
				if (counter2 == 0 && current_image == IMAGE(0)) {
					current_image = IMAGE(1);
					counter2 = CLUSTER_REP;
					ENGINE.create_alwaysupdate(new Cluster_o(x + 67, y + 5, z, -(3 + cluster_counter), -(3 + cluster_counter), 0, me));
					cluster_counter++;
					if (cluster_counter > MAX_CLUSTER_COUNTER)
                                        	cluster_counter = 0;
				} else if (counter2 == 0 && current_image == IMAGE(1)) {
					current_image = IMAGE(0);
					counter2 = CLUSTER_DELAY;
				}
				break;
			case RIGHT_DIR:
			default:
				break;
		}
        }

        // Fall or Stop
	if (ENGINE.check_floor(x, y + height, z) ||
		ENGINE.check_floor(x + width, y + height, z)) {
        	speed_y = 0;
        } else if (speed_y < MAX_Y_SPEED) {
        	speed_y += Y_ACCEL;
                speed_z = 0;
        }

        x += speed_x;
        y += speed_y;
        z += speed_z;

        if (x < 0)
        	x = 0;


	if (z > MIN_Z) {
        	z = MIN_Z;
                speed_z = 0;
                counter = 0;
	}
	if (z < MAX_Z) {
        	z = MAX_Z;
                speed_z = 0;
                counter = 0;
	}

	layer = z / TILE_TOP_HEIGHT;


	return 0;
}

void Tank_o::Collision(Object *o) {
	static int cmc = 1;

        if (!energy)
        	return;

	if (o->GetWho() & (ENEMY_FIREBALL | ENEMY_BEAM | ENEMY_ICEBALL | ENEMY_PLASMA))
        	return;

	if (o->GetEnemies() & me) {
		if (o->GetWho() & ENEMY_EXPLOSION)
	        	energy -= (o->GetStrength() / 2);
		else
			energy -= o->GetStrength();
	}

        if (energy < 0)
        	energy = 0;

	ENGINE.score.SetBossHealth(energy);

        if (IMAGE(FROMIMAGE(current_image)) != current_image) {
        	SOUND.PlaySFX_Critical(comments[cmc++]);
        }
	current_image = IMAGE(FROMIMAGE(current_image));

}

/**************************************************************************/
Tank_o::~Tank_o() {
}
/**************************************************************************/
