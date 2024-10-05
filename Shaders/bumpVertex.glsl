#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec4 colour;
in vec2 texCoord;
in vec3 normal;
in vec4 tangent;//new 

out Vertex{
	vec4 colour;
	vec3 normal;
	vec2 texCoord;
	vec3 worldPos;
	vec3 tangent;//New ! Note , Vec3 !
	vec3 binormal;//new
} OUT;

void main(void){
	OUT.colour = colour;
	OUT.texCoord = texCoord;
	
	//将modelMatrix的左上角3*3的部分进行 逆 转置
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	vec3 wNormal = normalize(normalMatrix * normalize(normal));
	vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));

	OUT.normal = wNormal;
	OUT.tangent = wTangent;
	OUT.binormal = cross(wTangent , wNormal) * tangent.w;


	vec4 worldPos = (modelMatrix * vec4(position , 1));
	OUT.worldPos = worldPos.xyz;

	gl_Position = (projMatrix * viewMatrix) * worldPos;

}



