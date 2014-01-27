#include "mesh_import.h"

using namespace std;

vector<float> g_vp, g_vt, g_vn;
int g_point_count = 0;

bool load_mesh(const char *file_name) {
	const aiScene *scene = aiImportFile(file_name, aiProcess_Triangulate);
	if(!scene) {
		fprintf(stderr, "error reading mesh %s\n", file_name);
		return false;
	}

	printf("%i animations\n", scene->mNumAnimations);
	printf("%i cameras\n", scene->mNumCameras);
	printf("%i lights\n", scene->mNumLights);
	printf("%i materials\n", scene->mNumMaterials);
	printf("%i meshes\n", scene->mNumMeshes);
	printf("%i textures\n", scene->mNumTextures);

	for(unsigned int mi = 0; mi < scene->mNumMeshes; mi++) {
		const aiMesh *mesh = scene->mMeshes[mi];
		printf("%i verts in mesh\n", mesh->mNumVertices);

		g_point_count = mesh->mNumVertices;
		for(unsigned int vi = 0; vi < mesh->mNumVertices; vi++) {
			if(mesh->HasPositions()) {
				const aiVector3D *vp = &(mesh->mVertices[vi]);
				printf ("vp %i (%f,%f,%f)\n", vi, vp->x, vp->y, vp->z);
				g_vp.push_back(vp->x);
				g_vp.push_back(vp->y);
				g_vp.push_back(vp->z);
			}
			if(mesh->HasNormals()) {
				const aiVector3D *vn = &(mesh->mNormals[vi]);
				printf("vn %i (%f,%f,%f)\n", vi, vn->x, vn->y, vn->z);
				g_vn.push_back(vn->x);
				g_vn.push_back(vn->y);
				g_vn.push_back(vn->z);
			}
			if(mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][vi]);
				printf("vt %i (%f, %f)\n", vi, vt->x, vt->y);
				g_vt.push_back(vt->x);
				g_vt.push_back(vt->y);
			}
			if(mesh->HasTangentsAndBitangents()) {

			}
		}
	}
	return true;
}
