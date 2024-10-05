#version 330 core

//������⣬Ӧ�þ���ƽ�й⣬�Ͳ���Ҫ˥�������հ뾶��

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

	vec3 incident = normalize(lightPos - lightCenter);//�����������
	vec3 viewDir = normalize(cameraPos - IN.worldPos);//����ӽǷ�������
	vec3 halfDir = normalize(incident + viewDir);//���ڼ��㾵�淴��

	mat3 TBN = mat3( normalize(IN.tangent),
		normalize(IN.binormal), normalize(IN.normal));

	//��������Ҫ����������ɫ��������������괦�Ƚ��в���
	vec4 diffuse = texture(diffuseTex , IN.texCoord);

	vec3 bumpNormal = texture(bumpTex, IN.texCoord).rgb;
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

	//��˼������ƽ��Ĺ������
	//max�����Լ����һ������0����Ϊ�˱���������Ϊ����
	float lambert = max(dot(incident , IN.normal), 0.0f);


	//�����淴��
	float specFactor = clamp(dot(halfDir , IN.normal), 0.0f , 1.0f);
	specFactor = pow(specFactor , 60.0f);

	//�������յ���ɫ
	//�����ñ������ɫ
	vec3 surface = (diffuse.rgb * lightColour.rgb);


	if(lightRadius > 0.0f){
		//�����û��˥��������Ҫ����
		float distance = length(lightPos - IN.worldPos);

		//û��˥��
		float attenuation = 1.0f - clamp(distance/lightRadius , 0.0f, 1.0f);

		fragColor.rgb = surface * lambert * attenuation;
		fragColor.rgb +=(lightColour.rgb * specFactor) * attenuation * 0.33f;
	}
	else{
		//��������ɫ�ļ���
		fragColor.rgb = surface * lambert;

		//���淴�䲻��Ҫ�������ɫ�����Բ��ó�surface
		//0.33��Ϊ�˽��;��淴����ǿ�ȣ����������޸Ļ��߲���
		fragColor.rgb +=(lightColour.rgb * specFactor) * 0.33f;
	}

	//����һ��С�Ĺ̶�������ambient
	fragColor.rgb +=surface * 0.1f;

	//͸���Ȳ�����ֱ��ʹ�ò�����
	fragColor.a = diffuse.a;
}