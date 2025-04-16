#include"display.h"
#include<stdio.h>
#include<GL/glut.h>
#include<vector>
#include<assert.h>

double mouse_x;
double mouse_y;

double** deformable;

void setup() {
	deformable = (double**) malloc(sizeof(double) * 2); // two points, making a rect
	for(int i = 0; i < 2; i++)
		deformable[i] = (double*) malloc(sizeof(double) * 2); //one point, two doubles
};

void display() {
	// main display fn
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// draw stuff
	deformable[0][0] = 0.25;
	deformable[0][1] = 0.25;
	deformable[1][0] = 0.5;
	deformable[1][1] = 0.5;

	double** d = deformable;
	draw_rect(d[0][0], d[0][1], d[1][0], d[1][1]);

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void mouse_func(int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

void on_mouse_button(int button, int state, int x, int y) {
}

void draw_rect(double x0, double y0, double x1, double y1) {
    glBegin(GL_LINES);
		glVertex2f(x0, y0);
		glVertex2f(x1, y0);

		glVertex2f(x1, y0);
		glVertex2f(x1, y1);

		glVertex2f(x1, y1);
		glVertex2f(x0, y1);

		glVertex2f(x0, y1);
		glVertex2f(x0, y0);
    glEnd();
}

void load_img(char* fp) {
	struct image;
	//GL_load(fp, &image);
}
void map_to_rect(double** img, double** rect) {
	int len = sizeof(img) / sizeof(double*);
	for(int i = 0; i < len; i++) {
		img[i] = rect[i];
	}
}

void rotate_img(double* img, int deg) {
    //matrix transform
}

void deform_img(double** img, int i_buf) {
	// change the position of the rect coord, let the mapping handle the rest
	img[i_buf][0] = mouse_x;
	img[i_buf][1] = mouse_y;
	map_to_rect(img, deformable);
}

void load_model(char* fp) {}
void map_to_cube(int* img, int* cube) {}
void deform_model(int* model, int* mouse) {}
void rotate_model(int* model, int dimension, int deg) {
    // rotate model in dimension by deg
    // 3D matrix transform
}

void free_mem() {
	free(deformable);
}

