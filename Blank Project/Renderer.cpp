#include "Renderer.h"

#include <algorithm>


#define SHADOWSIZE 2048
const int POST_PASSES	= 10;
const int LIGHT_NUM		= 32;

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	quad			= Mesh::GenerateQuad();
	screenQuad		= Mesh::GenerateQuad();
	waterQuad		= Mesh::GenerateQuad();
	sunMesh			= Mesh::LoadFromMeshFile("Sphere.msh");
	sphere			= Mesh::LoadFromMeshFile("Sphere.msh");
	quadDeferred	= Mesh::GenerateQuad();
	quadGlass		= Mesh::GenerateQuad();
	cubeRobot		= Mesh::LoadFromMeshFile("OffsetCubeY.msh");
	fireCube		= Mesh::LoadFromMeshFile("FireCube.msh");
	

	sceneMeshes.emplace_back(Mesh::GenerateQuad());
	sceneMeshes.emplace_back(Mesh::LoadFromMeshFile("Sphere.msh"));
	sceneMeshes.emplace_back(Mesh::LoadFromMeshFile("Cylinder.msh"));
	sceneMeshes.emplace_back(Mesh::LoadFromMeshFile("Cone.msh"));
	sceneMeshes.emplace_back(Mesh::LoadFromMeshFile("Role_T.msh"));

	spaceShip.emplace_back(Mesh::LoadFromMeshFile("StarSparrow.msh"));
	spaceShip.emplace_back(Mesh::LoadFromMeshFile("StarSparrow1.msh"));
	spaceShip.emplace_back(Mesh::LoadFromMeshFile("StarSparrow2.msh"));
	spaceShip.emplace_back(Mesh::LoadFromMeshFile("StarSparrow3.msh"));
	spaceShip.emplace_back(Mesh::LoadFromMeshFile("StarSparrow4.msh"));
	spaceShip.emplace_back(Mesh::LoadFromMeshFile("StarSparrow5.msh"));

	
	mesh		= Mesh::LoadFromMeshFile("Role_T.msh");
	anim		= new MeshAnimation("Role_T.anm");
	material	= new MeshMaterial("Role_T.mat");
	
	heightMap = new HeightMap(TEXTUREDIR "Canyon04_Heightmap.png");
	Vector3 heightmapSize = heightMap->GetHeightmapSize();


	waterTex = SOIL_load_OGL_texture(
		TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	earthTex = SOIL_load_OGL_texture(
		TEXTUREDIR"Heather_BaseColor.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	earthBump = SOIL_load_OGL_texture(
		TEXTUREDIR"Heather_MaskMap.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	sceneTexture = SOIL_load_OGL_texture(
		TEXTUREDIR"stainedglass.tga", SOIL_LOAD_AUTO, 
		SOIL_CREATE_NEW_ID, 0);
	heightTexture = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	sunTex = SOIL_load_OGL_texture(TEXTUREDIR"2k_sun.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	earthDayTex = SOIL_load_OGL_texture(TEXTUREDIR"2k_earth_daymap.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	moonTex = SOIL_load_OGL_texture(TEXTUREDIR"2k_moon.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	marsTex = SOIL_load_OGL_texture(TEXTUREDIR"2k_mars.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	spaceSparrowTex = SOIL_load_OGL_texture(TEXTUREDIR"StarSparrow_Red.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	rainTex = SOIL_load_OGL_texture(TEXTUREDIR"Mine_EmissionRed.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	if (!earthTex || !earthBump || !cubeMap || !waterTex ||
		!sceneTexture || !sunTex || !earthDayTex || !moonTex ||
		!marsTex || !spaceSparrowTex || !rainTex
		) {
		return;
	}

	vector<std::string> faces
	{
		TEXTUREDIR"rusted_west.jpg",
		TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg",
		TEXTUREDIR"rusted_down.jpg",
		TEXTUREDIR"rusted_south.jpg",
		TEXTUREDIR"rusted_north.jpg"
	};
	cubeMap = loadCubemap(faces);

	vector<std::string> facesSpace
	{
		TEXTUREDIR"diversespace-left.jpg",
		TEXTUREDIR"diversespace-right.jpg",
		TEXTUREDIR"diversespace-up.jpg",
		TEXTUREDIR"diversespace-down.jpg",
		TEXTUREDIR"diversespace-front.jpg",
		TEXTUREDIR"diversespace-back.jpg"
	};
	cubeMapSpace = loadCubemap(facesSpace);



	

	SetTextureRepeating(earthTex,		true);
	SetTextureRepeating(earthBump,		true);
	SetTextureRepeating(waterTex,		true);
	SetTextureRepeating(heightTexture,	true);
	SetTextureRepeating(rainTex,		true);

	reflectShader		=	new Shader("reflectVertex.glsl", "reflectFragment.glsl");
	skyboxShader		=	new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	lightShader			=	new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");
	dirLightShader		=	new Shader("bumpVertex.glsl", "DirLightFragment.glsl");
	sceneShader			=	new Shader("shadowscenevert.glsl", "shadowscenefrag.glsl");
	shadowShader		=	new Shader("shadowVert.glsl", "shadowFrag.glsl");
	SkinningShader		=	new Shader("SkinningVertex.glsl", "TexturedFragment.glsl");
	sceneNodeShader		=	new Shader("SceneVertex.glsl", "SceneFragment.glsl");
	processSceneShader	=	new Shader("TexturedVertex.glsl", "TexturedFragment.glsl");
	processShader		=	new Shader("TexturedVertex.glsl", "processfrag.glsl");
	planet				=	new Shader("MatrixVertex.glsl", "ColourFragment.glsl");
	deferredShader		=	new Shader("BumpVertex.glsl", "bufferFragment.glsl");
	pointlightShader	=	new Shader("pointlightvert.glsl", "pointlightfrag.glsl");
	combineShader		=	new Shader("combinevert.glsl", "combinefrag.glsl");

	if (!reflectShader->LoadSuccess()		||
		!skyboxShader->LoadSuccess()		||
		!lightShader->LoadSuccess()			||
		!dirLightShader->LoadSuccess()		||
		!sceneShader->LoadSuccess()			|| 
		!shadowShader->LoadSuccess()		||
		!SkinningShader->LoadSuccess()		||
		!sceneNodeShader->LoadSuccess()		||
		!processSceneShader->LoadSuccess()	||
		!processShader->LoadSuccess()		||
		!planet->LoadSuccess()				||
		!deferredShader->LoadSuccess()		||
		!pointlightShader->LoadSuccess()	||
		!combineShader->LoadSuccess()) {
		return;

	}
	
	InitPorcess();
	

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	



	root = new SceneNode();
	root->SetTransform(Matrix4::Translation(heightmapSize * Vector3(0.5f, 2.0f, 0.5f)));

	for (int i = 0; i < 5; i++) {
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
		s->SetTransform(Matrix4::Translation( 
			Vector3(0, 100.0f, -300.0f + 100.0f + 100.0f * i)));
		s->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
		s->SetBoundingRadius(100.0f);
		s->SetMesh(quadGlass);
		s->SetTexture(sceneTexture);
		root->AddChild(s);
	}

	CubeRobot* cubeRobotNode = new CubeRobot(cubeRobot);

	root->AddChild(cubeRobotNode);

	SceneNode* sun = new SceneNode(sunMesh, Vector4(1, 1, 0, 1));
	sun->SetModelScale(Vector3(600, 600, 600));
	sun->SetTransform(Matrix4::Translation(Vector3(heightmapSize.x *0.5f, 0, 0)));
	sun->SetBoundingRadius(600);
	sun->SetTexture(sunTex);
	root->AddChild(sun);
	
	root2 = new SceneNode();
	root2->SetTransform(Matrix4::Translation(heightmapSize * Vector3(0.5f, 2.0f, 0.5f)));

	Planet* space = new Planet(sunMesh);
	space->SetTexture(earthDayTex);
	root2->AddChild(space);

	for (int i = 0; i < 5; i++) {
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		s->SetTransform(Matrix4::Translation( 
			Vector3(0, 100.0f, 200.0f * i)));
		s->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		s->SetBoundingRadius(500);

		s->SetMesh(spaceShip[i]);
		s->SetTexture(spaceSparrowTex);
		root2->AddChild(s);
	}

	rainRoot = new SceneNode();
	rainRoot->SetTransform(Matrix4::Translation(heightmapSize * Vector3(0.25f, 2.5f, 0.25f)));

	rain = new Rain(100,100,1000);
	rainRoot->AddChild(rain);
	

	for (int i = 0; i < 4; i++) {
		Warplane* warPlane = new Warplane(spaceShip[i], i + 1);
		warPlane->SetTransform(
			warPlane->GetTransform() *
			Matrix4::Translation(Vector3(800.0f - i * 800.0f, 300 + i * 500, -1200.0f + (i *3) * 350))
		);
		root2->AddChild(warPlane);
	}


	
	AddPlant();


	for (int i = 0; i < mesh->GetSubMeshCount(); i++) {
		const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);

		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

		matTextures.emplace_back(texID);
	}

	sceneDiffuse = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sceneBump = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	SetTextureRepeating(sceneDiffuse, true);
	SetTextureRepeating(sceneBump, true);

	

	dirLight = new Light(heightmapSize * Vector3(1.0f, 1.0f, 0.5f), Vector4(0.8f, 0.8f, 1.0f, 1), -1);
	light = new Light(heightmapSize * Vector3(0.5f, 3.0f, 0.5f),Vector4(1, 1, 1, 1), heightmapSize.x);
	camera = new Camera(0.0f, 0.0f, Vector3(-8.0f, 5.0f, 8.0f) + heightmapSize * Vector3(0.5f, 4.0f, 0.5f));
	light2 = new Light(Vector3(-20.0f, 10.0f, -20.0f) + heightmapSize * Vector3(0.5f, 4.0f, 0.5f), Vector4(1, 1, 1, 1), heightmapSize.x);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	sceneTransforms.resize(6);
	sceneTransforms[0] = Matrix4::Translation(Vector3(heightmapSize * Vector3(0.5f, 2.0f, 0.5f)))*
		Matrix4::Rotation(90, Vector3(1, 0, 0)) *Matrix4::Scale(Vector3(10, 10, 1));
	sun->SetTransform(Matrix4::Translation(Vector3(dirLight->GetPosition())));

	

	pointLights = new Light[LIGHT_NUM];

	for (int i = 0; i < LIGHT_NUM; ++i) {
		Light& l = pointLights[i];
		l.SetPosition(Vector3(rand() % (int)heightmapSize.x,
			600.0f, rand() % (int)heightmapSize.z));

		l.SetColour(Vector4(
			0.5f + (float)(rand() / (float)RAND_MAX),
			0.5f + (float)(rand() / (float)RAND_MAX),
			0.5f + (float)(rand() / (float)RAND_MAX),
			1));

		l.SetRadius(500.0f + (rand() % 500));

	}
	
	glGenFramebuffers(1, &bufferFBODeferred);
	glGenFramebuffers(1, &pointLightFBO);

	GLenum buffers[2] = {
		GL_COLOR_ATTACHMENT0 ,
		GL_COLOR_ATTACHMENT1
	};

	GenerateScreenTexture(bufferDepthTexDeferred, true);
	GenerateScreenTexture(bufferColourTexDeferred);
	GenerateScreenTexture(bufferNormalTexDeferred);
	GenerateScreenTexture(lightDiffuseTex);
	GenerateScreenTexture(lightSpecularTex);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBODeferred);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTexDeferred, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, bufferNormalTexDeferred, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTexDeferred, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, lightDiffuseTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	waterRotate		= 0.0f;
	waterCycle		= 0.0f;

	currentFrame	= 0;
	frameTime		= 0.0f;
	sceneTime		= 0.0f;

	toggleScene					= false;
	toogleAutoCamera			= true;
	toggleGaussianBlur			= false;
	toggleDeffered				= false;
	toggleShowTutorialSample	= false;

	selectTex = 0;

	init = true;
}

Renderer::~Renderer(void) {
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete waterQuad;
	delete sceneShader;
	delete shadowShader;
	delete SkinningShader;
	delete mesh;
	delete anim;
	delete material;
	delete cubeRobot;
	delete sceneNodeShader;
	delete root;
	delete root2;
	delete quadGlass;
	delete processSceneShader;
	delete processShader;
	delete screenQuad;
	delete deferredShader;
	delete combineShader;
	delete pointlightShader;
	delete sphere;
	delete quadDeferred;
	delete[] pointLights;
	delete sunMesh;
	//delete rainRoot;
	//delete rain;

	for (auto& i : sceneMeshes) {
		delete i;
	}
	
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	glDeleteTextures(1, &sceneTexture);
	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
	glDeleteTextures(1, &bufferColourTexDeferred);
	glDeleteTextures(1, &bufferNormalTexDeferred);
	glDeleteTextures(1, &bufferDepthTexDeferred);
	glDeleteTextures(1, &lightDiffuseTex);
	glDeleteTextures(1, &lightSpecularTex);
	glDeleteFramebuffers(1, &bufferFBODeferred);
	glDeleteFramebuffers(1, &pointLightFBO);
	glDeleteTextures(1, &rainTex);
	glDeleteTextures(1, &spaceSparrowTex);
	glDeleteTextures(1, &marsTex);
	glDeleteTextures(1, &moonTex);
	glDeleteTextures(1, &earthDayTex);	
	glDeleteTextures(1, &sunTex);

	for (auto plant : plants) {
		delete plant;
	}
}

void Renderer::InitPorcess() {
	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	for (int i = 0; i < 2; i++) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &processFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::UpdateScene(float dt) {

	if (toogleAutoCamera) {
		camera->UpdateCamera(dt);
	} 
	else{
		camera->UpdateCameraAuto(dt);
	}
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	sceneTime += dt;

	waterRotate += dt * 2.0f;
	waterCycle += dt * 0.25f; 


	for (int i = 1; i < 4; ++i) { 
		Vector3 t = Vector3(-10 + (5 * i), 2.0f + sin(sceneTime * i), 0);
		sceneTransforms[i] = Matrix4::Translation(Vector3(heightMap->GetHeightmapSize() * Vector3(0.5f, 2.0f, 0.5f)))*
			Matrix4::Translation(t) *
			Matrix4::Rotation(sceneTime * 10 * i, Vector3(1, 0, 0));

	}

	Vector3 t = Vector3(50, 0, 20.0f * sin(sceneTime * 4));
	sceneTransforms[4] = Matrix4::Translation(Vector3(heightMap->GetHeightmapSize() * Vector3(0.5f, 2.5f, 0.5f))) *
		Matrix4::Translation(t) *
		Matrix4::Scale(Vector3(50.0f, 50.0f, 50.0f));
		Matrix4::Rotation(sceneTime * 10, Vector3(0, 1, 0));

	

	frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();
	}

	root->Update(dt);
	root2->Update(dt);
	rainRoot->Update(dt);

}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();

	if (!toggleScene && !toggleGaussianBlur) {

		if (toggleDeffered) {
			RenderDefferedScene();
		}
		else {
			DrawWater();
			DrawHeightmap();
			DrawPlants();
			if (toggleShowTutorialSample) {
				DrawSceneNode(root);
			}
			DrawSoldier();
			DrawRain();
			DrawShadowScene();
			DrawMainScene();
		}
		
	}
	else if (!toggleScene && toggleGaussianBlur) {
		DrawScene();
		DrawPostProcess();
		PresentScene();
	}
	else {
		DrawSceneNode(root2);
	}

}

