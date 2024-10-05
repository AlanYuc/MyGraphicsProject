#include "Planet.h"

Planet::Planet(Mesh* cube):SceneNode() {
	GLuint earthDayTex = SOIL_load_OGL_texture(TEXTUREDIR"2k_earth_daymap.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	GLuint moonTex = SOIL_load_OGL_texture(TEXTUREDIR"2k_moon.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	GLuint marsTex = SOIL_load_OGL_texture(TEXTUREDIR"2k_mars.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	earth = new SceneNode(cube, Vector4(1, 1, 1, 1));//red
	earth->SetModelScale(Vector3(100, 100,100));
	earth->SetTransform(Matrix4::Translation(Vector3(0, -500, 0)));
	earth->SetTexture(earthDayTex);
	AddChild(earth);

	moon = new SceneNode(cube, Vector4(1, 1, 1, 1));//green
	moon->SetModelScale(Vector3(100, 100, 100));
	moon->SetTransform(Matrix4::Translation(Vector3(0, 0, 800)));
	moon->SetTexture(moonTex);
	earth->AddChild(moon);

	mars = new SceneNode(cube, Vector4(1, 1, 0.5f, 1));
	mars->SetModelScale(Vector3(100, 100, 100));
	mars->SetTransform(Matrix4::Translation(Vector3(1600, 0, 0)));
	mars->SetTexture(marsTex);
	earth->AddChild(mars);

	earth->SetBoundingRadius(800.0f);
	moon->SetBoundingRadius(400.0f);

	mars->SetBoundingRadius(300.0f);

}

void Planet::Update(float dt) {
	transform = transform * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0));

	//earth->SetTransform(earth->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));
	moon->SetTransform(moon->GetTransform() * Matrix4::Rotation(-60.0f * dt, Vector3(1, 0, 0)));
	mars->SetTransform(mars->GetTransform() * Matrix4::Rotation(10.0f * dt, Vector3(1, 0, 0)));

	SceneNode::Update(dt);
}
