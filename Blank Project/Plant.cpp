#include "Plant.h"

Plant::Plant(Vector3 pos, float scale, float rotation, int selectPlant) {
	if (selectPlant == 1) {
		pMesh = Mesh::LoadFromMeshFile("PT_Pine_Tree_03_green.msh");
		tex = SOIL_load_OGL_texture(TEXTUREDIR"PT_Pine_Tree_01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	}
	else {
		pMesh = Mesh::LoadFromMeshFile("PT_Fruit_Tree_01_green.msh");
		tex = SOIL_load_OGL_texture(TEXTUREDIR"PT_Fruit_Tree_Leaves_01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	}

	this->position = pos;
	this->scale = scale;
	this->rotation = rotation;
}
