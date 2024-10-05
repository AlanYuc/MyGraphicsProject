#version 330 core

uniform sampler2D diffuseTex ;
uniform samplerCube cubeTex ;

uniform vec3 cameraPos ;

in Vertex {
	vec4 colour ;
	vec2 texCoord ;
	vec3 normal ;
	vec3 worldPos ;
} IN;

out vec4 fragColour ;
void main ( void ) {
	vec4 diffuse = texture ( diffuseTex , IN.texCoord );
	vec3 viewDir = normalize ( cameraPos - IN.worldPos );

	//reflect是GLSL的函数，可以直接计算反射向量
	//这次方向向量不再看向相机，而是直接看向fragment（应该指光线接触的顶点片段）
	vec3 reflectDir = reflect (- viewDir , normalize (IN.normal ));
	vec4 reflectTex = texture ( cubeTex , reflectDir );

	//反射光和水的颜色进行混合
	fragColour = reflectTex + ( diffuse * 0.25f);
 }