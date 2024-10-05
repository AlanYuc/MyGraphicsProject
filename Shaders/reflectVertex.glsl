#version 330 core


uniform mat4 modelMatrix ;
uniform mat4 viewMatrix ;
uniform mat4 projMatrix ;
uniform mat4 textureMatrix ;

in vec3 position ;
in vec3 normal ;
in vec2 texCoord ;

out Vertex {
	vec3 colour ;
	vec2 texCoord ;
	vec3 normal ;
	vec3 worldPos;
} OUT ;

void main( void ) {
	
	mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));

	//将2维的纹理坐标先转成4维，然后通过textureMatrix（在renderer中处理好的）
	//做一次点乘，完成纹理变换，这里是放大了纹理
	//再提取新的纹理变换矩阵的xy，把新的纹理坐标提取出来
	OUT.texCoord = ( textureMatrix * vec4 ( texCoord , 0.0 , 1.0)).xy;

	OUT.normal = normalize ( normalMatrix * normalize ( normal ));

	vec4 worldPos = ( modelMatrix * vec4 ( position ,1));

	OUT.worldPos = worldPos.xyz ;

	gl_Position = ( projMatrix * viewMatrix ) * worldPos ;
 }

