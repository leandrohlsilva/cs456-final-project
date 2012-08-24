#include <string.h>
#include "reader.h"
#include "jmesh.h"
#include "offreader.h"

void load_obj_mesh(FILE *file, jmesh *mesh) {
		
	if (file == NULL) {
		fprintf(stderr, "File is null");
		return;
	}

	char buffer[200];


	//number of points, faces and edges


	mesh->vertices = (float *) malloc(sizeof(float));
	mesh->triangles = (int *) malloc(sizeof(int));

	mesh->ntri = 0;
	mesh->nvert = 0;
	while (!feof(file)) {

		fscanf(file, "%[^\n]\n", buffer);
		printf("What is inside the buffer = %s\n", buffer);

		char *pch;

		pch = strtok(buffer, " ");

		char option[3];

		sscanf(pch, "%s" , option);


		if (strcmp(option, "v") == 0) {
			mesh->vertices = (float *) realloc(mesh->vertices, (mesh->nvert + 1)*3*sizeof(int));
			pch = strtok(NULL, " ");
			mesh->vertices[mesh->nvert] = atof(pch);
			pch = strtok(NULL, " ");
			mesh->vertices[mesh->nvert + 1] = atof(pch);
			pch = strtok(NULL, " ");
			mesh->vertices[mesh->nvert + 2] = atof(pch);		
			mesh->nvert++;
			continue;
		}

		if (strcmp(option, "f") == 0) {
			int i = 0;
			mesh->triangles = (int *) realloc(mesh->triangles, (mesh->ntri + 1)*3*sizeof(int));
			mesh->ntri++;
			while (pch != NULL) {
				if (i > 2) {
					i = 0;
					mesh->triangles = (int *) realloc(mesh->triangles, (mesh->ntri + 1)*3*sizeof(int));
					mesh->ntri++;
					mesh->triangles[(mesh->ntri - 1)*3+i] = mesh->triangles[mesh->ntri*3+i-2]; ++i;
					mesh->triangles[(mesh->ntri - 1)*3+i] = mesh->triangles[mesh->ntri*3+i-2]; ++i;				
				}
				mesh->triangles[(mesh->ntri - 1)*3+i] = atoi(pch); ++i;
				pch = strtok(NULL, " ");
			}

			continue;
		}
	}
		
		

}

void loadMesh(FILE *f, int type, jmesh *mesh) {
	
	switch (type) {
		case OFF_FILE: {
			load_off_mesh(f,mesh);
			break;
		}
		case OBJ_FILE: {
			load_obj_mesh(f, mesh);
		}

		case PLY_FILE: {

		}

	}


}
