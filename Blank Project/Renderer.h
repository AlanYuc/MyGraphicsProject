# pragma once
#include "..\nclgl\OGLRenderer.h"
#include "..\nclgl\Camera.h"
#include "..\nclgl\HeightMap.h"
#include "..\nclgl\Light.h"
#include "..\nclgl\stb_image.h"
#include "..\nclgl\MeshAnimation.h"
#include "..\nclgl\MeshMaterial.h"
#include "..\nclgl\SceneNode.h"
#include "..\nclgl\Frustum.h"
#include "..\nclgl\CubeRobot.h"
#include "Planet.h"
#include "Rain.h"
#include "Warplane.h"
#include "Plant.h"
#include <algorithm>


class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);
	void RenderScene() override;
	void UpdateScene(float dt) override;

	GLuint loadCubemap(vector<std::string> faces);

	void ToggleScene();
	void ToggleAutoCamera();
	void ToggleGaussianBlur();
	void ToggleDeffered();
	void ToggleShowTutorialSample();
	void RotateDirLight(float dt);

protected:
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	void DrawShadowScene();
	void DrawMainScene();
	void DrawSoldier();
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void DrawSceneNode(SceneNode* r);
	void PresentScene();
	void DrawPostProcess();
	void DrawScene();
	void InitPorcess();
	void FillBuffers();
	void DrawPointLights();
	void CombineBuffers();
	void RenderDefferedScene();
	void GenerateScreenTexture(GLuint& into, bool depth = false);
	void DrawRain();

	Shader* lightShader;
	Shader* dirLightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* sceneShader;
	Shader* shadowShader;
	Shader* SkinningShader;
	Shader* sceneNodeShader;
	Shader* processSceneShader;
	Shader* processShader;
	Shader* planet;
	Shader* deferredShader;
	Shader* pointlightShader;
	Shader* combineShader;

	HeightMap* heightMap;

	Mesh* quad;
	Mesh* waterQuad;
	Mesh* mesh;
	Mesh* quadGlass;
	Mesh* cubeRobot;
	Mesh* fireCube;
	Mesh* screenQuad;
	Mesh* sunMesh;
	Mesh* sphere;
	Mesh* quadDeferred;

	Light* light;
	Light* dirLight;
	Light* light2;
	Light* pointLights;

	Camera* camera;

	GLuint cubeMap;
	GLuint cubeMapSpace;
	GLuint waterTex;
	GLuint earthTex;
	GLuint earthBump;
	GLuint shadowTex;
	GLuint shadowFBO;
	GLuint sceneDiffuse;
	GLuint sceneBump;
	GLuint spaceSparrowTex;
	GLuint heightTexture;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;
	GLuint sceneTexture;
	GLuint bufferFBODeferred;
	GLuint bufferColourTexDeferred;
	GLuint bufferNormalTexDeferred;
	GLuint bufferDepthTexDeferred;
	GLuint pointLightFBO;
	GLuint lightDiffuseTex;
	GLuint lightSpecularTex;
	GLuint sunTex;
	GLuint earthDayTex;
	GLuint marsTex;
	GLuint moonTex;
	GLuint rainTex;

	int selectTex;
	int currentFrame;
	float frameTime;
	float waterRotate;
	float waterCycle;
	float sceneTime;

	vector < Mesh*> sceneMeshes;
	vector < Matrix4 > sceneTransforms;
	vector <Mesh*> spaceShip;
	vector <GLuint> matTextures;
	vector <SceneNode*> transparentNodeList;
	vector <SceneNode*> nodeList;
	
	MeshAnimation* anim;
	MeshMaterial* material;
	
	bool toggleScene;
	bool toogleAutoCamera;
	bool toggleGaussianBlur;
	bool toggleDeffered;
	bool toggleShowTutorialSample;

	SceneNode* root;
	SceneNode* root2;
	SceneNode* rainRoot;
	Rain* rain;

	Vector3 sunPos;
	Matrix4 sunTransform;
	
	//Frustum frameFrustum;


	vector <Plant*> plants;
	Plant* plant;
	void AddPlant(); 
	void DrawPlant(Plant* p);
	void DrawPlants();
};