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
double rot_clickable_x;
double rot_clickable_y;
double rot_click_drawable_x;
double rot_click_drawable_y;
double** image_pos;
int image_size;
double** deformable_pos;
int deformable_size;
double** rot_line_pos;
int rot_line_size;
unsigned char* img_buf;
std::vector<unsigned char> data;

int startup = 0;
int clickable_is_mouse = 0;
int clickable_radius = 10;
double deform_offset_x = 0;
double deform_offset_y = 0;
double rot_angle = 0;
char* fp;

void setup(char* fp_str) {
	fp = fp_str;
	image_size = 4;
	deformable_size = 4;
	rot_line_size = 2;
	image_pos = (double**) malloc(sizeof(double*) * image_size); // two points, making a rect
	deformable_pos = (double**) malloc(sizeof(double*) * deformable_size); // two points, making a rect
	rot_line_pos = (double**) malloc(sizeof(double*) * 2); // two points, making a rect
	for(int i = 0; i < image_size; i++)
		image_pos[i] = (double*) malloc(sizeof(double) * 2); //one point, two doubles
	for(int i = 0; i < deformable_size; i++)
		deformable_pos[i] = (double*) malloc(sizeof(double) * 2); //one point, two doubles
	for(int i = 0; i < 2; i++)
		rot_line_pos[i] = (double*) malloc(sizeof(double) * 2); //one point, two doubles

	image_pos[0][0] = -0.5;
	image_pos[0][1] = 0.95;

	image_pos[1][0] = 0.5;
	image_pos[1][1] = 0.95;

	image_pos[2][0] = 0.5;
	image_pos[2][1] = 0.25;

	image_pos[3][0] = -0.5;
	image_pos[3][1] = 0.25;

    deformable_pos[0][0] = -0.5;
	deformable_pos[0][1] = 0.05;

	deformable_pos[1][0] = 0.5;
	deformable_pos[1][1] = 0.05;

    deformable_pos[2][0] = 0.5;
	deformable_pos[2][1] = -0.75;

    deformable_pos[3][0] = -0.5;
	deformable_pos[3][1] = -0.75;

	double line_len = 0.15;
	double mid_x = deformable_pos[0][0] + (deformable_pos[1][0] - deformable_pos[0][0]) / 2;
	double mid_y = deformable_pos[0][1] + (deformable_pos[2][1] - deformable_pos[0][1]) / 2;
	rot_line_pos[0][0] = deformable_pos[1][0];
	rot_line_pos[0][1] = mid_y;
	rot_line_pos[1][0] = deformable_pos[1][0] + line_len;
	rot_line_pos[1][1] = mid_y;

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
    draw_image();

	//display user interface
    draw_deform_interactable();

	// display rotation ui
	double mid_x = deformable_pos[0][0] + (deformable_pos[1][0] - deformable_pos[0][0]) / 2;
	double mid_y = deformable_pos[0][1] + (deformable_pos[2][1] - deformable_pos[0][1]) / 2;
	double coord_mid_x = (sw/2) + (mid_x * (sw/2));
	double coord_mid_y = (sh/2) + (mid_y * (sh/2));
	rot_clickable_x = (sw/2) + (rot_line_pos[1][0] * (sw/2));
	rot_clickable_y = (sh/2) + (rot_line_pos[1][1] * (sh/2));
	if(startup == 0) { rot_click_drawable_x = rot_clickable_x; rot_click_drawable_y = rot_clickable_y; startup = 1; }

	if(clickable_is_mouse == 2) {
		rot_angle = atan2(mouse_y - coord_mid_y, mouse_x - coord_mid_x);

		//translate to origin
		rot_click_drawable_x = rot_clickable_x - coord_mid_x;
		rot_click_drawable_y = rot_clickable_y - coord_mid_y;

		//rotate
		double newx = (rot_click_drawable_x * cos(rot_angle)) - (rot_click_drawable_y * sin(rot_angle));
		double newy = (rot_click_drawable_x * sin(rot_angle)) + (rot_click_drawable_y * cos(rot_angle));

		//translate back
		rot_click_drawable_x = newx + coord_mid_x;
		rot_click_drawable_y = newy + coord_mid_y;
	}

	double** drawable = (double**) malloc(sizeof(double*) * rot_line_size);
	for(int i = 0; i < rot_line_size; i++)
		drawable[i] = (double*) malloc(sizeof(double) * 2);

	rotate(mid_x, mid_y, rot_angle, rot_line_pos, drawable, rot_line_size);
	glBegin(GL_LINES);
		glVertex2f(drawable[0][0], drawable[0][1]);
		glVertex2f(drawable[1][0], drawable[1][1]);
	glEnd();
	draw_circle(rot_click_drawable_x, rot_click_drawable_y, clickable_radius);

	free(drawable);

	glutSwapBuffers();
	glutPostRedisplay();
}

