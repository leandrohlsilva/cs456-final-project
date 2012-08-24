#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "jmesh.h"
#include "offreader.h"
#include "utils.h"
#include "reader.h"

jmesh * 
new_jmesh(FILE * fp, int type)
{
   jmesh * mesh;

   mesh = malloc(sizeof(jmesh));

   loadMesh(fp, type, mesh);

//   load_off_mesh(fp, mesh);

	if (mesh->nvert > 0) {
		
		float min_x = mesh->vertices[0];
		float max_x = mesh->vertices[0];
		float min_y = mesh->vertices[1];
		float max_y = mesh->vertices[1];
		float min_z = mesh->vertices[2];
		float max_z = mesh->vertices[2];
		float centroid_x = 0, centroid_y = 0, centroid_z = 0;
		
		int i;

		for (i = 0; i < mesh->ntri; i++) {
			min_x = min(min_x, mesh->vertices[mesh->triangles[i*3]*3]);
			min_x = min(min_x, mesh->vertices[mesh->triangles[i*3+1]*3]);
			min_x = min(min_x, mesh->vertices[mesh->triangles[i*3+2]*3]);

			min_y = min(min_y, mesh->vertices[mesh->triangles[i*3]*3+1]);
			min_y = min(min_y, mesh->vertices[mesh->triangles[i*3+1]*3+1]);
			min_y = min(min_y, mesh->vertices[mesh->triangles[i*3+2]*3+1]);

			min_z = min(min_z, mesh->vertices[mesh->triangles[i*3]*3+2]);
			min_z = min(min_z, mesh->vertices[mesh->triangles[i*3+1]*3+2]);
			min_z = min(min_z, mesh->vertices[mesh->triangles[i*3+2]*3+2]);

			max_x = max(max_x, mesh->vertices[mesh->triangles[i*3]*3]);
			max_x = max(max_x, mesh->vertices[mesh->triangles[i*3+1]*3]);
			max_x = max(max_x, mesh->vertices[mesh->triangles[i*3+2]*3]);

			max_y = max(max_y, mesh->vertices[mesh->triangles[i*3]*3+1]);
			max_y = max(max_y, mesh->vertices[mesh->triangles[i*3+1]*3+1]);
			max_y = max(max_y, mesh->vertices[mesh->triangles[i*3+2]*3+1]);

			max_z = max(max_z, mesh->vertices[mesh->triangles[i*3]*3+2]);
			max_z = max(max_z, mesh->vertices[mesh->triangles[i*3+1]*3+2]);
			max_z = max(max_z, mesh->vertices[mesh->triangles[i*3+2]*3+2]);
		}

		for(i=0;i<mesh->nvert;i++){
			centroid_x += mesh->vertices[i*3];
			centroid_y += mesh->vertices[i*3+1];
			centroid_z += mesh->vertices[i*3+2];
		}

		centroid_x /= (float) mesh->nvert;
		centroid_y /= (float) mesh->nvert;
		centroid_z /= (float) mesh->nvert;

		mesh->min_x = min_x;
		mesh->max_x = max_x;
		mesh->min_y = min_y;
		mesh->max_y = max_y;
	    mesh->min_z = min_z;
		mesh->max_z = max_z;
		mesh->centroid_x = centroid_x;
		mesh->centroid_y = centroid_y;
		mesh->centroid_z = centroid_z;
	}
	return mesh;
}

void
free_jmesh(jmesh*mesh)
{

	free(mesh);

}

