#include "display.h"
#include <GL/glut.h>
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc != 2) { printf("usage: project path_to_file\n"); return 1; }
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);

	glutCreateWindow("Assignment 4");

	char* fp = argv[1];
	printf("opening %s\n", argv[1]);
	setup(fp);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	//register mouse callback
	glutMouseFunc(on_mouse_button);
	glutPassiveMotionFunc(mouse_func);
	glutMotionFunc(mouse_motion);

	glutMainLoop();

	//theoretically never called?
	free_mem();
	return 0;
}