void rotate(double x, double y, double angle, double** points, double** result_arr, int size) {
	//rotate array points around the center point x,y
	//angle in radians
	double coord_mid_x = (sw/2) + (x * (sw/2));
	double coord_mid_y = (sh/2) + (y * (sh/2));
	for(int i = 0; i < size; i++) {
		//convert from gl coords, translate to origin
		result_arr[i][0] = ((sw/2) + (points[i][0] * (sw/2))) - coord_mid_x;
		result_arr[i][1] = ((sh/2) + (points[i][1] * (sh/2))) - coord_mid_y;

		//rotate
		double newx = (result_arr[i][0] * cos(angle)) - (result_arr[i][1] * sin(angle));
		double newy = (result_arr[i][0] * sin(angle)) + (result_arr[i][1] * cos(angle));

		//translate back
		result_arr[i][0] = newx + coord_mid_x;
		result_arr[i][1] = newy + coord_mid_y;

		//convert back to gl coordinates
		result_arr[i][0] = (result_arr[i][0] / sw) * 2 - 1;
		result_arr[i][1] = (result_arr[i][1] / sh) * 2 - 1;
	}
}

void draw_image() {
	double mid_x = image_pos[0][0] + ((image_pos[1][0] - image_pos[0][0])/2);
	double mid_y = (image_pos[0][1] + image_pos[2][1]) / 2;

	double deformed_x = mid_x + deform_offset_x;
	double deformed_y = mid_y + deform_offset_y;

	double** drawable = (double**) malloc(sizeof(double*) * image_size);
	for(int i = 0; i < image_size; i++)
		drawable[i] = (double*) malloc(sizeof(double) * 2);
    rotate(mid_x, mid_y, rot_angle, image_pos, drawable, image_size);

	double drawable_mid_right_x = (drawable[2][0] + drawable[1][0])/2;
	double drawable_mid_right_y = (drawable[2][1] + drawable[1][1])/2;

	double drawable_mid_left_x = (drawable[0][0] + drawable[3][0])/2;
	double drawable_mid_left_y = (drawable[0][1] + drawable[3][1])/2;

	double drawable_mid_bot_x = (drawable[3][0] + drawable[2][0])/2;
	double drawable_mid_bot_y = (drawable[3][1] + drawable[2][1])/2;

	double drawable_mid_top_x = (drawable[0][0] + drawable[1][0])/2;
	double drawable_mid_top_y = (drawable[0][1] + drawable[1][1])/2;

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		// Bottom-left triangle
		glTexCoord2f(0.5f, 0.5f); glVertex2f(deformed_x, deformed_y);
		glTexCoord2f(0.0f, 0.5f); glVertex2f(drawable_mid_left_x, drawable_mid_left_y);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(drawable[3][0], drawable[3][1]);
		glTexCoord2f(0.5f, 0.0f); glVertex2f(drawable_mid_bot_x, drawable_mid_bot_y);

		// Bottom-right triangle
		glTexCoord2f(0.5f, 0.5f); glVertex2f(deformed_x, deformed_y);
		glTexCoord2f(0.5f, 0.0f); glVertex2f(drawable_mid_bot_x, drawable_mid_bot_y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(drawable[2][0], drawable[2][1]);
		glTexCoord2f(1.0f, 0.5f); glVertex2f(drawable_mid_right_x, drawable_mid_right_y);

		// Top-right triangle
		glTexCoord2f(0.5f, 0.5f); glVertex2f(deformed_x, deformed_y);
		glTexCoord2f(1.0f, 0.5f); glVertex2f(drawable_mid_right_x, drawable_mid_right_y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(drawable[1][0], drawable[1][1]);
		glTexCoord2f(0.5f, 1.0f); glVertex2f(drawable_mid_top_x, drawable_mid_top_y);

		// Top-left triangle
		glTexCoord2f(0.5f, 0.5f); glVertex2f(deformed_x, deformed_y);
		glTexCoord2f(0.5f, 1.0f); glVertex2f(drawable_mid_top_x, drawable_mid_top_y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(drawable[0][0], drawable[0][1]);
		glTexCoord2f(0.0f, 0.5f); glVertex2f(drawable_mid_left_x, drawable_mid_left_y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void draw_deform_interactable() {
	double dmid_x = deformable_pos[0][0] + (deformable_pos[1][0] - deformable_pos[0][0]) / 2;
	double dmid_y = deformable_pos[0][1] + (deformable_pos[2][1] - deformable_pos[0][1]) / 2;

	double ddeformed_x = dmid_x;
	double ddeformed_y = dmid_y;

	ddeformed_x = (clickable_x / sw) * 2 - 1;
	ddeformed_y = (clickable_y / sh) * 2 - 1;
	deform_offset_x = -(dmid_x - ddeformed_x);
	deform_offset_y = -(dmid_y - ddeformed_y);

	double** deform_drawable = (double**) malloc(sizeof(double*) * deformable_size);
	for(int i = 0; i < deformable_size; i++)
		deform_drawable[i] = (double*) malloc(sizeof(double) * 2);

    rotate(dmid_x, dmid_y, rot_angle, deformable_pos, deform_drawable, deformable_size);
	draw_rect(deform_drawable[0][0], deform_drawable[0][1],
              deform_drawable[1][0], deform_drawable[1][1],
              deform_drawable[2][0], deform_drawable[2][1],
              deform_drawable[3][0], deform_drawable[3][1]
			  );

	double left_edge = (sw/2) + ((sw/2) * deform_drawable[3][0]);
	double right_edge = (sw/2) + ((sw/2) * deform_drawable[2][0]);
	double top_edge = (sh/2) + ((sh/2) * deform_drawable[0][1]);
	double bottom_edge = (sh/2) + ((sh/2) * deform_drawable[3][1]);
	double mouse_norm_x = (mouse_x - (sw/2)) / (sw/2);
	double mouse_norm_y = (mouse_y - (sh/2)) / (sh/2);
	if(clickable_is_mouse == 1 &&
			point_in_quad(mouse_norm_x, mouse_norm_y, deform_drawable) == 1
	) {
		clickable_x = mouse_x;
		clickable_y = mouse_y;
	}
	if (clickable_is_mouse == 0) {
		clickable_x = (sw / 2) + ((sw / 2) * dmid_x);
		clickable_y = (sh / 2) + ((sh / 2) * dmid_y);
	}

	double drawable_mid_right_x = (deform_drawable[2][0] + deform_drawable[1][0])/2;
	double drawable_mid_right_y = (deform_drawable[2][1] + deform_drawable[1][1])/2;

	double drawable_mid_left_x = (deform_drawable[0][0] + deform_drawable[3][0])/2;
	double drawable_mid_left_y = (deform_drawable[0][1] + deform_drawable[3][1])/2;

	double drawable_mid_top_x = (deform_drawable[3][0] + deform_drawable[2][0])/2;
	double drawable_mid_top_y = (deform_drawable[3][1] + deform_drawable[2][1])/2;

	double drawable_mid_bot_x = (deform_drawable[0][0] + deform_drawable[1][0])/2;
	double drawable_mid_bot_y = (deform_drawable[0][1] + deform_drawable[1][1])/2;

	glBegin(GL_LINES);
		glColor3f(0,0,1);
		glVertex2f(drawable_mid_left_x, drawable_mid_left_y); glVertex2f(dmid_x, dmid_y);
		glVertex2f(drawable_mid_top_x, drawable_mid_top_y); glVertex2f(dmid_x, dmid_y);
		glVertex2f(drawable_mid_right_x, drawable_mid_right_y); glVertex2f(dmid_x, dmid_y);
		glVertex2f(drawable_mid_bot_x, drawable_mid_bot_y); glVertex2f(dmid_x, dmid_y);

		glColor3f(1,0,0);
		glVertex2f(drawable_mid_left_x, drawable_mid_left_y); glVertex2f(ddeformed_x, ddeformed_y);
		glVertex2f(drawable_mid_top_x, drawable_mid_top_y); glVertex2f(ddeformed_x, ddeformed_y);
		glVertex2f(drawable_mid_right_x, drawable_mid_right_y); glVertex2f(ddeformed_x, ddeformed_y);
		glVertex2f(drawable_mid_bot_x, drawable_mid_bot_y); glVertex2f(ddeformed_x, ddeformed_y);
	glEnd();

	draw_circle(clickable_x, clickable_y, clickable_radius);
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
		} else if(x < rot_click_drawable_x + clickable_radius && x > rot_click_drawable_x - clickable_radius) {
			if(mouse_y < rot_click_drawable_y + clickable_radius && mouse_y > rot_click_drawable_y - clickable_radius) {
				clickable_is_mouse = 2;
			}
		}
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) { clickable_is_mouse = 0; }
}

void draw_rect(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3) {
    glBegin(GL_LINES);
		glVertex2f(x0, y0);
		glVertex2f(x1, y1);

		glVertex2f(x1, y1);
		glVertex2f(x2, y2);

		glVertex2f(x2, y2);
		glVertex2f(x3, y3);

		glVertex2f(x3, y3);
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

int point_in_quad(double px, double py, double** quad) {
	for (int i = 0; i < 4; ++i) {
		int j = (i + 1) % 4;
		double x0 = quad[i][0], y0 = quad[i][1];
		double x1 = quad[j][0], y1 = quad[j][1];
		double dx = x1 - x0, dy = y1 - y0;
		double cross = (px - x0) * dy - (py - y0) * dx;
		if (cross < 0) return 0; // Outside the edge
	}
	return 1; // Inside all edges
}
