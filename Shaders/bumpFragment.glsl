#version 330 core

uniform sampler2D	diffuseTex ;
uniform sampler2D	bumpTex;//new
uniform vec3		cameraPos;
uniform vec4		lightColour;
uniform vec3		lightPos;
uniform float		lightRadius;


in Vertex {
	//这里的接口顺序必须和顶点着色器的完全相同
	vec4 colour;
	vec3 normal;
	vec2 texCoord;
	vec3 worldPos;
	vec3 tangent;//New ! Note , Vec3 !
	vec3 binormal;//new
} IN;

out vec4 fragColor ;

void main ( void ) {

	vec3 incident = normalize(lightPos - IN.worldPos);//入射光线向量
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

	//计算距离，方便做远距离的光的强度衰减
	float distance = length(lightPos - IN.worldPos);

	//衰减的计算公式
	//clamp函数是将数据裁剪到0.0 到 1.0 的范围
	float attenuation = 1.0f - clamp(distance/lightRadius , 0.0f, 1.0f);

	//处理镜面反射
	float specFactor = clamp(dot(halfDir , IN.normal), 0.0f , 1.0f);
	specFactor = pow(specFactor , 60.0f);

	//设置最终的颜色
	//先设置表面的颜色
	vec3 surface = (diffuse.rgb * lightColour.rgb);

	//漫反射颜色的计算
	fragColor.rgb = surface * lambert * attenuation;

	//镜面反射不需要表面的颜色，所以不用乘surface
	//0.33是为了降低镜面反射光的强度，可以自行修改或者不加
	fragColor.rgb +=(lightColour.rgb * specFactor) * attenuation * 0.33f;

	//加入一个小的固定环境光ambient
	fragColor.rgb +=surface * 0.1f;

	//透明度不动，直接使用采样的
	fragColor.a = diffuse.a;
}