#ifndef _MESH_IMPORT_H_
#define _MESH_IMPORT_H_

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <stdio.h>

using namespace std;

unsigned int load_mesh(const char *file_name, vector<float> *g_vp, vector<float> *g_vt, vector<float> *g_vn);

#endif
