#include"display.h"
#include<stdio.h>
#include<GL/glut.h>
#include<vector>
#include<assert.h>
#include<cstring>
#include<cmath>

GLuint tex;
int WIDTH_IMG;
int HEIGHT_IMG;
int sw;
int sh;
double mouse_x;
double mouse_y;
double clickable_x;
double clickable_y;
double** image_pos;
double** deformable_pos;
unsigned char* img_buf;
std::vector<unsigned char> data;

int clickable_is_mouse = 0;
int clickable_radius = 10;
double deform_offset_x = 0;
double deform_offset_y = 0;
auto fp = "./materials/flower.bmp";

void setup() {
	image_pos = (double**) malloc(sizeof(double*) * 2); // two points, making a rect
	deformable_pos = (double**) malloc(sizeof(double*) * 2); // two points, making a rect
	for(int i = 0; i < 2; i++)
		image_pos[i] = (double*) malloc(sizeof(double) * 2); //one point, two doubles
	for(int i = 0; i < 2; i++)
		deformable_pos[i] = (double*) malloc(sizeof(double) * 2); //one point, two doubles

	load_bmp(fp);
	img_buf = (unsigned char*)malloc(sizeof(unsigned char)*data.size());
    img_buf = data.data();

	//texturing
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH_IMG, HEIGHT_IMG, 0, GL_RGB, GL_UNSIGNED_BYTE, img_buf);
	glEnable(GL_TEXTURE_2D);
};

