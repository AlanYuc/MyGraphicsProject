#include "Plane.h"

Plane::Plane(const Vector3& normal, float distance, bool normalise) {
	if (normalise) {
		float length = sqrt(Vector3::Dot(normal, normal));

		this->normal = normal / length;
		this->distance = distance / length;
	}
	else {
		this->normal = normal;
		this->distance = distance;
	}
}
//(N·V)+d=0的平面公式,拓展为左值小于球体负半径
//这里的左值为点到平面的距离，为正，说明与法向量同方向，也就是在平面的前面，或者说内部
//为负，说明点在平面的背面（后面）
//这里不比较正的半径因为，当点已经在平面内的时候，球体一定有部分在平面内
//因此，当点的距离为负且比负半径还小的时候，说明点到平面的距离比半径大，并且因为点在平面后面，所以完全没有渲染的必要，可以丢弃
bool Plane::SphereInPlane(const Vector3& position, float radius) const{
	if (Vector3::Dot(position, normal) + distance <= -radius) {
		return false;
	}
	else {
		return true;
	}
}