/* BILLARD
 * Author: Iacopo Sprenger
 * Version: 1.1
 * Date: 25.02.2018
 * Filename: physics.c
 * */
 
#include <stdlib.h>
#include <stdio.h>
#include "physics.h"
#include "constants.h"
#include "game.h"



OBJECT* objects[MAX_OBJECTS];
int object_count = 0;

LINK* links[MAX_LINKS];
int link_count = 0;

void collisions(void);
void wall_collisions(void);
void stick_collision(void);
void link_forces(void);
void down_gravity(void);
void general_gravity(void);
void friction(void);
void sticky_force(void);
void update(OBJECT* o);
void movement(void);

void calculate() {
	movement();
	for(int i = 0; i < object_count; i++) {
		update(objects[i]);
	}
	//down_gravity();
	//general_gravity();
	//sticky_force();
	stick_collision();
	friction();
	collisions();
	//link_forces();
	wall_collisions();
	glutTimerFunc(P_RATE, calculate, 0);
}

void update(OBJECT* o) {
	//update a
	double ax = o->fx/o->m;
	double ay = o->fy/o->m;
	//update v
	o->vx += ax*P_RATE/MS;
	o->vy += ay*P_RATE/MS;
	//update pos
	o->x += o->vx*P_RATE/MS;
	o->y += o->vy*P_RATE/MS;
	//reset force
	o->fx = 0.0;
	o->fy = 0.0;
}

void collisions(void) {
	if(object_count > 1) {
		for(int i = 0; i < object_count-1; i++) {
			for(int j = i+1; j < object_count; j++) {
				double dx = objects[i]->x - objects[j]->x;
				double dy = objects[i]->y - objects[j]->y;
				double cdist = 	sqrt(dx*dx+dy*dy);
				double mdist = (objects[i]->rad + objects[j]->rad);
				if(cdist < mdist) {
					double ux = dx / cdist;
					double uy = dy / cdist;
					double overlap = mdist-cdist;
					double fx = overlap*BOUNCE*ux;
					double fy = overlap*BOUNCE*uy;
					objects[i]->fx += fx;
					objects[i]->fy += fy;
					objects[j]->fx += -fx;
					objects[j]->fy += -fy;
				}
			}
		}
	}
}

void sticky_force(void) {
	if(object_count > 1) {
		for(int i = 0; i < object_count-1; i++) {
			for(int j = i+1; j < object_count; j++) {
				if((objects[i]->grav == 1)&&(objects[j]->grav == 1)) {
					double dx = objects[i]->x - objects[j]->x;
					double dy = objects[i]->y - objects[j]->y;
					double cdist = 	sqrt(dx*dx+dy*dy);
					if((cdist - objects[i]->rad - objects[j]->rad) < S_DIST) {
						double ux = dx / cdist;
						double uy = dy / cdist;
						double fx = S_FORCE/cdist*ux;
						double fy = S_FORCE/cdist*uy;
						objects[i]->fx += -fx;
						objects[i]->fy += -fy;
						objects[j]->fx += fx;
						objects[j]->fy += fy;
					}
				}
			}
		}
	}
}

void wall_collisions(void) {
	for(int i = 0; i < object_count; i++) {
		if((objects[i]->y+objects[i]->rad) > B_TOP){
			double overlap = B_TOP - (objects[i]->y+objects[i]->rad);
			objects[i]->fy += overlap*BOUNCE;
		}
		if((objects[i]->y-objects[i]->rad) < B_BOTTOM){
			double overlap = B_BOTTOM - (objects[i]->y-objects[i]->rad);
			objects[i]->fy += overlap*BOUNCE;
		}
		if((objects[i]->x+objects[i]->rad) > B_RIGHT){
			double overlap = B_RIGHT - (objects[i]->x+objects[i]->rad);
			objects[i]->fx += overlap*BOUNCE;
		}
		if((objects[i]->x-objects[i]->rad) < B_LEFT){
			double overlap = B_LEFT - (objects[i]->x-objects[i]->rad);
			objects[i]->fx += overlap*BOUNCE;
		}
	}
}
void stick_collision(void) {
	double dx = stick.x - white_ball.x;
	double dy = stick.y - white_ball.y;
	double cdist = 	sqrt(dx*dx+dy*dy);
	double mdist = (stick.rad + white_ball.rad);
	if(cdist < mdist) {
		double ux = dx / cdist;
		double uy = dy / cdist;
		double overlap = mdist-cdist;
		double fx = overlap*BOUNCE*ux;
		double fy = overlap*BOUNCE*uy;
		white_ball.fx += -fx;
		white_ball.fy += -fy;
	}
}

