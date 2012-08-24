#ifndef _JMESH_H_
#define _JMESH_H_

#include <stdio.h>
#include <GL/glu.h>

typedef struct {
  int nvert;
  int ntri;
  GLfloat * vertices;
  int * triangles;
  GLfloat * normals;
  float min_x, max_x, min_y, max_y, min_z, max_z;
  float centroid_x, centroid_y, centroid_z;
} jmesh;


/* return value: pointer to jmesh upon success, NULL upon failure*/
jmesh * new_jmesh(FILE * file, int type);
void free_jmesh(jmesh * mesh);
void computeNormals(jmesh *mesh);

void printSummary(jmesh * mesh);

#endif
