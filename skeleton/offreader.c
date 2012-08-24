#include "offreader.h"
#include "jmesh.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int load_off_mesh(FILE *file, jmesh *mesh) {

	if (file == NULL) {
		fprintf(stderr, "File is null");
		return -1;
	}

	char buffer[200];

	//getting first lines - hearder
	fscanf(file, "%s", buffer);

	if (strcmp(buffer, "OFF") != 0) {
		fprintf(stderr, "File is not OFF");
		return -1;
	}

	strcpy(buffer, "");

	//number of points, faces and edges
	int numVertex, numFaces, numEdges;
	fscanf(file, "%d %d %d", &numVertex, &numFaces, &numEdges);

	if (numVertex < 0 || numEdges < 0) {
		fprintf(stderr, "Incorrect number of points or number of edges");
		return -1;
	}

	mesh->nvert = numVertex;

	mesh->vertices = (float *) malloc(sizeof(float)*numVertex*3);
	mesh->triangles = (int *) malloc(sizeof(int));

	mesh->ntri = 0;

	float vert;
	int i;
	for (i = 0; i < numVertex*3; i++) {
		fscanf(file, "%f", &vert);
		mesh->vertices[i] = vert;
	}
	//printf("%d Vertex values have been read.\n", i);
	fgets(buffer, 200, file);	
	int v, counter = 0;
	while (!feof(file) && counter < numFaces) {

		fscanf(file, "%[^\n]\n", buffer);
		//printf("What is inside the buffer = %s\n", buffer);

		char *pch;

		pch = strtok(buffer, " ");

		sscanf(pch, "%d" , &v);
		//read v - 2 triangles
		if (v < 3) {
			fprintf(stderr, "Incorrect number of vertices to form a triangle (face number %d)", counter);
			return -1;
		}

		//counting new triangles and after that malloc those in the vector
		mesh->triangles = (int *) realloc(mesh->triangles, (mesh->ntri + (v - 2))*3*sizeof(int));

		for (i = 0; i < v; i++) {
			pch = strtok(NULL, " ");

			if (pch == NULL) {
				fprintf(stderr, "Malformed face number %d\n", counter);
				return -1;
			}
			if (i > 2) {
				mesh->triangles[mesh->ntri*3+i] = mesh->triangles[mesh->ntri*3+i-3]; ++i;
				mesh->triangles[mesh->ntri*3+i] = mesh->triangles[mesh->ntri*3+i-2]; ++i;
			}
			mesh->triangles[mesh->ntri*3+i] = atoi(pch);
		}
		//more possible values like RGB color of the face are discarded
		counter++;
		mesh->ntri += v - 2;
	}

	//printf("%d faces have been found\n", counter);

	return 1;

}