void link_forces(void) {
	for(int i = 0; i < link_count; i++) {
		if(links[i]->intact) {
			double dx = links[i]->o1->x - links[i]->o2->x;
			double dy = links[i]->o1->y - links[i]->o2->y;
			double dist = sqrt(dx*dx + dy*dy);
			double ux = dx / dist;
			double uy = dy / dist;
			links[i]->o1->fx += -links[i]->strength*ux;
			links[i]->o1->fy += -links[i]->strength*uy;
			links[i]->o2->fx += links[i]->strength*ux;
			links[i]->o2->fy += links[i]->strength*uy;
			if(dist < (links[i]->o1->rad + links[i]->o2->rad)) {
				double rvx = abs(links[i]->o1->vx-links[i]->o2->vx);
				double rvy = abs(links[i]->o1->vy-links[i]->o2->vy);
				if((rvx+rvy)>LF_THOLD) {
					links[i]->o1->fx += -LINK_FRICTION*links[i]->o1->vx;
					links[i]->o1->fy += -LINK_FRICTION*links[i]->o1->vy;
					links[i]->o2->fx += -LINK_FRICTION*links[i]->o2->vx;
					links[i]->o2->fy += -LINK_FRICTION*links[i]->o2->vy;
				}
			}
			if((dist-(links[i]->o1->rad + links[i]->o2->rad)) > LB_THOLD) {
				links[i]->intact = 0;
			}
		}
	}
}

void general_gravity(void) {
	if(object_count > 1) {
		for(int i = 0; i < object_count-1; i++) {
			for(int j = i+1; j < object_count; j++) {
				if((objects[i]->grav == 1)&&(objects[j]->grav == 1)) {
					double dx = objects[i]->x - objects[j]->x;
					double dy = objects[i]->y - objects[j]->y;
					double sdist = 	dx*dx+dy*dy;
					double cdist = 	sqrt(sdist);
					double ux = dx / cdist;
					double uy = dy / cdist;
					double fx = (G_GRAV*objects[i]->m*objects[j]->m/sdist)*ux;
					double fy = (G_GRAV*objects[i]->m*objects[j]->m/sdist)*uy;
					objects[i]->fx += -fx;
					objects[i]->fy += -fy;
					objects[j]->fx += fx;
					objects[j]->fy += fy;
				}
			}
		}
	}
}

void down_gravity(void) {
	for(int i = 0; i < object_count; i++) {
		objects[i]->fy += -GRAVITY*objects[i]->m;
	}
}

void friction(void) {
	for(int i = 0; i < object_count; i++) {
		objects[i]->fy += -FRICTION*objects[i]->rad*objects[i]->vy;
		objects[i]->fx += -FRICTION*objects[i]->rad*objects[i]->vx;
	}
}



void init_object(OBJECT* o, double x, double y, double rad, double vx, double vy) {
	o->x = x;
	o->y = y;
	o->vx = vx;
	o->vy = vy;
	o->rad = rad;
	o->fx = 0.0;
	o->fy = 0.0;
	o->r = 0.5f;
	o->g = 0.5f;
	o->b = 0.5f;
	o->m = VOL_MASS*PI*rad*rad;
	o->grav = 1;
}

void color_object(OBJECT* o, float r, float g, float b) {
	o->r = r;
	o->g = g;
	o->b = b;
}

void init_link(LINK* l, OBJECT* o1, OBJECT* o2, double strength) {
	l->o1 = o1;
	l->o2 = o2;
	l->strength = strength;
	l->intact = 1;
}

void add_physics_object(OBJECT* o) {
	objects[object_count] = o;
	object_count++;
}

void add_physics_link(LINK* l) {
	links[link_count] = l;
	link_count++;
}

void movement(void) {
	if(player_up)
		player.fy += PLAYER_FORCE*P_RATE/MS;
	if(player_down)
		player.fy -= PLAYER_FORCE*P_RATE/MS;
	if(player_left)
		player.fx -= PLAYER_FORCE*P_RATE/MS;
	if(player_right)
		player.fx += PLAYER_FORCE*P_RATE/MS;
	stick.x = cursor_x;
	stick.y = cursor_y;
}

double convert_x(int x) {
	return ((double) x / F_WIDTH)*W_WIDTH ;
}

double convert_y(int y) {
	y -= F_HEIGHT; 
	y *= -1;
	return ((double) y / F_HEIGHT)*W_HEIGHT ;
}

