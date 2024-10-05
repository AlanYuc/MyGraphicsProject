#pragma once

#include "..\nclgl\SceneNode.h"
#include "..\nclgl\Mesh.h"

class Planet :public SceneNode {
public:
	Planet(Mesh* cube);
	~Planet(void) {};

	void Update(float dt) override;




protected:
	SceneNode* earth;
	SceneNode* moon;
	SceneNode* mars;

};


