#ifndef _OFF_READER_H_
#define _OFF_READER_H_
#include "utils.h" 
#include <stdio.h>
#include "jmesh.h"

/* return value: number of vertices upon success, -1 upon failure*/
int load_off_mesh(FILE *file, jmesh *mesh);


#endif
