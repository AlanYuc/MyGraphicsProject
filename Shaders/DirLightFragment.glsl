#version 330 core

//做方向光，应该就是平行光，就不需要衰减，光照半径等

uniform sampler2D	diffuseTex ;
uniform sampler2D	bumpTex;
uniform vec3		cameraPos;
uniform vec4		lightColour;
uniform vec3		lightPos;
uniform float		lightRadius;
uniform vec3		lightCenter;



in Vertex {
	vec4 colour;
	vec3 normal;
	vec2 texCoord;
	vec3 worldPos;
	vec3 tangent;
	vec3 binormal;
} IN;

out vec4 fragColor ;

void main ( void ) {

	vec3 incident = normalize(lightPos - lightCenter);//入射光线向量
	vec3 viewDir = normalize(cameraPos - IN.worldPos);//相机视角方向向量
	vec3 halfDir = normalize(incident + viewDir);//用于计算镜面反射

	mat3 TBN = mat3( normalize(IN.tangent),
		normalize(IN.binormal), normalize(IN.normal));

	//漫反射需要物体表面的颜色，因此在纹理坐标处先进行采样
	vec4 diffuse = texture(diffuseTex , IN.texCoord);

	vec3 bumpNormal = texture(bumpTex, IN.texCoord).rgb;
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

	//点乘计算打入平面的光的数量
	//max函数以及最后一个参数0，是为了避免光的数量为负数
	float lambert = max(dot(incident , IN.normal), 0.0f);


	//处理镜面反射
	float specFactor = clamp(dot(halfDir , IN.normal), 0.0f , 1.0f);
	specFactor = pow(specFactor , 60.0f);

	//设置最终的颜色
	//先设置表面的颜色
	vec3 surface = (diffuse.rgb * lightColour.rgb);


	if(lightRadius > 0.0f){
		//方向光没有衰减，不需要距离
		float distance = length(lightPos - IN.worldPos);

		//没有衰减
		float attenuation = 1.0f - clamp(distance/lightRadius , 0.0f, 1.0f);

		fragColor.rgb = surface * lambert * attenuation;
		fragColor.rgb +=(lightColour.rgb * specFactor) * attenuation * 0.33f;
	}
	else{
		//漫反射颜色的计算
		fragColor.rgb = surface * lambert;

		//镜面反射不需要表面的颜色，所以不用乘surface
		//0.33是为了降低镜面反射光的强度，可以自行修改或者不加
		fragColor.rgb +=(lightColour.rgb * specFactor) * 0.33f;
	}

	//加入一个小的固定环境光ambient
	fragColor.rgb +=surface * 0.1f;

	//透明度不动，直接使用采样的
	fragColor.a = diffuse.a;
}