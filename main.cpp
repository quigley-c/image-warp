#include "display.h"
#include <GL/glut.h>

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400,400);
	glutInitWindowPosition(100,100);

	glutCreateWindow("Assignment 4");
	setup();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	//register mouse callback
	glutMouseFunc(on_mouse_button);
	glutPassiveMotionFunc(mouse_func);

	glutMainLoop();

	//theoretically never called?
	free_mem();
	return 0;
}

