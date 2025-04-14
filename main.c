int draw_rect();
int on_mouse_button();
int load_img(char*);
int load_model(char*);
int map_to_rect();
int map_to_cube();
int rotate_img();
int rotate_model();
int deform_img();
int deform_model();

double mouse_x;
double mouse_y;

int main() {
    //register mouse callback
}

int draw_rect(int x0, int y0, int x1, int y1) {
    glBegin(GL_LINES);
		glLine(x0, y0, x1, y0);
		glLine(x1, y0, x1, y1);
		glLine(x1, y1, x0, y1);
		glLine(x0, y1, x0, y0);
    glEnd();
}

int on_mouse_button(GLFWwindow win) {
	glfw_get_cursor_pos(win, &mouse_x, &mouse_y);
}

int load_img(char* fp) {
	struct image;
	GL_load(fp, &image);
}
int load_model(char* fp) {}

int map_to_rect(int* img, int* rect) {
	for(int i = 0; i < buf.len; i++) {
		buf[i].pos = rect[i];
	}

	return 0;
}
int map_to_cube(int* img, int* cube) {}

int rotate_img(int* img, int deg) {
    //matrix transform
}

int rotate_model(int* model, int dimension, int deg) {
    // rotate model in dimension by deg
    // 3D matrix transform
}

int deform_img(int* img, i_buf int* mouse) {
	// change the position of the rect coord, let the mapping handle the rest
	img[i_buf].x = mouse_x;
	img[i_buf].y = mouse_y;
	map_to_rect(img);
}
int deform_model(int* model, int* mouse) {}
