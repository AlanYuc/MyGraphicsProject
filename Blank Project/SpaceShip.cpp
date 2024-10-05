#include "SpaceShip.h"

SpaceShip::SpaceShip(Mesh* spaceShip, Mesh* fire) {
	shipBody = new SceneNode(spaceShip, Vector4(0, 0, 1, 1));
	shipBody->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
	shipBody->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	AddChild(shipBody);

	boostFire = new SceneNode(fire, Vector4(1, 0, 0, 1));
	boostFire->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
	boostFire->SetTransform(Matrix4::Translation(Vector3(-1000, 0, -4200)));
	shipBody->AddChild(boostFire);

	shipBody->SetBoundingRadius(1000.0f);
	boostFire->SetBoundingRadius(5000.0f);

}

SpaceShip::~SpaceShip(void) {

}

void SpaceShip::Update(float dt) {
	//transform = transform * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0));

	boostFire->SetTransform(
		boostFire->GetTransform()*
		//Matrix4::Translation(Vector3(0, 0, 100 * sin(dt))) *
		Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0))
		//Matrix4::Scale(Vector3(0.5, 0.5, 0.5))
	);

	SceneNode::Update(dt);
}