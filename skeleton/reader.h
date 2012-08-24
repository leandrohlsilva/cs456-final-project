#ifndef _READER_H_
#define _READER_H_
#include <stdlib.h>
#include "jmesh.h"

#define OFF_FILE 1
#define OBJ_FILE 2
#define PLY_FILE 3


void loadMesh(FILE *, int, jmesh *);

#endif
