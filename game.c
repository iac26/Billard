/* BILLARD
 * Author: Iacopo Sprenger
 * Version: 1.1
 * Date: 25.02.2018
 * Filename: game.c
 * */

#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "physics.h"
#include "constants.h"
#include "graphics.h"
#include "game.h"

void affichage(void);
void keyboard(char key);
void keyboard_2(char key);
void mouse(int button, int state, int x, int y);
void mouse_hover(int x, int y);
void reshape(int w, int h);
void redraw(void);
void draw_circle(OBJECT o);
void draw_box(void);
unsigned short player_up = 0;
unsigned short player_down = 0;
unsigned short player_left = 0;
unsigned short player_right = 0;


OBJECT obj[MAX_OBJ];
OBJECT player = {2.0, 2.0, BALL_SIZE, 0.0f, 0.0f, 1.0f, 0.0, 0.0, 0.0, 0.0, PLAYER_MASS, 1};
OBJECT white_ball;
OBJECT stick;
LINK l1;
LINK l2;
LINK l3;

int main(int argc, char **argv) {
	srand(time(NULL));
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(F_WIDTH, F_HEIGHT);
	glutCreateWindow("BILLARD"); 
	glutDisplayFunc(affichage);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_2);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouse_hover);
	glutReshapeFunc(reshape);
	glutTimerFunc(P_RATE, calculate, 0);
	glutTimerFunc(F_RATE, redraw, 0);
	for(int i = 0; i < MAX_OBJ; i++) {
		add_physics_object(&obj[i]);
	}
	init_object(&stick, 0.0, 0.0, 0.05, 0.0, 0.09);
	color_object(&stick, 0.0, 0.0, 0.0);
	init_link(&l1, &obj[4], &obj[5], 30.0);
	add_physics_link(&l1);
	init_link(&l2, &obj[5], &obj[6], 30.0);
	add_physics_link(&l2);
	init_link(&l3, &obj[6], &obj[4], 30.0);
	add_physics_link(&l3);
	//add_physics_object(&player);
	add_physics_object(&white_ball);
	glutMainLoop();
}

void affichage(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(0, W_WIDTH, 0, W_HEIGHT,-1,1); 
	draw_box();
	for(int i = 0; i < MAX_OBJ; i++) {
		draw_circle(obj[i]);
	}
	//draw_circle(player);
	draw_circle(white_ball);
	draw_circle(stick);
	
	
	glutSwapBuffers();
}



void reshape() {
	glutReshapeWindow(F_WIDTH, F_HEIGHT);
}

void keyboard(char key) {
	switch(key) {
		case 27:
			quit();
			break;
		case 'w':
			player_up = 1;
			break;
		case 's':
			player_down = 1;
			break;
		case 'd':
			player_right = 1;
			break;
		case 'a':
			player_left = 1;
			break;
		case ' ':
			init_object(&white_ball, 2.0, 3.5, 0.1, 0.0, 0.0);
			color_object(&white_ball, 1.0, 1.0, 1.0);
			init_object(&obj[0], 6.0, 3.5, 0.1, 0.0, 0.0);
			color_object(&obj[0], 1.0, 0.0, 0.0);
			init_object(&obj[1], 6.18, 3.4, 0.1, 0.0, 0.0);
			color_object(&obj[1], 0.0, 1.0, 0.0);
			init_object(&obj[2], 6.18, 3.6, 0.1, 0.0, 0.0);
			color_object(&obj[2], 1.0, 1.0, 0.0);
			init_object(&obj[3], 6.36, 3.7, 0.1, 0.0, 0.0);
			color_object(&obj[3], 1.0, 0.5, 0.0);
			init_object(&obj[4], 6.36, 3.5, 0.1, 0.0, 0.0);
			color_object(&obj[4], 0.0, 0.0, 1.0);
			init_object(&obj[5], 6.36, 3.3, 0.1, 0.0, 0.0);
			color_object(&obj[5], 1.0, 0.0, 0.0);
			init_object(&obj[6], 6.54, 3.4, 0.1, 0.0, 0.0);
			color_object(&obj[6], 0.0, 1.0, 0.0);
			init_object(&obj[7], 6.54, 3.6, 0.1, 0.0, 0.0);
			color_object(&obj[7], 1.0, 0.5, 0.0);
			init_object(&obj[8], 6.54, 3.8, 0.1, 0.0, 0.0);
			color_object(&obj[8], 1.0, 1.0, 0.0);
			init_object(&obj[9], 6.54, 3.2, 0.1, 0.0, 0.0);
			color_object(&obj[9], 0.0, 0.0, 1.0);
			break;
	}
}

void keyboard_2(char key) {
	switch(key) {
		case 'w':
			player_up = 0;
			break;
		case 's':
			player_down = 0;
			break;
		case 'd':
			player_right = 0;
			break;
		case 'a':
			player_left = 0;
			break;
		}
}

void mouse(int button, int state, int x, int y) {
	if((button == GLUT_LEFT_BUTTON)&&(state == GLUT_DOWN)) {
		
	}
	if((button == GLUT_RIGHT_BUTTON)&&(state == GLUT_DOWN)) {
		
	}
}

void mouse_hover(int x, int y) {
		cursor_x = convert_x(x);
		cursor_y = convert_y(y);
}

void draw_circle(OBJECT o) {
	glPushMatrix();
	glTranslated(o.x, o.y, 0.0);
	glColor3f(o.r, o.g, o.b);
	glBegin(GL_POLYGON);
		for(double i = 0.0; i < 2*PI; i += (2*PI/C_STEP)) {
			glVertex2d(o.rad*sin(i), o.rad*cos(i));
		}
	glEnd();
	glPopMatrix();
}

void draw_box(void) {
	glPushMatrix();
	glColor3f(0.0, 0.5, 0.0);
	glBegin(GL_POLYGON);
		glVertex2d(B_LEFT, B_TOP);
		glVertex2d(B_RIGHT, B_TOP);
		glVertex2d(B_RIGHT, B_BOTTOM);
		glVertex2d(B_LEFT, B_BOTTOM);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2d(B_LEFT, B_TOP);
		glVertex2d(B_RIGHT, B_TOP);
		glVertex2d(B_RIGHT, B_BOTTOM);
		glVertex2d(B_LEFT, B_BOTTOM);
	glEnd();
	glPopMatrix();
}

void draw_line(double x1, double y1, double x2, double y2) {
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
		glVertex2d(x1, y1);
		glVertex2d(x2, y2);
	glEnd();
}

void redraw(void) {
	glutPostRedisplay();
	glutTimerFunc(F_RATE, redraw, 0);
}

void quit(void) {
	glutDestroyWindow(1);
	exit(0);
}





