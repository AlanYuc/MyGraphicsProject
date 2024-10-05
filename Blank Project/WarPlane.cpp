#include "Warplane.h"

Warplane::Warplane(Mesh* wp, int tex) :SceneNode() {
	GLuint boostTex = SOIL_load_OGL_texture(TEXTUREDIR"2k_sun.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	GLuint bodytex;

	switch (tex) {
	case(1):
		bodytex = SOIL_load_OGL_texture(TEXTUREDIR"StarSparrow_Red.png",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		break;
	case(2):
		bodytex = SOIL_load_OGL_texture(TEXTUREDIR"StarSparrow_Yellow.png",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		break;
	case(3):
		bodytex = SOIL_load_OGL_texture(TEXTUREDIR"StarSparrow_White.png",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		break;
	default:
		bodytex = SOIL_load_OGL_texture(TEXTUREDIR"StarSparrow_Blue.png",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		break;
	}

	
	Mesh* boostMesh = Mesh::LoadFromMeshFile("FireCube.msh");


	body = new SceneNode(wp, Vector4(1, 1, 1, 1));
	body->SetModelScale(Vector3(50, 50, 50));
	body->SetTransform(Matrix4::Translation(Vector3(0, 2000, 2000)));
	body->SetTexture(bodytex);
	AddChild(body);

	boost = new SceneNode(boostMesh, Vector4(1, 1, 1, 1));
	boost->SetModelScale(Vector3(50, 50, 50));
	boost->SetTransform(Matrix4::Translation(Vector3(0, 0, -800)));
	boost->SetTexture(boostTex);
	body->AddChild(boost);


	body->SetBoundingRadius(800.0f);
	boost->SetBoundingRadius(400.0f);


}

void Warplane::Update(float dt) {
	transform = transform *
		Matrix4::Translation(Vector3(dt *30, dt * 10, dt * 10))*
		Matrix4::Rotation(10.0f * dt, Vector3(0, 1, 0));

	switch (rand() % 3) {
	case(0):
		transform = transform * Matrix4::Rotation(10 * dt, Vector3(0, 1, 0)); 
	case(1):
		transform = transform * Matrix4::Rotation(10 * dt, Vector3(1, 0, 0));
	case(2):
		transform = transform * Matrix4::Rotation(10 * dt, Vector3(0, 0, 1));
	}

	//earth->SetTransform(earth->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));
	//body->SetTransform(body->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(1, 0, 0)));
	boost->SetTransform(boost->GetTransform() * Matrix4::Scale( Vector3(1 - sin(dt * 10), 1 - sin(dt * 10), 1 - sin(dt * 10))));

	SceneNode::Update(dt);
}