#pragma once
#include "..\nclgl\SceneNode.h"

class SpaceShip : public SceneNode {
public:
	//�ӵ������������д
	SpaceShip(Mesh* spaceShip, Mesh* fire);
	~SpaceShip(void);

	void Update(float dt) override;

protected:
	SceneNode* shipBody;
	SceneNode* boostFire;
	SceneNode* leftBullet;
	SceneNode* rightBullet;
};