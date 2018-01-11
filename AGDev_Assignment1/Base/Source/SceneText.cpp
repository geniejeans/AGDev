#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"
#include "PlayerEntityBase\PlayerEntityBase.h"
#include "Waypoint\WaypointManager.h"

#include <iostream>
using namespace std;

//SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

//SceneText::SceneText(SceneManager* _sceneMgr)
//{
//	_sceneMgr->AddScene("Start", this);
//}

SceneText::~SceneText()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	//currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	//
	//// Tell the shader program to store these uniform locations
	//currProg->AddUniform("MVP");
	//currProg->AddUniform("MV");
	//currProg->AddUniform("MV_inverse_transpose");
	//currProg->AddUniform("material.kAmbient");
	//currProg->AddUniform("material.kDiffuse");
	//currProg->AddUniform("material.kSpecular");
	//currProg->AddUniform("material.kShininess");
	//currProg->AddUniform("lightEnabled");
	//currProg->AddUniform("numLights");
	//currProg->AddUniform("lights[0].type");
	//currProg->AddUniform("lights[0].position_cameraspace");
	//currProg->AddUniform("lights[0].color");
	//currProg->AddUniform("lights[0].power");
	//currProg->AddUniform("lights[0].kC");
	//currProg->AddUniform("lights[0].kL");
	//currProg->AddUniform("lights[0].kQ");
	//currProg->AddUniform("lights[0].spotDirection");
	//currProg->AddUniform("lights[0].cosCutoff");
	//currProg->AddUniform("lights[0].cosInner");
	//currProg->AddUniform("lights[0].exponent");
	//currProg->AddUniform("lights[1].type");
	//currProg->AddUniform("lights[1].position_cameraspace");
	//currProg->AddUniform("lights[1].color");
	//currProg->AddUniform("lights[1].power");
	//currProg->AddUniform("lights[1].kC");
	//currProg->AddUniform("lights[1].kL");
	//currProg->AddUniform("lights[1].kQ");
	//currProg->AddUniform("lights[1].spotDirection");
	//currProg->AddUniform("lights[1].cosCutoff");
	//currProg->AddUniform("lights[1].cosInner");
	//currProg->AddUniform("lights[1].exponent");
	//currProg->AddUniform("colorTextureEnabled");
	//currProg->AddUniform("colorTexture");
	//currProg->AddUniform("textEnabled");
	//currProg->AddUniform("textColor");
	//
	//// Tell the graphics manager to use the shader we just loaded
	//GraphicsManager::GetInstance()->SetActiveShader("default");

	//currProg->UpdateInt("numLights", 1);
	//currProg->UpdateInt("textEnabled", 0);

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";


	
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1.0f, 0.078f, 0.576f), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);

	//Tree
	MeshBuilder::GetInstance()->GenerateOBJ("TreeHigh", "OBJ//Tree_High.obj");
	MeshBuilder::GetInstance()->GetMesh("TreeHigh")->textureID = LoadTGA("Image//tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("TreeMid", "OBJ//Tree_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("TreeMid")->textureID = LoadTGA("Image//tree_mid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("TreeLow", "OBJ//Tree_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("TreeLow")->textureID = LoadTGA("Image//tree_low.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("TreeMid2", "OBJ//Tree_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("TreeMid2")->textureID = LoadTGA("Image//tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("TreeLow2", "OBJ//Tree_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("TreeLow2")->textureID = LoadTGA("Image//tree.tga");

	//Barrel
	MeshBuilder::GetInstance()->GenerateOBJ("BarrelHigh", "OBJ//Barrel_High.obj");
	MeshBuilder::GetInstance()->GetMesh("BarrelHigh")->textureID = LoadTGA("Image//barrel.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BarrelMid", "OBJ//Barrel_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("BarrelMid")->textureID = LoadTGA("Image//barrel.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BarrelLow", "OBJ//Barrel_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("BarrelLow")->textureID = LoadTGA("Image//barrel.tga");

	//flag
	MeshBuilder::GetInstance()->GenerateOBJ("flag", "OBJ//flag.obj");
	MeshBuilder::GetInstance()->GetMesh("flag")->textureID = LoadTGA("Image//flag.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("flagmid", "OBJ//Flag_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("flagmid")->textureID = LoadTGA("Image//flag.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("flaglow", "OBJ//Flag_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("flaglow")->textureID = LoadTGA("Image//flag.tga");

	//Dummy
	MeshBuilder::GetInstance()->GenerateOBJ("dummyhead", "OBJ//dummyhead.obj");
	MeshBuilder::GetInstance()->GetMesh("dummyhead")->textureID = LoadTGA("Image//dummyhead.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("dummyheadmid", "OBJ//Dummyhead_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("dummyheadmid")->textureID = LoadTGA("Image//dummyhead.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("dummyheadlow", "OBJ//Dummyhead_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("dummyheadlow")->textureID = LoadTGA("Image//dummyhead.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("dummyarm", "OBJ//dummyarm.obj");
	MeshBuilder::GetInstance()->GetMesh("dummyarm")->textureID = LoadTGA("Image//dummy.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("dummyarmmid", "OBJ//Dummyarm_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("dummyarmmid")->textureID = LoadTGA("Image//dummy.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("dummyarmlow", "OBJ//Dummyarm_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("dummyarmlow")->textureID = LoadTGA("Image//dummy.tga");
	MeshBuilder::GetInstance()->GenerateCube("dummybody", Color(0.871f, 0.722f, 0.529f), 1.0f);

	//Tank
	MeshBuilder::GetInstance()->GenerateOBJ("tankbottom", "OBJ//tankbottom.obj");
	MeshBuilder::GetInstance()->GetMesh("tankbottom")->textureID = LoadTGA("Image//tank.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("tankbottommid", "OBJ//Tankbottom_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("tankbottommid")->textureID = LoadTGA("Image//tank.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("tankbottomlow", "OBJ//Tankbottom_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("tankbottomlow")->textureID = LoadTGA("Image//tank.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("tanktop", "OBJ//tanktop.obj");
	MeshBuilder::GetInstance()->GetMesh("tanktop")->textureID = LoadTGA("Image//tanktop.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("tanktopmid", "OBJ//Tanktop_Mid.obj");
	MeshBuilder::GetInstance()->GetMesh("tanktopmid")->textureID = LoadTGA("Image//tanktop.tga");


	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(0, 1, 0), 1.f);
	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(60000.0f, 160000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball
	{
		//	Dummy
		GenericEntity* dummybody = Create::Entity("dummybody", Vector3(-20.0f, -3.0f, -20.0f));
		dummybody->SetCollider(true);
		dummybody->SetScale(Vector3(3.0f, 5.0f, 3.0f));
		dummybody->SetAABB(Vector3(1.5f, 2.5f, 1.5f), Vector3(-1.5f, -2.5f, -1.5f));
		dummybody->SetMeshName("Dummy");
		CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(dummybody);
		if (theNode == NULL)
		{
			cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
		}

		GenericEntity* dummyhead = Create::Entity("dummyhead", Vector3(-20.0f, 1.1f, -20.0f));
		dummyhead->SetCollider(true);
		dummyhead->SetScale(Vector3(3.0f, 3.0f, 3.0f));
		dummyhead->SetAABB(Vector3(1.5f, 1.5f, 1.5f), Vector3(-1.5f, -1.5f, -1.5f));
		dummyhead->SetMeshName("Dummy");
		dummyhead->InitLOD("dummyhead", "dummyheadmid", "dummyheadlow");
		CSceneNode* anotherNode = theNode->AddChild(dummyhead);
		if (anotherNode == NULL)
		{
			cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
		}

		GenericEntity* dummyarm = Create::Entity("dummyarm", Vector3(-20.0f, -1.5f, -20.0f));
		dummyarm->SetScale(Vector3(2.0f, 2.0f, 2.0f));
		dummyarm->SetCollider(true);
		dummyarm->SetAABB(Vector3(8.0f, 1.0f, 1.0f), Vector3(-8.0f, -1.0f, -1.0f));
		dummyarm->SetMeshName("Dummy");
		CSceneNode* thenode3 = theNode->AddChild(dummyarm);
		dummyarm->InitLOD("dummyarm", "distancemid", "distancelow");
		CUpdateTransformation* rotateArms = new CUpdateTransformation();
		rotateArms->ApplyUpdate(1.0f, 0.0f, 1.0f, 0.0f);
		rotateArms->SetSteps(0, 3600);
		thenode3->SetUpdateTransformation(rotateArms);

		if (thenode3 == NULL)
		{
			cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
		}
	}

	
	
	// Windmail
	//GenericEntity* baseCube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);

	//CUpdateTransformation* baseMtx = new CUpdateTransformation();
	//baseMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	//baseMtx->SetSteps(-60, 60);
	//baseNode->SetUpdateTransformation(baseMtx);

	//GenericEntity* childCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* childNode = baseNode->AddChild(childCube);
	//childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);

	//GenericEntity* grandchildCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	//grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);
	//CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
	//aRotateMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	//aRotateMtx->SetSteps(-120, 60);
	//grandchildNode->SetUpdateTransformation(aRotateMtx);
	
	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);

	// Create a CEnemy instance
	srand(time(NULL));
	//for (int i = 0; i < 10; i++)
	//{
	//	theEnemy = new CEnemy();
	//	float x = 1.0f + (i * rand() % 1000 - 500.0f);
	//	float y = 1.0f + (i * rand() % 1000 - 500.0f);
	//	theEnemy->SetRandomSeed(rand());
	//	theEnemy->Init(x, y);
	//	theEnemy->SetTerrain(groundEntity);
	//	theEnemy->SetTarget(theEnemy->GenerateTarget());
	//	theEnemy = NULL;
	//}
	//GenericEntity* cube = Create::Entity("cube", Vector3(0.0f, 0.0f, 0.0f));
	//cube->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	//cube->SetCollider(true);
	//cube->SetAABB(Vector3(1.5f, 1.5f, 1.5f), Vector3(-1.5f, -1.5f, -1.5f));

	GenericEntity* cube = Create::Entity("dummyhead", Vector3(-20.0f, 3.0f, -30.0f));
	cube->SetCollider(true);
//	cube->SetScale(Vector3(3.0f, 3.0f, 3.0f));
	cube->SetAABB(Vector3(1.5f, 1.5f, 1.5f), Vector3(-1.5f, -1.5f, -1.5f));
	//tank
	GenericEntity* tankbottom = Create::Entity("tankbottom", Vector3(55.0f, 0.0f, 55.0f));
	tankbottom->SetMeshName("Enemy");
	tankbottom->SetScale(Vector3(22.0f, 20.0f, 22.0f));
	tankbottom->SetCollider(true);
	tankbottom->SetAABB(Vector3(24.0f, 16.0f, 24.0f), Vector3(-24.0f, -16.0f, -24.0f));
	tankbottom->InitLOD("tankbottom", "tankbottommid", "tankbottomlow");
	//making the tank move

	// Add the pointer to this new entity to the Scene Graph
	CSceneNode* tankbottomenode = CSceneGraph::GetInstance()->AddNode(tankbottom);
	CUpdateTransformation* moveTank = new CUpdateTransformation();
//	moveTank->ApplyUpdate(0, 0, 0.9);
//	moveTank->SetSteps(-320, 320);
//	tankbottomenode->SetUpdateTransformation(moveTank);
	if (tankbottomenode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}

	//Create a Waypoint inside WaypointManager
	int aWayPoint = CWaypointManager::GetInstance()->AddWaypoint(Vector3(10.f, 0.0f, 50.0f));
	int anotherWaypoint = CWaypointManager::GetInstance()->AddWaypoint(aWayPoint, Vector3(10.0f, 0.0f, -50.0f));
	CWaypointManager::GetInstance()->AddWaypoint(anotherWaypoint, Vector3(-10.0f, 0.0f, 0.0f));
	CWaypointManager::GetInstance()->PrintSelf();

	// top of the tank
	theEnemy = new CEnemy();
	theEnemy->SetRandomSeed(rand());
	theEnemy->SetMeshName("Enemy");
	theEnemy->SetScale(Vector3(20.0f, 15.0f, 20.0f));
	theEnemy->SetAABB(Vector3(10.0f, 6.5f, 10.0f), Vector3(-10.0f, -6.5f, -10.0f));
	theEnemy->Init(55.0f, 12.0f, 55.0f);
	theEnemy->SetTerrain(groundEntity);
//	CSceneNode* tanktopnode = tankbottomenode->AddChild(theEnemy);
//	if (tanktopnode == NULL)
//	{
//		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
//	}

	CPlayerEntityBase* player = new CPlayerEntityBase();
	player->SetMeshName("Player");
	player->SetScale(Vector3(4.0f, 5.0f, 4.0f));
	player->Init(10, 10);
	CSceneGraph::GetInstance()->AddNode(player);

	// Create 100 trees
	for (int i = 0; i < 100; i++)
	{
		float x = 1.0f + (i * rand() % 1000 - 500.0f);
		float y = 1.0f + (i * rand() % 1000 - 500.0f);
		GenericEntity* Tree = Create::TreeEntity("TreeHigh", Vector3(x, -10.0f, y));
		Tree->SetScale(Vector3(10.0f, 10.0f, 10.0f));
		Tree->SetCollider(true);
		Tree->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
		Tree->InitLOD("TreeHigh", "TreeMid", "TreeLow");
	}

	GenericEntity* barrel = Create::Entity("BarrelHigh", Vector3(95, -4.0f, 55));
	barrel->SetScale(Vector3(4.0f, 5.0f, 4.0f));
	barrel->SetCollider(true);
	barrel->SetMeshName("Barrel");
	barrel->SetAABB(Vector3(3.0f, 6.0f, 3.0f), Vector3(-4.0f, -6.0f, -3.0f));
	barrel->InitLOD("BarrelHigh", "BarrelMid", "BarrelLow");

	CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(barrel);

	// Create 20 barrels
	for (int i = 1; i <= 20; i++)
	{
		float x = 1.0f + (i * rand() % 1000 - 500.0f);
		float y = 1.0f + (i * rand() % 1000 - 500.0f);
		GenericEntity* barrel = Create::Entity("BarrelHigh", Vector3(x, -4.0f, y));
		barrel->SetScale(Vector3(4.0f, 5.0f, 4.0f));
		barrel->SetCollider(true);
		barrel->SetAABB(Vector3(3.0f, 6.0f, 3.0f), Vector3(-4.0f, -6.0f, -3.0f));
		barrel->SetMeshName("Barrel");
		barrel->InitLOD("BarrelHigh", "BarrelMid", "BarrelLow");

		CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(barrel);
	}

	GenericEntity* Flag = Create::TreeEntity("flag", Vector3(0, 6.0f, 0));
	Flag->SetMeshName("Flag");
	Flag->SetScale(Vector3(15.0f, 15.0f, 15.0f));
	Flag->SetCollider(true);
	Flag->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	Flag->InitLOD("flag", "flag", "flag");

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 5; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");
}

void SceneText::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
		Vector3 pos = theNode->GetEntity()->GetPosition();
		theNode->GetEntity()->SetPosition(Vector3(pos.x + 50.0f, pos.y, pos.z + 50.0f));
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	//if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	//{
	//	cout << "Middle Mouse Button was released!" << endl;
	//}
	//if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	//{
	//	cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	//}
	//if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	//{
	//	cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	//}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss1.str());

	std::ostringstream ss2;
	ss2.precision(3);
	ss2 << "Points:" << playerInfo->GetPoints();
	textObj[3]->SetText(ss2.str());

	ss2.str("");
	ss2.precision(3);
	ss2 << "Health:" << playerInfo->GetHealth();
	textObj[4]->SetText(ss2.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}