void Renderer::DrawSkybox() {

	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	glUniform1i(glGetUniformLocation(
		skyboxShader->GetProgram(), "cubeTex"), 0);
	glActiveTexture(GL_TEXTURE0);

	if (!toggleScene) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapSpace);
	}
	

	UpdateShaderMatrices();
	quad->Draw();

	glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightmap() {

	BindShader(dirLightShader);
	SetShaderLight(*dirLight);

	viewMatrix = camera->BuildViewMatrix();

	glUniform3fv(glGetUniformLocation(lightShader->GetProgram(),
		"lightCenter"), 1, (float*)&Vector3(heightMap->GetHeightmapSize() * Vector3(1.0f, 0.0f, 1.0f)));


	glUniform3fv(glGetUniformLocation(dirLightShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(
		dirLightShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex);

	glUniform1i(glGetUniformLocation(
		dirLightShader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthBump);

	modelMatrix.ToIdentity(); 
	textureMatrix.ToIdentity(); 

	UpdateShaderMatrices();

	heightMap->Draw();

}

void Renderer::DrawWater() {
	BindShader(reflectShader);

	glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(
		reflectShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(
		reflectShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);

	//glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	Vector3 hSize = heightMap->GetHeightmapSize();

	modelMatrix =
		Matrix4::Translation(hSize * 0.5f) *
		Matrix4::Scale(hSize * 0.5f) *
		Matrix4::Rotation(90, Vector3(1, 0, 0));

	textureMatrix =
		Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) *
		Matrix4::Scale(Vector3(10, 10, 10)) *
		Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

	UpdateShaderMatrices();
	waterQuad->Draw();

	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Renderer::DrawSoldier() {
	BindShader(SkinningShader);
	glUniform1i(glGetUniformLocation(SkinningShader->GetProgram(), "diffuseTex"), 0);
	UpdateShaderMatrices();

	vector<Matrix4> frameMatrices;
	const Matrix4* invBindPose = mesh->GetInverseBindPose();
	const Matrix4* frameData = anim->GetJointData(currentFrame);

	for (unsigned int i = 0; i < mesh->GetJointCount(); i++) {
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}

	int	j = glGetUniformLocation(SkinningShader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());

	modelMatrix = sceneTransforms[4];
	UpdateShaderMatrices();
	for (int i = 0; i < mesh->GetSubMeshCount(); i++) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		sceneMeshes[4]->DrawSubMesh(i);
	}
}

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	BindShader(shadowShader);
	viewMatrix = Matrix4::BuildViewMatrix(light2->GetPosition(), Vector3(heightMap->GetHeightmapSize() * Vector3(0.5f, 2.0f, 0.5f)));
	projMatrix = Matrix4::Perspective(1, 100, 1, 45);
	shadowMatrix = projMatrix * viewMatrix; 

	for (int i = 0; i < 4; ++i) {
		modelMatrix = sceneTransforms[i];
		UpdateShaderMatrices();
		sceneMeshes[i]->Draw();
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawMainScene() {
	BindShader(sceneShader);
	SetShaderLight(*light2);
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "shadowTex"), 2);

	glUniform3fv(glGetUniformLocation(sceneShader->GetProgram(), "cameraPos"),
		1, (float*)&camera->GetPosition());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneDiffuse);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sceneBump);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	for (int i = 0; i < 4; ++i) {
		modelMatrix = sceneTransforms[i];
		UpdateShaderMatrices();
		sceneMeshes[i]->Draw();
	}

}

