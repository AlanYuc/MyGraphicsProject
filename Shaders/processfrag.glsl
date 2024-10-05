#version 330 core


//�������Ǹ�˹ģ��
//���õ�һά����

uniform sampler2D sceneTex ;

uniform int isVertical ;

in Vertex {
	vec2 texCoord ;
} IN;

out vec4 fragColor ;

//�����趨����Χ������������
//���Ը���һ������Ϊ7���������������λ�õ�Ȩ�أ���������ɫ��Ӱ��̶ȣ�
//���������3�����Լ����ĵĵ�Ĳ�ֵ
//����Ȩ�ؼ�����Ϊ1������1����ɫ������С��1������
const float scaleFactors[7] =
	float [](0.006 , 0.061 , 0.242 , 0.383 , 0.242 , 0.061 , 0.006);

void main ( void ) {
	fragColor = vec4 (0 ,0 ,0 ,1);

	vec2 delta = vec2 (0 ,0);

	if( isVertical == 1) {
		delta = dFdy(IN.texCoord );//dFdy���Եõ�y��ı仯��
	}
	else {
		delta = dFdx(IN.texCoord );
	}
	for (int i = 0; i < 7; i++ ) {
		vec2 offset = delta * (i - 3);
		//����������˼���Ƕ�sceneTex������в�����
		//�������������Ǿ���ƫ�ƺ���������ࣨ�������£�ȡ����isVertical��ֵ��
		vec4 tmp = texture2D( sceneTex , IN.texCoord.xy + offset );
		//�����е������ɺ󽫸��������ɫ����Ȩ�ص�ֵ���������Ǹ���ɫ������Ч��
		fragColor += tmp * scaleFactors[i];
	}
}