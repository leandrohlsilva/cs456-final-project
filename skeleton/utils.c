#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>
#include "utils.h"
#include "sgi_texture.h"
#include "glsl_setup.h"


static graphics_state * current_gs;

static double totalTime = 0;
static struct timeval start;
static frames = 0;


void print_howto(void){
  printf("A:   Move X axis\n");
  printf("D:   Move X avis\n");
  printf("W:   Move Y axis\n");
  printf("S:   Move Y axis\n");
  printf("E:   Move Z axis\n");
  printf("R:   Move Z axis\n\n");
  printf("\nI:  Change the looking center in Y axis\n");   
  printf("K:  Change the looking center in Y axis\n");
  printf("J:  Change the looking center in X axis\n");  
  printf("L:  Change the looking center in X axis\n");

}

void 
set_gs(graphics_state * gs){
  current_gs = gs;
}

void setTexture(int tex, const char* texName) {

	unsigned *image;
	int width, height, components;
	
	glBindTexture(GL_TEXTURE_2D, current_gs->texNames[tex]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	image = read_texture(texName, &width, &height, &components);
	glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	free(image);
	
}

void 
init(graphics_state * gs){
	
	gettimeofday(&start, NULL);
	
	current_gs = gs;
	
	gs->blurMode = 0;
	
	gs->totalTrees = 5000;
	gs->scaleTrees = (GLfloat *) malloc(sizeof(GLfloat)*gs->totalTrees*3);
	gs->translateTrees = (GLfloat *) malloc(sizeof(GLfloat)*gs->totalTrees*3);
	
	gs->totalMeshes = 5000;
	gs->scaleMeshes = (GLfloat *) malloc(sizeof(GLfloat)*gs->totalMeshes*3);
	gs->translateMeshes = (GLfloat *) malloc(sizeof(GLfloat)*gs->totalMeshes*3);
	
	gs->totalSnowFlakes = 10000;
	
	gs->snowFlakes = (struct particle *) malloc(sizeof(struct particle)*gs->totalSnowFlakes);
	
	gs->windspeed=0.005f;
	gs->winddir=45;
	
	srand ( time(NULL) );
	int i;
	for (i=0;i<gs->totalSnowFlakes;i++)
	{
		gs->snowFlakes[i].xd=-(rand()/(float)RAND_MAX-0.5f)/200.0f;
		gs->snowFlakes[i].zd=-(rand()/(float)RAND_MAX-0.5f)/200.0f;
		gs->snowFlakes[i].yd=-rand()/(float)RAND_MAX/100.0f;
		gs->snowFlakes[i].x= gs->camx + ((rand()%2==0)?1:-1)*((rand()/(float)RAND_MAX)*50);
		gs->snowFlakes[i].y= (((rand()/(float)RAND_MAX)*20));
		gs->snowFlakes[i].z= gs->camz + ((rand()%2==0)?1:-1)*((rand()/(float)RAND_MAX)*50);
		gs->snowFlakes[i].b=rand()/(float)RAND_MAX;
		gs->snowFlakes[i].g=gs->snowFlakes[i].b;
		gs->snowFlakes[i].r=gs->snowFlakes[i].b;
	}
	glShadeModel(GL_SMOOTH);						
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				
	glClearDepth(1.0f);								
    glClearAccum(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthFunc(GL_LEQUAL);							
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//define trees sizes and position

	for (i = 0; i < gs->totalTrees*3; i++) {
		//gs->scaleTrees[i] = (rand() % ((i%3==1)?10:5) + 5);
		gs->scaleTrees[i] = 5;
		if (i % 3 == 0) gs->translateTrees[i] = gs->camx + ((rand()%2==0)?1:-1)*((rand()/(float)RAND_MAX)*500);
		if (i % 3 == 1) gs->translateTrees[i] = 1.0;
		else gs->translateTrees[i] = gs->camz + ((rand()%2==0)?1:-1)*((rand()/(float)RAND_MAX)*500);

		
	}
	
	for (i = 0; i < gs->totalMeshes*3; i++) {
		gs->scaleMeshes[i] = 2.8;
		if (i % 3 == 0) gs->translateMeshes[i] = gs->camx + ((rand()%2==0)?1:-1)*((rand()/(float)RAND_MAX)*500);
		if (i % 3 == 1) gs->translateMeshes[i] = -2.0;
		else gs->translateMeshes[i] = gs->camz + ((rand()%2==0)?1:-1)*((rand()/(float)RAND_MAX)*500);

	}
	
	

	GLfloat mat_specular[] = {0.508273,0.508273, 0.508273, 1.0};
	GLfloat mat_diffuse[] = {0.50754, 0.50754, 0.50754, 1.0};
	GLfloat mat_ambient[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat mat_shininess[] = {81.2};
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0}; 
	GLfloat density = 0.01;


	glLoadIdentity();
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_FOG);
	glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2
	glFogfv (GL_FOG_COLOR, fogColor); //set the fog color to
	glFogf (GL_FOG_DENSITY, density); //set the density to the
	glHint (GL_FOG_HINT, GL_NICEST);

	GLfloat light_pos[] = {100, 400, 0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	
	current_gs->lookx = current_gs->looky = current_gs->lookz = 0.0;

	//setting the max and min height
	current_gs->camx = 5;
	current_gs->camy = 0;
	current_gs->camz = 1;

	GLfloat s = 1;

	current_gs->scalex = s;
	current_gs->scaley = s;
	current_gs->scalez = s;

	current_gs->rotation = 0;

	unsigned *image;
	int width, height, components;
    
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	current_gs->texNames = (GLuint *) malloc(sizeof(GLuint)*21);
	glGenTextures(21, current_gs->texNames);
	setTexture(0, "../imagefiles/ice.rgb");
	setTexture(1, "../imagefiles/wood2.rgb");
	setTexture(2, "../imagefiles/birch.rgb");
	setTexture(3, "../imagefiles/cotton.rgb");
	setTexture(4, "../imagefiles/checkers.rgb");
	setTexture(5, "../imagefiles/people.rgb");
	setTexture(6, "../imagefiles/eoe2_alphaEqu1_all.rgb");
	setTexture(7, "../imagefiles/eoe3_alphaEqu1_land_alpha0_water.rgb");
	
	//--------------- Neyland textures --------------------------//
	
	setTexture(8, "../imagefiles/ground_frozen.rgb");
	setTexture(9, "../imagefiles/plate5.rgba");
	setTexture(10, "../imagefiles/landscape1.rgb");
	setTexture(11, "../imagefiles/landscape1.rgb");
	setTexture(12, "../imagefiles/landscape1.rgb");
	setTexture(13, "../imagefiles/landscape1.rgb");
	
	setTexture(20, "../imagefiles/tree.rgb");
	

}


void snowFlakes() {

	graphics_state *gs = current_gs;
	glTranslatef(0.0f,0.0f,-2.0f);
	
	GLfloat param = param;
		
		glBegin(GL_LINES);
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(sin(gs->winddir*param)*gs->windspeed*2.0f,cos(gs->winddir*param)*gs->windspeed*2.0f,0.0f);
		glEnd();
		glPointSize(2.0f);
		glBegin(GL_POINTS);
		int i;
		for (i=0;i<gs->totalSnowFlakes;i++)
		{
			gs->snowFlakes[i].x+=cos(gs->winddir*param)*gs->windspeed;
			gs->snowFlakes[i].y+=gs->snowFlakes[i].yd;
			gs->snowFlakes[i].z+=sin(gs->winddir*param)*gs->windspeed;
			gs->snowFlakes[i].yd-=rand()/(float)RAND_MAX/10000.0f;
			if ((gs->snowFlakes[i].y<=-2.0f) || (abs(gs->snowFlakes[i].x - gs->camx) > 100) || (abs(gs->snowFlakes[i].z - gs->camz) > 100))
			{
				gs->snowFlakes[i].xd=-(rand()/(float)RAND_MAX-0.5f)/200.0f;
				gs->snowFlakes[i].zd=-(rand()/(float)RAND_MAX-0.5f)/200.0f;
				gs->snowFlakes[i].yd=-rand()/(float)RAND_MAX/100.0f;
				gs->snowFlakes[i].x= gs->camx + ((rand()%2==0)?1:-1)*((rand()/(float)RAND_MAX)*50);
				gs->snowFlakes[i].y= 5 + (((rand()/(float)RAND_MAX)*5));
				gs->snowFlakes[i].z= gs->camz + ((rand()%2==0)?1:-1)*((rand()/(float)RAND_MAX)*50);
				gs->snowFlakes[i].b=rand()/(float)RAND_MAX;
				gs->snowFlakes[i].g=gs->snowFlakes[i].b;
				gs->snowFlakes[i].r=gs->snowFlakes[i].b;
			}
			glColor3f(gs->snowFlakes[i].r,gs->snowFlakes[i].g,gs->snowFlakes[i].b);
			glVertex3f(gs->snowFlakes[i].x,gs->snowFlakes[i].y,gs->snowFlakes[i].z);
			//printf("(%f,%f,%f)\n",gs->snowFlakes[i].x,gs->snowFlakes[i].y,gs->snowFlakes[i].z);
		}
		glEnd();

}

void tree() {
	glEnable(GL_TEXTURE_2D);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, current_gs->texNames[20]);
	glBegin(GL_QUADS);
	
	GLfloat f1 = 1.0, f2 = 1.0, f3 = 1.0;
	
	//south wall
	glTexCoord2f(0.0, 0.0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(-f1-0.1, -f2, 0.0);
	glTexCoord2f(0.0, 1.0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(-f1-0.1, f2, 0.0);
	glTexCoord2f(1.0, 1.0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(f1-0.1, f2, 0.0);
	glTexCoord2f(1.0, 0.0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(f1-0.1, -f2, 0.0);
	
	//east wall
	glTexCoord2f(0.0, 0.0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(0.0, -f2, -f3);
	glTexCoord2f(0.0, 1.0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(0.0, f2, -f3);
	glTexCoord2f(1.0, 1.0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(0.0, f2, f3);
	glTexCoord2f(1.0, 0.0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(0.0, -f2, f3);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void environment() {
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, current_gs->texNames[8]);
	glBegin(GL_QUADS);

	
	GLfloat f1 = 1.0, f2 = 1.0, f3 = 1.0;
	//floor
	glTexCoord2f(0.0, 0.0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(-f1, -f2, -f3);
	glTexCoord2f(0.0, 1.0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(-f1, -f2, f3);
	glTexCoord2f(1.0, 1.0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(f1, -f2, f3);
	glTexCoord2f(1.0, 0.0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(f1, -f2, -f3);

	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, current_gs->texNames[9]);
	glBegin(GL_QUADS);

	//ceiling
	glTexCoord2f(0.0, 0.0); glNormal3f(0.0, 1.0, 0.0); glVertex3f(-f1, f2, -f3);
	glTexCoord2f(0.0, 1.0); glNormal3f(0.0, 1.0, 0.0); glVertex3f(-f1, f2, f3);
	glTexCoord2f(1.0, 1.0); glNormal3f(0.0, 1.0, 0.0); glVertex3f(f1, f2, f3);
	glTexCoord2f(1.0, 0.0); glNormal3f(0.0, 1.0, 0.0); glVertex3f(f1, f2, -f3);
	
	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, current_gs->texNames[10]);
	glBegin(GL_QUADS);
	
	//south wall
	glTexCoord2f(0.0, 0.0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(-f1, -f2, -f3);
	glTexCoord2f(0.0, 1.0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(-f1, f2, -f3);
	glTexCoord2f(1.0, 1.0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(f1, f2, -f3);
	glTexCoord2f(1.0, 0.0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(f1, -f2, -f3);

	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, current_gs->texNames[11]);
	glBegin(GL_QUADS);
	
	//east wall
	glTexCoord2f(1.0, 0.0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(-f1, -f2, -f3);
	glTexCoord2f(0.0, 0.0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(-f1, -f2, f3);
	glTexCoord2f(0.0, 1.0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(-f1, f2, f3);
	glTexCoord2f(1.0, 1.0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(-f1, f2, -f3);

	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, current_gs->texNames[12]);
	glBegin(GL_QUADS);
	
	//west wall
	glTexCoord2f(1.0, 0.0); glNormal3f(1.0, 0, 0.0); glVertex3f(f1, -f2, -f3);
	glTexCoord2f(0.0, 0.0); glNormal3f(1.0, 0, 0.0); glVertex3f(f1, -f2, f3);
	glTexCoord2f(0.0, 1.0); glNormal3f(1.0, 0, 0.0); glVertex3f(f1, f2, f3);
	glTexCoord2f(1.0, 1.0); glNormal3f(1.0, 0, 0.0); glVertex3f(f1, f2, -f3);

	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, current_gs->texNames[13]);
	glBegin(GL_QUADS);
	
	//north wall
	glTexCoord2f(0.0, 0.0); glNormal3f(0.0, 0.0, 1.0); glVertex3f(-f1, -f2, f3);
	glTexCoord2f(0.0, 1.0); glNormal3f(0.0, 0.0, 1.0); glVertex3f(-f1, f2, f3);
	glTexCoord2f(1.0, 1.0); glNormal3f(0.0, 0.0, 1.0); glVertex3f(f1, f2, f3);
	glTexCoord2f(1.0, 0.0); glNormal3f(0.0, 0.0, 1.0); glVertex3f(f1, -f2, f3);


	glEnd();
	//glFlush();
	glDisable(GL_TEXTURE_2D);
	
}

void puddle() {

	glEnable(GL_TEXTURE_2D);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		GLfloat f1 = 1.0, f2 = 1.0, f3 = 1.0;
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, current_gs->texNames[9]);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_POLYGON);
	
	glTexCoord2f(0.0, 0.0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(-f1, -f2, -f3);
	glTexCoord2f(0.0, 1.0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(-f1, -f2, f3);
	glTexCoord2f(1.0, 1.0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(f1, -f2, f3);
	glTexCoord2f(1.0, 0.0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(f1, -f2, -f3);
	glTexCoord2f(0.5, 0.5); glNormal3f(0.0, -1.0, 0.0); glVertex3f(f1/.5f, -f2, -f3/.5f);
	glTexCoord2f(0.75, 0.75); glNormal3f(0.0, -1.0, 0.0); glVertex3f(f1/.5f, -f2, f3/.5f);
	glTexCoord2f(0.20, 0.20); glNormal3f(0.0, -1.0, 0.0); glVertex3f(-f1/.5f, -f2, f3/.5f);
		
	glEnd();
	//glFlush();
	glDisable(GL_TEXTURE_2D);

}

void offObj() {
	int i,j;
	jmesh * jm;
	jm = current_gs->mesh;
	
	glEnable ( GL_TEXTURE_2D );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture( GL_TEXTURE_2D, current_gs->texNames[0] );


	glBegin( GL_TRIANGLES );
	for (i = 0; i < jm->ntri;i++) {
		GLfloat r = fabsf(jm->normals[3*jm->triangles[i*3]]);
		GLfloat g = fabsf(jm->normals[3*jm->triangles[i*3]+1]);
		GLfloat b = fabsf(jm->normals[3*jm->triangles[i*3]+2]);
		glColor3f(r, g, b);

		//tex coordinates
		for(j = 0; j < 4; j++) {
			//glTexCoord2fv(current_gs->texCoords + j*2);
		}

		for(j = 0; j < 3; j++){
			//Pass in the vertex normal
			//Pass the vertex
			glNormal3fv((GLfloat *)&jm->normals[jm->triangles[i*3+j]*3]);
			glVertex3fv((GLfloat *)&jm->vertices[jm->triangles[i*3+j]*3]);


		}

	}
	glEnd();

}


void 
display(void){

	

	graphics_state *gs = current_gs;

	GLfloat camx, camy, camz, lookx, looky, lookz;
	camx = gs->camx;
	camy = gs->camy;
	camz = gs->camz;
	lookx = gs->lookx;
	looky = gs->looky;
	lookz = gs->lookz;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) current_gs->width/(GLfloat) current_gs->height, 1.0, 10000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//switch_shaders(gs->program, gs->fragShaders[1], gs->fragSources[1]); //environment shader
	
	
	
	if (gs->blurMode) {
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);
		//glAccum(GL_RETURN, 0.95f);
	    //glClear(GL_ACCUM_BUFFER_BIT);
		//glAccum(GL_RETURN, 1.0);
	} else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//glPushMatrix();
	//glLoadIdentity();
	gluLookAt(camx, camy, camz, 
		lookx, looky, lookz,
		0.0, 1.0, 0.0 );
	
	switch_shaders(gs->program[0]); //environment shader
	//init_GLSL("standard.vert", "fog.frag");
	
	//init_GLSL("standard.vert", "fog.frag");
	glPushMatrix();
		glScalef(5000.0, 500.0, 5000.0);
		glTranslatef(0.0,0.985,0.0);
		//glRotatef(current_gs->rotation,0.0,1.0,0.0);
		environment(); //environment	
	glPopMatrix();
	
	//init_GLSL("standard.vert", "environment.frag");
	glPushMatrix();
		//glScalef(50.0, 50.0, 50.0);
		snowFlakes();
	glPopMatrix();
	
	//switch_shaders(gs->program, gs->fragShaders[1]); //fog shader
	
/*			
	glPushMatrix();
		//glScalef(current_gs->scalex, current_gs->scaley, current_gs->scalez);
		//glRotatef(current_gs->rotation, current_gs->rotation, current_gs->rotation,0.0);
		//offObj();

	glPopMatrix();
	

	glPushMatrix();
		glTranslatef(-1.0,2.0,3.0);
		glRotatef(current_gs->rotation,0.0,1.0,0.0);
		cube(abs(current_gs->mode - 2)); //neyland box
	glPopMatrix();
	*/
	
	//init_GLSL("standard.vert", "fog.frag");
	switch_shaders(gs->program[1]); //fog shader
	
	glPushMatrix();
		int i;	
		for (i = 0; i < gs->totalTrees; i+=3) {
			glPushMatrix();
				glTranslatef(	gs->translateTrees[i], 
								gs->translateTrees[i+1], 
								gs->translateTrees[i+2]);
				
				glScalef(	gs->scaleTrees[i],
							gs->scaleTrees[i+1],
							gs->scaleTrees[i+2]);
				tree();
			glPopMatrix();	
		}	
		//glTranslatef(-10.0,0.0,-10.0);

		//init_GLSL("reflection.vert", "reflection.frag");
		switch_shaders(gs->program[2]); //environment shader
		//switch_shader();
		for (i = 0; i < gs->totalMeshes; i+=3) {
			glPushMatrix();
				glTranslatef(	gs->translateMeshes[i], 
								gs->translateMeshes[i+1], 
								gs->translateMeshes[i+2]);
				
				glScalef(	gs->scaleMeshes[i],
							gs->scaleMeshes[i+1],
							gs->scaleMeshes[i+2]);
				//offObj();
				puddle();

			glPopMatrix();	
		}

	glPopMatrix();
	

	
	glFlush();
	
	//glPopMatrix();
	glutSwapBuffers();

	if (gs->blurMode) {
	//glAccum(GL_ACCUM, 0.90f);
	//glAccum(GL_RETURN, 0.10f);
	glAccum(GL_MULT, 0.10);
	glAccum(GL_ACCUM, 0.90);
	}	
	// Calcualate our FPS and display it in the console every five seconds
    frames++;
    {
    	struct timeval end;
		gettimeofday(&end, NULL);
		totalTime = end.tv_usec;
		totalTime -= start.tv_usec;
		totalTime /= 1000000.0;
		totalTime += end.tv_sec;
		totalTime -= start.tv_sec;
		if (totalTime > 5)
		{
			GLfloat seconds = totalTime;
			GLfloat fps = frames / seconds;
			printf("%d frames in %g seconds = %g FPS\n", frames, seconds, fps);
			start = end;
			frames = 0;
		}
    }
	
}

void
idle(void) {
	display();
}

void reshape(int w, int h)
{
	current_gs->width = w;
	current_gs->height = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

float deltaAngle = 0.0f;
int xOrigin = -1;
float angle = 0.0;

void 
mouse_handler(int button, int button_state, int x, int y){


// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (button_state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}

	//printf("mouse_handler(%d,%d)\n", x, y);
	/*graphics_state * gs = current_gs;
	GLfloat deslocx = ((x > gs->width/2)?(x % gs->width/2):-(gs->width/2 - (x % gs->width/2)))/(float)gs->width;
	GLfloat deslocy = ((y > gs->height/2)?(y % gs->height/2):-(gs->height/2 - (y % gs->height/2)))/(float)gs->height;
	
	gs->lookx += sin(-deslocx);
	gs->looky += sin(-deslocy);
	gs->lookz += sin(-deslocx);
/*	float max = 10;
	if (deslocx < 0) gs->lookx += ((abs(gs->lookx - gs->camx) -max + deslocx > 0)?deslocx:0);
	else gs->lookx += ((abs(gs->lookx - gs->camx) + deslocx < max)?deslocx:0);
	if (deslocy < 0) gs->looky += ((abs(gs->looky - gs->camy) -max + deslocy > 0)?deslocy:0);
	else gs->looky += ((abs(gs->looky - gs->camy) + deslocy < max)?deslocy:0);
	*/
	//printf("cam = (%f, %f, %f); look = (%f,%f,%f)\n", gs->camx, gs->camy, gs->camz, gs->lookx, gs->looky, gs->lookz);
}

void 
trackMotion(int x, int y) {
	//printf("track motion(%d, %d)\n", x, y);
	//mouse_handler(0,0, x, y);
	/*graphics_state * gs = current_gs;
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.01f;

		// update camera's direction
		gs->lookx = sin(deltaAngle);
		gs->lookz = -cos(deltaAngle);
	}*/
	
}

struct vector {
	float x1, x2;
	float y1, y2;
	float z1, z2;
} ;

float multVec(struct vector v1, struct vector v2) {
	return sqrt(abs((v1.x2 - v1.x1)*(v2.x2 - v2.x1) + (v1.y2 - v1.y1)*(v2.y2 - v2.y1) + (v1.z2 - v1.z1)*(v2.z2 - v2.z1)));
}

//float subVec(struct vector v1, struct vector v2) {
//	return sqrt((v1.x2 - v1.x1)*(v2.x2 - v2.x1) + (v1.y2 - v1.y1)*(v2.y2 - v2.y1) + (v1.z2 - v1.z1)*(v2.z2 - v2.z1));
//}


void 
keys(unsigned char c, int x, int y) {

	//quit the program
	if (toupper(c) == 'Q') exit(0);

	graphics_state * gs = current_gs;

	//if(toupper(c) == 'S') saveImage();
	struct vector view;
	view.x1 = gs->lookx;
	view.x2 = gs->camx;
	view.y1 = gs->looky;
	view.y2 = gs->camy;
	view.z1 = gs->lookz;
	view.z2 = gs->camz;
	struct vector movx;
	movx.x1 = gs->lookx;
	movx.x2 = gs->camx;
	movx.y1 = gs->looky;
	movx.y2 = gs->camy;
	movx.z1 = gs->lookz;
	movx.z2 = 0.0;
	
	GLfloat cos_ang = (multVec(view,movx))/(multVec(view,view)*multVec(movx,movx));
	
	if (cos_ang != cos_ang) cos_ang = 1;
	
	GLfloat off = .1;
	
	GLfloat movX = (gs->camx - gs->lookx)*cos_ang;
	GLfloat movZ = (gs->camx - gs->lookx) - (gs->camx - gs->lookx)*cos_ang;

	movX *= off;
	movZ *= off;
	
	if (toupper(c) == 'W') {
		gs->camx -= movZ;
		gs->camz -= movX;
		gs->lookx -= movZ;
		gs->lookz -= movX;
	}
	if (toupper(c) == 'S') {
		gs->camx += movZ;
		gs->camz += movX;
		gs->lookx += movZ;
		gs->lookz += movX;
	}
	if (toupper(c) == 'A') { 
		gs->camx += movX;
		gs->camz += movZ;
		gs->lookx += movX;
		gs->lookz += movZ;
	}
	if (toupper(c) == 'D') {
		gs->camx -= movX;
		gs->camz -= movZ;
		gs->lookx -= movX;
		gs->lookz -= movZ;	
	}
	if (toupper(c) == 'E') gs->camy+=off;
	if (toupper(c) == 'R') gs->camy-=off;
	//if (toupper(c) == 'Z') gs->rotation+=off*20;
	//if (toupper(c) == 'X') gs->rotation-=off*20;
	if (toupper(c) == 'K') gs->looky-=off;
	if (toupper(c) == 'I') gs->looky+=off; 
	if (toupper(c) == 'L') gs->lookz-=off;
	if (toupper(c) == 'J') gs->lookz+=off;
	if (toupper(c) == 'B') {
		gs->blurMode = !gs->blurMode;
		printf("BLUR MODE = %d\n", gs->blurMode);
	}


	if (toupper(c) == '+') {
		gs->scalex+= off/10.0;
		gs->scaley+= off/10.0;
		gs->scalez+= off/10.0;
	}
	if (toupper(c) == '-') {
		gs->scalex-= off/10.0;
		gs->scaley-= off/10.0;
		gs->scalez-= off/10.0;
	}

	//printf("cam = (%f, %f, %f); look = (%f,%f,%f)\n", gs->camx, gs->camy, gs->camz, gs->lookx, gs->looky, gs->lookz);



}