void Renderer::DrawScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	DrawSkybox();

	//glEnable(GL_DEPTH_TEST);
	BindShader(processSceneShader);
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(processSceneShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightTexture);

	heightMap->Draw();
	
	BindShader(reflectShader);

	glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(
		reflectShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(
		reflectShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	waterQuad->Draw();
	

	//glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

}

void Renderer::DrawPostProcess() {
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	BindShader(processShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(processShader->GetProgram(), "sceneTex"), 0);
	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 0);

		glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
		screenQuad->Draw();
		glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[0], 0);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[1]);
		screenQuad->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::PresentScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(processSceneShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
	glUniform1i(glGetUniformLocation(processSceneShader->GetProgram(), "diffuseTex"), 0);
	screenQuad->Draw();
}

void Renderer::DrawRain()
{
	DrawSceneNode(rainRoot);
}

void Renderer::AddPlant() {
	for (int i = 0; i < 15; i++) {
		/*int w, h, c;
		unsigned char* heightMapData = SOIL_load_image(TEXTUREDIR "Canyon04_Heightmap.png", &w, &h, &c, 1);
		int randomH = rand() % 4000 - 2000;
		int randomW = rand() % 4000 - 2000;
		int offset = (randomH + 2000) * w + (randomW + 2000);
		float radius = heightMap->GetHeightmapSize().x;
		float height = heightMap->GetHeightmapSize().y;
		Plant* newPlant = new Plant(Vector3(randomH + radius, heightMapData[offset] * 5, randomW + radius), 100, 90);*/
		Plant* newPlant = new Plant(Vector3(rand() % 4000 + heightMap->GetHeightmapSize().x / 2.0f, 150, rand() % 4000 + heightMap->GetHeightmapSize().x / 2.0f), 75, 90, 1);
		Plant* newPlant2 = new Plant(Vector3(rand() % 4000 + heightMap->GetHeightmapSize().x / 2.0f, 150, rand() % 4000 + heightMap->GetHeightmapSize().x / 2.0f), 75, 90, 2);
		plants.push_back(newPlant);
		plants.push_back(newPlant2);
	}
}

