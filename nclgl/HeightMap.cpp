#include "HeightMap.h"
#include <iostream>

HeightMap::HeightMap(const std::string& name) {
	int iWidth, iHeight, iChans;
	unsigned char* data = SOIL_load_image(name.c_str(), &iWidth, &iHeight, &iChans, 1);
	if (!data) {
		std::cout << "Heightmap can 't load file !\n";
		return;
	}

	//一个像素一个点，所以顶点数就是像素数
	numVertices = iWidth * iHeight;
	//一个网格包含两个三角六个顶点索引
	//为确保最后一行和列有足够的顶点给网格，将宽高全部减一
	numIndices = (iWidth - 1) * (iHeight - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	Vector3 vertexScale = Vector3(16.0f, 1.0f, 16.0f);
	Vector2 textureScale = Vector2(1 / 16.0f, 1 / 16.0f);

	//下面两个循环多看一下教程
	//第一个是将线性一维的data数组中的图片信息转变成用offset来查找的二维数组
	for (int z = 0; z < iHeight; z++) {
		for (int x = 0; x < iWidth; x++) {
			int offset = (z * iWidth) + x;

			//**************************
			// 在Vector3(x, data[offset], z)中，x和z决定了它的位置，也就是它是高度图平面中的哪个像素
			// data[offset]则是根据索引找到data中的数据，根据这个值的不同，就可以形成一个有高度的山峰
			// 默认是Vector3(x, data[offset], z)，这里给data[offset]乘了5，让山峰更高一点
			//**************************
			vertices[offset] = Vector3(x, data[offset] * 5, z) * vertexScale;
			textureCoords[offset] = Vector2(x, z) * textureScale;
		}
	}
	SOIL_free_image_data(data);

	//这里是按照教程中a,b,c,d的顶点顺序分别获取了每个顶点的offset
	int i = 0;
	for (int z = 0; z < iHeight - 1; z++) {
		for (int x = 0; x < iWidth - 1; x++) {
			int a = (z * (iWidth)) + x;
			int b = (z * (iWidth)) + (x + 1);
			int c = ((z + 1) * (iWidth)) + (x + 1);
			int d = ((z + 1) * (iWidth)) + x;

			indices[i++] = a;
			indices[i++] = c;
			indices[i++] = b;

			indices[i++] = c;
			indices[i++] = a;
			indices[i++] = d;
		}
	}

	//Tutorial  11
	GenerateNormals();
	//Tutorial  12
	GenerateTangents();

	BufferData();

	heightmapSize.x = vertexScale.x * (iWidth - 1);
	heightmapSize.y = vertexScale.y * 255.0f;// each height is a byte !
	heightmapSize.z = vertexScale.z * (iHeight - 1);

}