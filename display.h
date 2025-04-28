#pragma once

void setup();
void display();
void reshape(int, int);
void mouse_func(int, int);
void on_mouse_button(int, int, int, int);
void draw_rect(double, double, double, double);
void load_bmp(const char*);
void load_model(char*);
void map_to_rect(double*, double*);
void map_to_cube();
void rotate_img(double*, int);
void rotate_model();
void deform_img(double*, int);
void deform_model();
void free_mem();
void flip_image_vertical(char*, int, int, int);
void set_pixel(double, double, double);
void draw_circle(double, double, double);
void mouse_motion(int, int);
void draw_deform_interactable(double, double, double, double);
void draw_image(double, double, double, double);
void rotate(double*);
