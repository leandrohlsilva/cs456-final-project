#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "glsl_setup.h"


int main(int argc, char** argv)
{

	graphics_state gs;
	gs.height = 512;
	gs.width = 512;

	print_howto();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(gs.width, gs.height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	int mode = 0;
	if (argc > 1) mode = atoi(argv[1]);
	gs.mode = mode;
	init(&gs);
	
	//GLuint program;
	//if (argc > 3) program = init_GLSL();

	//gs.program = program;
	
	glutDisplayFunc(display1);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keys);
	glutIdleFunc(idle1);
	glutMouseFunc(mouse_handler);
	glutMotionFunc(trackMotion);
	glutMainLoop();
	return 0;
}