void Renderer::DrawPlant(Plant* p) {
	BindShader(dirLightShader);
	SetShaderLight(*dirLight);

	glUniform3fv(glGetUniformLocation(lightShader->GetProgram(),
		"lightCenter"), 1, (float*)&Vector3(heightMap->GetHeightmapSize() * Vector3(1.0f, 0.0f, 1.0f)));
	glUniform3fv(glGetUniformLocation(dirLightShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(
		dirLightShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(
		dirLightShader->GetProgram(), "bumpTex"), 1);

	modelMatrix = Matrix4::Translation(p->GetPosition()) * Matrix4::Scale(p->GetScale()) * Matrix4::Rotation(p->GetRotation(), Vector3(0, 1, 0));
	UpdateShaderMatrices();

	for (int i = 0; i < p->pMesh->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, p->tex);
		p->pMesh->DrawSubMesh(i);
	}
}

void Renderer::DrawPlants() {
	for (auto plant : plants) {
		DrawPlant(plant);
	}
}


void Renderer::ToggleScene() {
	toggleScene = !toggleScene;
}

void Renderer::ToggleAutoCamera() {
	toogleAutoCamera = !toogleAutoCamera;
}

void Renderer::ToggleGaussianBlur() {
	if (!toggleScene) {
		toggleGaussianBlur = !toggleGaussianBlur;
	}
}

void Renderer::ToggleDeffered() {
	toggleDeffered = !toggleDeffered;
}

void Renderer::ToggleShowTutorialSample() {
	toggleShowTutorialSample = !toggleShowTutorialSample;
}

GLuint Renderer::loadCubemap(vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	

	return textureID;
}

void Renderer::BuildNodeLists(SceneNode* from) {

	if (from->GetColour().w < 1.0f) {
		transparentNodeList.push_back(from);
	}
	else
	{
		nodeList.push_back(from);
	}
	

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); i++) {
		BuildNodeLists(*i);
	}
}

