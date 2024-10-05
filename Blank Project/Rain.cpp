#include "Rain.h"
#include "..\nclgl\HeightMap.h"

Rain::Rain(float v, float b, float numParticle)
{
	this->variant = v;
	this->base = b;
	this->numParticles = numParticle;

	mesh = Mesh::LoadFromMeshFile("Mines.msh");
	texture = SOIL_load_OGL_texture(TEXTUREDIR"Mine_EmissionRed.png", 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	 HeightMap* hMap = new HeightMap(TEXTUREDIR "Canyon04_Heightmap.png");
	 radius = hMap->GetHeightmapSize().x;

	for (int i = 0; i < numParticles; i++) {
		Particle particle;
		particle.isAlive = true;
		particle.position = Vector3(rand() % radius, 000, rand() % radius);
		float speed = (rand() % 15) + 8;
		particle.speed = Vector3(0, -speed, -3);
		particle_list.push_back(particle);

		SceneNode* rain = new SceneNode();

		rain->SetMesh(this->mesh);
		rain->SetTransform(Matrix4::Translation(particle.position));
		rain->SetModelScale(Vector3(10, 10, 10));
		rain->SetTexture(texture);
		AddChild(rain);
	}
}

void Rain::Update(float dt)
{
	for (int i = 0; i < numParticles; i++) {
		if (children[i]->GetTransform().GetPositionVector().y <= 0) {
			children[i]->SetTransform(Matrix4::Translation(Vector3(rand() % radius, 8000, rand() % radius)));
			float speed = (rand() % 15) + 8;
			particle_list[i].speed = Vector3(0, -speed, 0);
		}
		children[i]->SetTransform(children[i]->GetTransform() * Matrix4::Translation(particle_list[i].speed));
	}
	SceneNode::Update(dt);
}
