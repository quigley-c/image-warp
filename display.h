#pragma once

void setup(char*);
void display();
void reshape(int, int);
void mouse_func(int, int);
void on_mouse_button(int, int, int, int);
void draw_rect(double, double, double, double, double, double, double, double);
void load_bmp(const char*);
void load_obj(const char*);
void render_obj();
void rotate_img(double*, int);
void free_mem();
void set_pixel(double, double, double);
void draw_circle(double, double, double);
void mouse_motion(int, int);
void draw_deform_interactable();
void draw_image();
void rotate(double, double, double, double**, double**, int);
void draw_rotation_ui();
int point_in_quad(double, double, double**);
void on_menu_button(int);
