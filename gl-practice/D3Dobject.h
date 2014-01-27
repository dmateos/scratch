#ifndef _D3DObject_H_
#define _D3DObject_H_

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

class D3DObject {
	public:
		std::vector<float> verticies, textures, normals;
		int verticies_count;

		D3DObject(std::string filename);
};

#endif
