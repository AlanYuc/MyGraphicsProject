#pragma once
#include "..\nclgl\Vector3.h"
#include "..\nclgl\Vector4.h"
#include "..\nclgl\Mesh.h"
#include "..\nclgl\Matrix4.h"


class  Plant {
public:
	Plant(Vector3 pos, float Scale, float Rotation , int selectPlant);
	~Plant(void) {};
	void SetPosition(Vector3 position) { this->position = position; }
	Vector3 GetPosition() { return position; }

	void SetRotation(float rotation) { this->rotation = rotation; }
	float GetRotation() { return rotation; }

	void SetScale(float scale) { this->scale = scale; }
	Vector3 GetScale() { return Vector3(scale, scale, scale); }

	Mesh* pMesh;
	GLuint tex;

protected:

	float scale;
	Vector3 position;
	float rotation;
};