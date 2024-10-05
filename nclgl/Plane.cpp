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
//(N��V)+d=0��ƽ�湫ʽ,��չΪ��ֵС�����帺�뾶
//�������ֵΪ�㵽ƽ��ľ��룬Ϊ����˵���뷨����ͬ����Ҳ������ƽ���ǰ�棬����˵�ڲ�
//Ϊ����˵������ƽ��ı��棨���棩
//���ﲻ�Ƚ����İ뾶��Ϊ�������Ѿ���ƽ���ڵ�ʱ������һ���в�����ƽ����
//��ˣ�����ľ���Ϊ���ұȸ��뾶��С��ʱ��˵���㵽ƽ��ľ���Ȱ뾶�󣬲�����Ϊ����ƽ����棬������ȫû����Ⱦ�ı�Ҫ�����Զ���
bool Plane::SphereInPlane(const Vector3& position, float radius) const{
	if (Vector3::Dot(position, normal) + distance <= -radius) {
		return false;
	}
	else {
		return true;
	}
}