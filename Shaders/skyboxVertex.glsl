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

	//天空盒和后期处理类似，都是在相机上渲染一个覆盖屏幕的quad
	//所以当相机旋转时，视域的变化会导致天空盒的四边进行伸展，这个过程需要透视矩阵projMatrix
	mat4 invproj = inverse ( projMatrix );

	//拉伸xy轴让veiw向量适应视域
	pos.xy *= vec2 ( invproj[0][0] , invproj[1][1]);
	pos.z = -1.0f;

	//而天空盒的旋转是跟随相机旋转的，因此需要viewMatrix
	//将viewMatrix转化为mat3，位移信息在viewMatrix的第四列，
	//而这里只需要让它旋转，并不移动，因此是需要丢弃位移信息
	OUT.viewDir = transpose( mat3 ( viewMatrix )) * normalize(pos );
	gl_Position = vec4 ( position , 1.0);
}
