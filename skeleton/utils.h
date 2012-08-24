/* utils.h */
#ifndef _GRAPHICS_UTILS_
#define _GRAPHICS_UTILS_

#include <GL/glu.h>
#include "jmesh.h"

#ifndef max
#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif




struct particle
{
	GLfloat x,y,z;
	GLfloat r,g,b;
	GLfloat xd,yd,zd;
	GLfloat cs;
};


typedef struct {
  int width;        /* width of rendering target */
  int height;       /* height of rendering target */
  jmesh *mesh;
  GLint *image;
  GLfloat camx, camy, camz;
  GLfloat lookx, looky, lookz;
  GLfloat scalex, scaley, scalez;
  GLfloat rotation;
  GLuint program[3];
  GLuint *texNames;
  int mode;
  int nTextures;
  int totalTrees;
  GLfloat *scaleTrees;
  GLfloat *translateTrees;
  int totalMeshes;
  GLfloat *scaleMeshes;
  GLfloat *translateMeshes;
  int totalSnowFlakes;
  struct particle *snowFlakes;
  GLfloat windspeed;
  int winddir;
  int blurMode;
  
} graphics_state;   /* global graphics state */

#ifdef __cplusplus
extern "C" {
#endif
  
  void print_howto(void);

  void init(graphics_state *);
  void set_gs(graphics_state *);

  void offObj(void);
  void display(void);
  void display1(void);
  void idle(void);
  void idle1(void);
  void reshape(int w, int h);
  void mouse_handler(int button, int button_state, int x, int y);
  void trackMotion(int x, int y);
  void keys(unsigned char c,int x, int y);

#ifdef __cplusplus
}
#endif
    
#endif
