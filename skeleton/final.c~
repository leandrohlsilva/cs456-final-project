#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "glsl_setup.h"
#include "reader.h"
#include "jmesh.h"


int main(int argc, char** argv)
{

	graphics_state gs;
	gs.height = 512;
	gs.width = 512;
    FILE *offFile = fopen("../off/mushroom.off", "r");

	gs.mesh = new_jmesh(offFile, 1);

	computeNormals(gs.mesh);

	printSummary(gs.mesh);

	print_howto();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(gs.width, gs.height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	int mode = 0;
	init(&gs);
	
	GLuint program;

	program = init_GLSL("standard.vert", "environment.frag");
	gs.program[0] = program;
	program = init_GLSL("standard.vert", "fog.frag");
	gs.program[1] = program;
	program = init_GLSL("reflection.vert", "reflection.frag");
	gs.program[2] = program;
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keys);
	glutIdleFunc(idle);
	glutMouseFunc(mouse_handler);
	glutMotionFunc(trackMotion);
	glutMainLoop();
	return 0;
}

