#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
in vec3 position;

out Vertex {
	vec3 viewDir;
} OUT;

void main ( void ) {
	vec3 pos = position;

	//��պкͺ��ڴ������ƣ��������������Ⱦһ��������Ļ��quad
	//���Ե������תʱ������ı仯�ᵼ����պе��ı߽�����չ�����������Ҫ͸�Ӿ���projMatrix
	mat4 invproj = inverse ( projMatrix );

	//����xy����veiw������Ӧ����
	pos.xy *= vec2 ( invproj[0][0] , invproj[1][1]);
	pos.z = -1.0f;

	//����պе���ת�Ǹ��������ת�ģ������ҪviewMatrix
	//��viewMatrixת��Ϊmat3��λ����Ϣ��viewMatrix�ĵ����У�
	//������ֻ��Ҫ������ת�������ƶ����������Ҫ����λ����Ϣ
	OUT.viewDir = transpose( mat3 ( viewMatrix )) * normalize(pos );
	gl_Position = vec4 ( position , 1.0);
}