void Renderer::SortNodeLists() {
	std::sort(transparentNodeList.rbegin(), transparentNodeList.rend(), SceneNode::CompareByCameraDistance);

	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes() {
	for (const auto& i : nodeList) {

		DrawNode(i);
	}
	for (const auto& i : transparentNodeList) {
		DrawNode(i);
	}
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());

		glUniformMatrix4fv(glGetUniformLocation(sceneNodeShader->GetProgram(), "modelMatrix"), 1, false, model.values);
		glUniform4fv(glGetUniformLocation(sceneNodeShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());

		GLuint tex = n->GetTexture();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform1i(glGetUniformLocation(sceneNodeShader->GetProgram(), "useTexture"), tex);

		n->Draw(*this);
	}
}

void Renderer::DrawSceneNode(SceneNode* r) {
	BuildNodeLists(r);
	SortNodeLists();

	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	BindShader(sceneNodeShader);
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(sceneNodeShader->GetProgram(), "diffuseTex"), 0);

	DrawNodes();

	ClearNodeLists();
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::RotateDirLight(float dt) {
	if (dirLight->GetPosition().y <= 800 &&
		dirLight->GetPosition().y >= 0) {
		
	}

	Vector3 t = Vector3(dirLight->GetPosition().x, dirLight->GetPosition().y + dt * 10, dirLight->GetPosition().z);
	dirLight->SetPosition(t);
}