void display() {
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// display image
	image_pos[0][0] = -0.5;
	image_pos[0][1] = 0.25;
	image_pos[1][0] = 0.5;
	image_pos[1][1] = 0.95;

	double mid_x = image_pos[0][0] + ((image_pos[1][0] - image_pos[0][0])/2);
	double mid_y = image_pos[0][1] + ((image_pos[1][1] - image_pos[0][1])/2);

	double deformed_x = mid_x + deform_offset_x;
	double deformed_y = mid_y + deform_offset_y;

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		//bottom-left, bottom-right, top-right, top-left
		glTexCoord2f(0.0f, 0.0f); glVertex2f(image_pos[0][0], image_pos[0][1]);
		glTexCoord2f(0.5f, 0.0f); glVertex2f(mid_x, image_pos[0][1]);
		glTexCoord2f(0.5f, 0.5f); glVertex2f(deformed_x, deformed_y);
		glTexCoord2f(0.0f, 0.5f); glVertex2f(image_pos[0][0], mid_y);

		glTexCoord2f(0.5f, 0.5f); glVertex2f(deformed_x, deformed_y);
		glTexCoord2f(0.5f, 0.0f); glVertex2f(mid_x, image_pos[0][1]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(image_pos[1][0], image_pos[0][1]);
		glTexCoord2f(1.0f, 0.5f); glVertex2f(image_pos[1][0], mid_y);

		glTexCoord2f(1.0f, 1.0f); glVertex2f(image_pos[1][0], image_pos[1][1]);
		glTexCoord2f(0.5f, 1.0f); glVertex2f(mid_x, image_pos[1][1]);
		glTexCoord2f(0.5f, 0.5f); glVertex2f(deformed_x, deformed_y);
		glTexCoord2f(1.0f, 0.5f); glVertex2f(image_pos[1][0], mid_y);

		glTexCoord2f(0.0f, 1.0f); glVertex2f(image_pos[0][0], image_pos[1][1]);
		glTexCoord2f(0.0f, 0.5f); glVertex2f(image_pos[0][0], mid_y);
		glTexCoord2f(0.5f, 0.5f); glVertex2f(deformed_x, deformed_y);
		glTexCoord2f(0.5f, 1.0f); glVertex2f(mid_x, image_pos[1][1]);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//display user interface
    deformable_pos[0][0] = -0.5;
	deformable_pos[0][1] = -0.75;
	deformable_pos[1][0] = 0.5;
	deformable_pos[1][1] = 0.05;

	double dmid_x = deformable_pos[0][0] + ((deformable_pos[1][0] - deformable_pos[0][0])/2);
	double dmid_y = deformable_pos[0][1] + ((deformable_pos[1][1] - deformable_pos[0][1])/2);

	double ddeformed_x = dmid_x;
	double ddeformed_y = dmid_y;

	double left_edge = (sw/2) + ((sw/2) * deformable_pos[0][0]);
	double right_edge = (sw/2) + ((sw/2) * deformable_pos[1][0]);
	double top_edge = (sh/2) + ((sh/2) * deformable_pos[1][1]);
	double bottom_edge = (sh/2) + ((sh/2) * deformable_pos[0][1]);
	if(clickable_is_mouse == 1 &&
			mouse_x > left_edge && mouse_x < right_edge &&
			mouse_y > bottom_edge && mouse_y < top_edge
	) {
		clickable_x = mouse_x;
		clickable_y = mouse_y;
	}
	if (clickable_is_mouse == 0) {
		clickable_x = (sw / 2) + ((sw / 2) * dmid_x);
		clickable_y = (sh / 2) + ((sh / 2) * dmid_y);
	}

	ddeformed_x = (clickable_x / sw) * 2 - 1;
	ddeformed_y = (clickable_y / sh) * 2 - 1;
	deform_offset_x = -(dmid_x - ddeformed_x);
	deform_offset_y = -(dmid_y - ddeformed_y);

	draw_rect(deformable_pos[0][0], deformable_pos[0][1],
            deformable_pos[1][0], deformable_pos[1][1]);
	glBegin(GL_LINES);
		glColor3f(0,0,1);
		glVertex2f(deformable_pos[0][0], dmid_y); glVertex2f(dmid_x, dmid_y);
		glVertex2f(dmid_x, deformable_pos[1][1]); glVertex2f(dmid_x, dmid_y);
		glVertex2f(deformable_pos[1][0], dmid_y); glVertex2f(dmid_x, dmid_y);
		glVertex2f(dmid_x, deformable_pos[0][1]); glVertex2f(dmid_x, dmid_y);

		glColor3f(1,0,0);
		glVertex2f(deformable_pos[0][0], dmid_y); glVertex2f(ddeformed_x, ddeformed_y);
		glVertex2f(dmid_x, deformable_pos[1][1]); glVertex2f(ddeformed_x, ddeformed_y);
		glVertex2f(deformable_pos[1][0], dmid_y); glVertex2f(ddeformed_x, ddeformed_y);
		glVertex2f(dmid_x, deformable_pos[0][1]); glVertex2f(ddeformed_x, ddeformed_y);
	glEnd();

	draw_circle(clickable_x, clickable_y, clickable_radius);

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h) {
	printf("resizing window: %d, %d\n", w, h);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	sw = w;
	sh = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void mouse_func(int x, int y) {
	mouse_x = x;
	mouse_y = sh - y;
}

void mouse_motion(int x, int y) {
	mouse_x = x;
	mouse_y = sh - y;
}

void on_mouse_button(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if(x < clickable_x + clickable_radius && x > clickable_x - clickable_radius) {
			if(mouse_y < clickable_y + clickable_radius && mouse_y > clickable_y - clickable_radius) {
				clickable_is_mouse = 1;
			}
		}
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) { clickable_is_mouse = 0; }
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
    int header = 54;
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

void map_img(double** img, int i_buf) {
	// change the position of the rect coord, let the mapping handle the rest
	img[i_buf][0] = mouse_x;
	img[i_buf][1] = mouse_y;
	map_to_rect(img, image_pos);
}

void load_model(char* fp) {}
void map_to_cube(int* img, int* cube) {}
void deform_model(int* model, int* mouse) {}
void rotate_model(int* model, int dimension, int deg) {
    // rotate model in dimension by deg
    // 3D matrix transform
}

void free_mem() {
	free(image_pos);
}

void draw_circle(double x, double y, double radius) {
	for(int i = 0; i < 360; i++) {
		double newx = radius*cos(i) + x;
		double newy = radius*sin(i) + y;
		set_pixel(newx, newy, 255);
	}
}

void set_pixel(double x, double y, double d) {
    double glx = ((double)x / sw) * 2 - 1;
    double gly = ((double)y / sh) * 2 - 1;

    double i = 1 - (d/16);
    glBegin(GL_POINTS);
        glColor4d(1,1,1,i);
        glVertex2d(glx,gly);
    glEnd();
}
