#ifndef __OBJECT2_H__
#define __OBJECT2_H__

#include "object.h"


class ComputerScreen_o:public Object {
	public:
        	ComputerScreen_o(int X, int Y, int Z, int Type = 0);
                int update();
                void Collision(Object *);
                virtual ~ComputerScreen_o();
        private:
};

class Dekor_o:public Object {
	public:
        	Dekor_o(int X, int Y, int Z, char *filename);
                int update();
                void Collision(Object *);
                virtual ~Dekor_o();
        private:
};

class SmallExplosion_o:public Object {
	public:
		SmallExplosion_o(int X, int Y, int Z);
                int update();
		virtual ~SmallExplosion_o();
                void Collision(Object *);
	private:
        	Anim anim;
};


class DrGreen_o:public Object {
	public:
        	DrGreen_o(int X, int Y, int Z);
                int update();
                virtual ~DrGreen_o();
                void Collision(Object *);
	private:
        	Anim anim;
};

class Boss_DrGreen_o:public DrGreen_o {
	public:
		Boss_DrGreen_o(int X, int Y, int Z);
                int update();
		virtual ~Boss_DrGreen_o();
};


class Points_o:public Object {
	public:
        	Points_o(int X, int Y, int Z, int score = 0);
                int update();
                virtual ~Points_o();
                void Collision(Object *);
	private:
};

class Plasma_o:public Object {
	public:
        	Plasma_o(int X, int Y, int Z, int Dir, int Evilness = EVIL_ENEMY, int Damage = 1);
		int update();
		void Collision(Object *);
                virtual ~Plasma_o();
};

class Seagal_o:public Object {
	public:
        	Seagal_o(int X, int Y, int Z);
                int update();
                virtual ~Seagal_o();
                void Collision(Object *);
	private:
        	Anim anim;
};

class Boss_Seagal_o:public Seagal_o {
	public:
		Boss_Seagal_o(int X, int Y, int Z);
                int update();
		virtual ~Boss_Seagal_o();
};

class Missile_o:public Object {
	public:
        	Missile_o(int X, int Y, int Z, int Speed);
		void Collision(Object *);
                int update();
                virtual ~Missile_o();
	private:
        	Anim anim;
};

class ElevatorStation_o:public Object {
	public:
        	ElevatorStation_o(int X, int Y, int Z, int dir = UP_DIR, Cardtype Card = none);
                void Collision(Object *);
                int update();
                virtual ~ElevatorStation_o();

                /* Specifikt för styrsystemet */
                int StartElevator();
		void SendElevator(int dir);

		void SetElevatorStation(Object *);
	private:
        	Object *elev_stat;
        	Object *elevator;
                Object *wire_up;
                Object *wire_down;
        	Cardtype card;
        	Anim anim;
                int direction_to_send;
};

class Elevator_o:public Object {
	public:
        	Elevator_o(int X, int Y, int Z);
                void Collision(Object *);
                int update();
                virtual ~Elevator_o();

                /* Specifikt för hiss */
                void Activate(int dir);
                void DeActivate();
                void SendElevator(int dir);
	private:
        	Object *passenger;
                Object *elev_controller;
                Anim anim;
};

class ElevatorWire_o:public Object {
	public:
        	ElevatorWire_o(int X, int Y, int Z);
                void Collision(Object *);
                int update();
                virtual ~ElevatorWire_o();

                /* Specifikt för hissledningarna */
                void FindElevatorStation(Object *w, Object *sender);
	private:
        	Object *elev_controller;
                Object *wire_up, *wire_down;

                Anim anim;
};

class Cluster_o:public Object {
	public:
        	Cluster_o(int X, int Y, int Z, int SpeedX = 0, int SpeedY = 0, int SpeedZ = 0, int Friends = 0);
                void Collision(Object *);
                int update();
                virtual ~Cluster_o();
};

class ClusterGrenade_o:public Object {
	public:
        	ClusterGrenade_o(int X, int Y, int Z, int SpeedX = 0, int SpeedY = 0, int SpeedZ = 0, int Friends = 0);
		void Collision(Object *);
                int update();
                virtual ~ClusterGrenade_o();
};

class soldier4_o:public Object {
	public:
        	soldier4_o(int X, int, int Y, int Z);
        	soldier4_o(int X, int Y, int Z);
                int update();
                virtual ~soldier4_o();
                void Collision(Object *);
	private:
                Anim anim;
};

class soldier3_o:public Object {
	public:
        	soldier3_o(int X, int, int Y, int Z);
        	soldier3_o(int X, int Y, int Z);
                int update();
                virtual ~soldier3_o();
                void Collision(Object *);
	private:
                Anim anim;
};

class ExplosiveTile_o:public Object {
	public:
        	ExplosiveTile_o(int X, int Y, int Z, int Type = 0);
                int update();
                virtual ~ExplosiveTile_o();
                void Collision(Object *);
	private:
        	int type;
};

class Water_o:public Object {
	public:
        	Water_o(int X, int Y, int Z);
                void Collision(Object *);
                int update();
                virtual ~Water_o();
	private:
                Anim anim;
};

class BubbleMaker_o:public Object {
	public:
        	BubbleMaker_o(int X, int Y, int Z);
                void Collision(Object *);
                int update();
                virtual ~BubbleMaker_o();
	private:
};

class Bubble_o:public Object {
	public:
        	Bubble_o(int X, int Y, int Z);
		int update();
                void Collision(Object *);
                virtual ~Bubble_o();
	private:
        	Anim anim;
};


class Airstrike_o:public Object {
	public:
        	Airstrike_o(int X, int Y, int Z);
                int update();
                void Collision(Object *);
                virtual ~Airstrike_o();
};

class AirExplosion_o:public Object {
	public:
        	AirExplosion_o(int X, int Y, int Z, int Speed_X = 0, int Speed_Y = 0, int Speed_Z = 0);
                int update();
		virtual ~AirExplosion_o();
                void Collision(Object *);
	private:
        	Anim anim;
};

class AirGrenade_o:public Object {
	public:
        	AirGrenade_o(int X, int Y, int Z, int SpeedX = 0, int SpeedY = 0, int SpeedZ = 0);
		void Collision(Object *);
                int update();
                virtual ~AirGrenade_o();
};

class Beam_o:public Object {
	public:
        	Beam_o(int X, int Y, int Z, int Friends, int SpeedX = 0, int SpeedY = 0);
                void Collision(Object *);
                int update();
                virtual ~Beam_o();
	private:
        	Anim anim;
};

class TankWheel_o:public Object {
	public:
        	TankWheel_o(int X, int Y, int Z);
                void Collision(Object *);
                int update();
                virtual ~TankWheel_o();

                /* unikt för wheel */
                void MoveLeft(int X);
                void MoveRight(int X);
                void DestroyTank();
	private:
        	Anim anim;
};

class Tank_o:public Object {
	public:
        	Tank_o(int X, int Y, int Z);
                void Collision(Object *);
                int update();
                virtual ~Tank_o();
	private:
        	Anim anim;
                TankWheel_o *wheel1, *wheel2;
};


class TeleDoor_o:public Object {
	public:
        	TeleDoor_o(int X, int Y, int Z);
                void Collision(Object *);
                int update();
                virtual ~TeleDoor_o();
	private:
        	Anim anim;
};

#endif
