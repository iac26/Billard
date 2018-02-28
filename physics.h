/* BILLARD
 * Author: Iacopo Sprenger
 * Version: 1.1
 * Date: 25.02.2018
 * Filename: physics.h
 * */
#ifndef CALCULATIONS_H
#define CALCULATIONS_H

typedef struct {
	double x;
	double y;
	double rad;
	float r;
	float g;
	float b;
	double vx;
	double vy;
	double fx;
	double fy;
	double m;
	unsigned short grav;
}OBJECT;

typedef struct {
	OBJECT* o1;
	OBJECT* o2;
	double strength;
	unsigned short intact;
}LINK;

void calculate(void);

void init_object(OBJECT* o, double x, double y, double rad, double vx, double vy);
void color_object(OBJECT* o, float r, float g, float b);
void init_link(LINK* l, OBJECT* o1, OBJECT* o2, double strength);
void add_physics_object(OBJECT* o);
void add_physics_link(LINK* l);

double convert_x(int x);
double convert_y(int y);

double cursor_x;
double cursor_y;

unsigned short player_up;
unsigned short player_down;
unsigned short player_left;
unsigned short player_right;

#endif