void Renderer::RenderDefferedScene() {
	FillBuffers();
	DrawPointLights();
	CombineBuffers();
}

void Renderer::GenerateScreenTexture(GLuint& into, bool depth) {
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLuint format = depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
	GLuint type = depth ? GL_DEPTH_COMPONENT : GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, type, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Renderer::FillBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBODeferred);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	BindShader(deferredShader);
	glUniform1i(glGetUniformLocation(deferredShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(deferredShader->GetProgram(), "bumpTex"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthBump);

	modelMatrix.ToIdentity();
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	UpdateShaderMatrices();

	heightMap->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawPointLights() {
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	BindShader(pointlightShader);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_ONE, GL_ONE);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);

	glUniform1i(glGetUniformLocation(pointlightShader->GetProgram(), "depthTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTexDeferred);

	glUniform1i(glGetUniformLocation(pointlightShader->GetProgram(), "normTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bufferNormalTexDeferred);

	glUniform3fv(glGetUniformLocation(pointlightShader->GetProgram(), "cameraPos"),
		1, (float*)&camera->GetPosition());

	glUniform2f(glGetUniformLocation(pointlightShader->GetProgram(), "pixelSize"),
		1.0f / width, 1.0f / height);

	Matrix4 invViewProj = (projMatrix * viewMatrix).Inverse();
	glUniformMatrix4fv(glGetUniformLocation(pointlightShader->GetProgram(), "inverseProjView"),
		1, false, invViewProj.values);

	UpdateShaderMatrices();
	for (int i = 0; i < LIGHT_NUM; ++i) {
		Light& l = pointLights[i];
		SetShaderLight(l);
		sphere->Draw();
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	glDepthMask(GL_TRUE);

	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::CombineBuffers() {
	BindShader(combineShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(combineShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferColourTexDeferred);

	glUniform1i(glGetUniformLocation(combineShader->GetProgram(), "diffuseLight"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lightDiffuseTex);

	glUniform1i(glGetUniformLocation(combineShader->GetProgram(), "specularLight"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

	quadDeferred->Draw();
}