#include"display.h"
#include<stdio.h>
#include<GL/glut.h>
#include<vector>
#include<assert.h>

double mouse_x;
double mouse_y;
int WIDTH_IMG;
int HEIGHT_IMG;
int header = 54;

double** deformable;
std::vector<unsigned char> data;

auto fp_flower = "./materials/flower.bmp";
char* img_buf;

void setup() {
	deformable = (double**) malloc(sizeof(double) * 2); // two points, making a rect
	for(int i = 0; i < 2; i++)
		deformable[i] = (double*) malloc(sizeof(double) * 2); //one point, two doubles

	load_bmp(fp_flower);
	img_buf = (char*)malloc(sizeof(char)*data.size());
	for(int i = 0; i < data.size(); i++) {
		img_buf[i] = data[i];
	}
};

void display() {
	// main display fn
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// draw stuff
	deformable[0][0] = -0.99999;
	deformable[0][1] = -0.99999;
	deformable[1][0] = 1;
	deformable[1][1] = 1;

	double** d = deformable;
	draw_rect(d[0][0], d[0][1], d[1][0], d[1][1]);

	int imgx = 100;
	int imgy = 100;
	glBegin(GL_POINTS);
		glRasterPos2i(imgx, imgy);
		glDrawPixels(WIDTH_IMG, HEIGHT_IMG, GL_RGB, GL_UNSIGNED_BYTE, img_buf);
	glEnd();

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

void load_bmp(const char* fp) {
	// from sample code
    FILE* fd;
    if((fd=fopen(fp,"rb")) == NULL){
        printf("Error happens\n");
	}else{
        printf("file opened\n");
    }
    unsigned char info[header];
    fread(info,sizeof(unsigned char), header, fd); //read the header-byte header
    //extract the  heght and width of the image from the header info.

    WIDTH_IMG = *(int *)&info[18];
    HEIGHT_IMG = *(int *)&info[22];
    int size=3 * WIDTH_IMG * HEIGHT_IMG;
    printf("%d, %d\n", WIDTH_IMG, HEIGHT_IMG );

    unsigned char * pixel = (unsigned char *)malloc(sizeof(unsigned char)*size);
    fread(pixel, sizeof(unsigned char), size, fd);// read the data
    fclose(fd);

    //restore pixel from BGR to RGB.
    //
    for (int i = 0; i < size; i += 3){
        unsigned char temp=pixel[i];
        pixel[i] = pixel[i+2];
        pixel[i+2] = temp;
    }

    for(int i = 0; i < size; i++){
        data.push_back(pixel[i]);
    }

    printf("%d\n", data.size() );
    // return data;
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

