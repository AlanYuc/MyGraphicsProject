#version 330 core


//处理方法是高斯模糊
//采用的一维采样

uniform sampler2D sceneTex ;

uniform int isVertical ;

in Vertex {
	vec2 texCoord ;
} IN;

out vec4 fragColor ;

//这里设定处理范围是左右三个点
//所以给定一个长度为7的数组来存放所有位置的权重（对最终颜色的影响程度）
//包括两侧各3个点以及中心的点的插值
//所有权重加起来为1（大于1则颜色更亮，小于1更暗）
const float scaleFactors[7] =
	float [](0.006 , 0.061 , 0.242 , 0.383 , 0.242 , 0.061 , 0.006);

void main ( void ) {
	fragColor = vec4 (0 ,0 ,0 ,1);

	vec2 delta = vec2 (0 ,0);

	if( isVertical == 1) {
		delta = dFdy(IN.texCoord );//dFdy可以得到y轴的变化量
	}
	else {
		delta = dFdx(IN.texCoord );
	}
	for (int i = 0; i < 7; i++ ) {
		vec2 offset = delta * (i - 3);
		//下面这句的意思就是对sceneTex纹理进行采样，
		//采样的坐标则是经过偏移后的左右两侧（或者上下，取决于isVertical的值）
		vec4 tmp = texture2D( sceneTex , IN.texCoord.xy + offset );
		//对所有点采样完成后将各个点的颜色乘上权重的值加起来就是该颜色的最终效果
		fragColor += tmp * scaleFactors[i];
	}
}