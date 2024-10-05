#pragma once


#include "..\nclgl\SceneNode.h"
#include "..\nclgl\Mesh.h"

class Warplane :public SceneNode {
public:
	Warplane(Mesh* wp , int tex);
	~Warplane(void) {};

	void Update(float dt) override;

protected:
	SceneNode* body;
	SceneNode* boost;

};

