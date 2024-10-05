#pragma once

#include "Plane.h"

//These let the compiler know that these keywords
//are classes , without including the whole header
class SceneNode;
class Matrix4;

class Frustum {
public:
	Frustum(void) {};
	~Frustum(void) {};

	void FromMatrix(const Matrix4& mvp);
	bool InsideFrustum(SceneNode& n);

protected:
	Plane planes[6];
};