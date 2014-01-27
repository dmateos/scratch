#include "D3DObject.h"

D3DObject::D3DObject(std::string file_name) {
	const aiScene *scene = aiImportFile(file_name.c_str(), aiProcess_Triangulate | aiProcess_MakeLeftHanded);
	unsigned int g_point_count = 0;
	if(!scene) {
		fprintf(stderr, "error reading mesh %s\n", file_name.c_str());
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
				this->verticies.push_back(vp->x);
				this->verticies.push_back(vp->y);
				this->verticies.push_back(vp->z);
			}
			if(mesh->HasNormals()) {
				const aiVector3D *vn = &(mesh->mNormals[vi]);
				printf("vn %i (%f,%f,%f)\n", vi, vn->x, vn->y, vn->z);
				this->normals.push_back(vn->x);
				this->normals.push_back(vn->y);
				this->normals.push_back(vn->z);
			}
			if(mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][vi]);
				printf("vt %i (%f, %f)\n", vi, vt->x, vt->y);
				this->textures.push_back(vt->x);
				this->textures.push_back(vt->y);
			}
			if(mesh->HasTangentsAndBitangents()) {

			}
		}
	}
	this->verticies_count = g_point_count;
	aiReleaseImport(scene);
}
