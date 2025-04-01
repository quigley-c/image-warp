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
    //register mouse fn
}

int draw_rect(int x0, int y0, int x1, int y1) {
    glBegin(GL_LINES);
    glEnd();
}

int on_mouse_button() {}

int load_img(char* fp) {}
int load_model(char* fp) {}

int map_to_rect(int* img, int* rect) {}
int map_to_cube(int* img, int* cube) {}

int rotate_img(int* img, int deg) {
    //matrix transform
}

int rotate_model(int* model, int dimension, int deg) {
    // rotate model in dimension by deg
    // 3D matrix transform
}

int deform_img(int* img, int* mouse) {}
int deform_model(int* model, int* mouse) {}