void
computeNormals(jmesh *mesh) {

	int i;

	if (mesh->normals == NULL) {
		//printf("normals are NULL\n");
		mesh->normals = (float *) malloc(sizeof(float)*mesh->nvert*3);
	} else {
		mesh->normals = (float *) malloc(sizeof(float)*mesh->nvert*3);
	}

	for (i = 0; i < mesh->nvert; i++) {
		mesh->normals[i*3 + 0] = 0.0;
		mesh->normals[i*3 + 1] = 0.0;
		mesh->normals[i*3 + 2] = 0.0;

	}
	//calculate per triangle
	for (i = 0; i < mesh->ntri; i++) {

		//vertex a
		float ax = mesh->vertices[mesh->triangles[i*3 + 0]*3 + 0];
		float ay = mesh->vertices[mesh->triangles[i*3 + 0]*3 + 1];
		float az = mesh->vertices[mesh->triangles[i*3 + 0]*3 + 2];

		//i++;
		//vertex b
		float bx = mesh->vertices[mesh->triangles[i*3 + 1]*3 + 0];
		float by = mesh->vertices[mesh->triangles[i*3 + 1]*3 + 1];
		float bz = mesh->vertices[mesh->triangles[i*3 + 1]*3 + 2];

		//i++;
		//vertex c
		float cx = mesh->vertices[mesh->triangles[i*3 + 2]*3 + 0];
		float cy = mesh->vertices[mesh->triangles[i*3 + 2]*3 + 1];
		float cz = mesh->vertices[mesh->triangles[i*3 + 2]*3 + 2];


		float ux, uy, uz;
		ux = bx - ax;
		uy = by - ay;
		uz = bz - az;

		float vx, vy, vz;
		vx = cx - ax;
		vy = cy - ay;
		vz = cz - az;

		float nx, ny, nz;
		nx = (uy*vz) - (uz*vy);
		ny = (uz*vx) - (ux*vz);
		nz = (ux*vy) - (uy*vx);


		mesh->normals[mesh->triangles[i*3 + 0]*3 + 0] += nx;
		mesh->normals[mesh->triangles[i*3 + 0]*3 + 1] += ny;
		mesh->normals[mesh->triangles[i*3 + 0]*3 + 2] += nz;
		mesh->normals[mesh->triangles[i*3 + 1]*3 + 0] += nx;
		mesh->normals[mesh->triangles[i*3 + 1]*3 + 1] += ny;
		mesh->normals[mesh->triangles[i*3 + 1]*3 + 2] += nz;
		mesh->normals[mesh->triangles[i*3 + 2]*3 + 0] += nx;
		mesh->normals[mesh->triangles[i*3 + 2]*3 + 1] += ny;
		mesh->normals[mesh->triangles[i*3 + 2]*3 + 2] += nz;

		//printf("%d - (%f, %f, %f)\n", i, nx, ny, nz);


	}
	/*for (i = 0; i < mesh->nvert; i++) {
		GLfloat nx = mesh->normals[i*3 + 0];
		GLfloat ny = mesh->normals[i*3 + 1];
		GLfloat nz = mesh->normals[i*3 + 2];

		GLfloat len = sqrt(nx*nx + ny*ny + nz*nz);

		mesh->normals[i*3 + 0] = nx/len;
		mesh->normals[i*3 + 1] = ny/len;
		mesh->normals[i*3 + 2] = nz/len;

		//printf("%d - (%f, %f, %f)\n", i, 
		//		mesh->normals[i*3 + 0], mesh->normals[i*3 + 1], mesh->normals[i*3 + 2]);

	}*/


	printf("Calculation of normals is completed\n");
}

void
printSummary(jmesh *mesh)
{
	if (mesh->nvert > 0) {
		printf("Number of vertices = %d\n", mesh->nvert);
		printf("number of triangles = %d\n", mesh->ntri);
		printf("min_x = %f\n", mesh->min_x);
		printf("max_x = %f\n", mesh->max_x);
		printf("min_y = %f\n", mesh->min_y);
		printf("max_y = %f\n", mesh->max_y);
		printf("min_z = %f\n", mesh->min_z);
		printf("max_z = %f\n", mesh->max_z);
		printf("centroid_x = %f\n", mesh->centroid_x);
		printf("centroid_y = %f\n", mesh->centroid_y);
		printf("centroid_z = %f\n", mesh->centroid_z);
	}

		

}
