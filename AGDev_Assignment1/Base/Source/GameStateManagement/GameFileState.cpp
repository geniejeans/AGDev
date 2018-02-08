#include <iostream>
using namespace std;

#include "GameFileState.h"
#include "GL/glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"

#include "KeyboardController.h"
#include "SceneManager.h"

CGameFileState::CGameFileState()
{
}

CGameFileState::~CGameFileState()
{
}

void CGameFileState::Init()
{
	// create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("GAMEFILESTATE_1", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GAMEFILESTATE_1")->textureID = LoadTGA("Image//GameFilesState1.tga");

	MeshBuilder::GetInstance()->GenerateQuad("GAMEFILESTATE_2", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GAMEFILESTATE_2")->textureID = LoadTGA("Image//GameFilesState2.tga");

	MeshBuilder::GetInstance()->GenerateQuad("GAMEFILESTATE_3", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GAMEFILESTATE_3")->textureID = LoadTGA("Image//GameFilesState3.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	GameFileStateBackground = Create::Sprite2DObject("GAMEFILESTATE_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0.0f));
	cout << "CGameFileState loaded\n" << endl;
	choice = 1;
}

void CGameFileState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
	{
		cout << "Loading COptionState" << endl;
		SceneManager::GetInstance()->SetActiveScene("OptionState");
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_LEFT))
	{
		if (choice == 3)
		{
			choice = 2;
			GameFileStateBackground->SetMesh("GAMEFILESTATE_2");
		}
	
		else if (choice == 2)
		{
			choice = 1;
			GameFileStateBackground->SetMesh("GAMEFILESTATE_1");
		}
		
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RIGHT))
	{
		if (choice == 1)
		{
			choice = 2;
			GameFileStateBackground->SetMesh("GAMEFILESTATE_2");
		}

		else if (choice == 2)
		{
			choice = 3;
			GameFileStateBackground->SetMesh("GAMEFILESTATE_3");
		}
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN))
	{
		switch (choice)
		{
		case 1:
			if (!Application::GetInstance().GetInGame())
			{
				CPlayerInfo::GetInstance()->SetLoadFromFile(true);
				CPlayerInfo::GetInstance()->SetPos(CLuaInterface::GetInstance()->getVector3Values("pos", "Image/GameFile1.lua"));
				CPlayerInfo::GetInstance()->SetTarget(CLuaInterface::GetInstance()->getVector3Values("tar", "Image/GameFile1.lua"));
				CPlayerInfo::GetInstance()->SetUp(CLuaInterface::GetInstance()->getVector3Values("up", "Image/GameFile1.lua"));
				CPlayerInfo::GetInstance()->SetHealth(CLuaInterface::GetInstance()->getIntValue("health", "Image/GameFile1.lua"));
				CPlayerInfo::GetInstance()->SetPoint(CLuaInterface::GetInstance()->getIntValue("points", "Image/GameFile1.lua"));
				cout << "Loading CGameState" << endl;
				SceneManager::GetInstance()->SetActiveScene("GameState");
			}
			else
			{
				CLuaInterface::GetInstance()->saveVector3Value("pos", CPlayerInfo::GetInstance()->GetPos(), "Image/GameFile1.lua", true);
				CLuaInterface::GetInstance()->saveVector3Value("tar", CPlayerInfo::GetInstance()->GetTarget(), "Image/GameFile1.lua");
				CLuaInterface::GetInstance()->saveVector3Value("up", CPlayerInfo::GetInstance()->GetUp(), "Image/GameFile1.lua");
				CLuaInterface::GetInstance()->saveIntValue("health", CPlayerInfo::GetInstance()->GetHealth(), "Image/GameFile1.lua");
				CLuaInterface::GetInstance()->saveIntValue("points", CPlayerInfo::GetInstance()->GetPoints(), "Image/GameFile1.lua");
			}
			break;
		case 2:
			if (!Application::GetInstance().GetInGame())
			{
				CPlayerInfo::GetInstance()->SetLoadFromFile(true);
				CPlayerInfo::GetInstance()->SetPos(CLuaInterface::GetInstance()->getVector3Values("pos", "Image/GameFile2.lua"));
				CPlayerInfo::GetInstance()->SetTarget(CLuaInterface::GetInstance()->getVector3Values("tar", "Image/GameFile2.lua"));
				CPlayerInfo::GetInstance()->SetUp(CLuaInterface::GetInstance()->getVector3Values("up", "Image/GameFile2.lua"));
				CPlayerInfo::GetInstance()->SetHealth(CLuaInterface::GetInstance()->getIntValue("health", "Image/GameFile2.lua"));
				CPlayerInfo::GetInstance()->SetPoint(CLuaInterface::GetInstance()->getIntValue("points", "Image/GameFile2.lua"));
				cout << "Loading CGameState" << endl;
				SceneManager::GetInstance()->SetActiveScene("GameState");
			}
			else
			{
				CLuaInterface::GetInstance()->saveVector3Value("pos", CPlayerInfo::GetInstance()->GetPos(), "Image/GameFile2.lua", true);
				CLuaInterface::GetInstance()->saveVector3Value("tar", CPlayerInfo::GetInstance()->GetTarget(), "Image/GameFile2.lua");
				CLuaInterface::GetInstance()->saveVector3Value("up", CPlayerInfo::GetInstance()->GetUp(), "Image/GameFile2.lua");
				CLuaInterface::GetInstance()->saveIntValue("health", CPlayerInfo::GetInstance()->GetHealth(), "Image/GameFile2.lua");
				CLuaInterface::GetInstance()->saveIntValue("points", CPlayerInfo::GetInstance()->GetPoints(), "Image/GameFile2.lua");
			}
			break;
		case 3:
			if (!Application::GetInstance().GetInGame())
			{
				CPlayerInfo::GetInstance()->SetLoadFromFile(true);
				CPlayerInfo::GetInstance()->SetPos(CLuaInterface::GetInstance()->getVector3Values("pos", "Image/GameFile3.lua"));
				CPlayerInfo::GetInstance()->SetTarget(CLuaInterface::GetInstance()->getVector3Values("tar", "Image/GameFile3.lua"));
				CPlayerInfo::GetInstance()->SetUp(CLuaInterface::GetInstance()->getVector3Values("up", "Image/GameFile3.lua"));
				CPlayerInfo::GetInstance()->SetHealth(CLuaInterface::GetInstance()->getIntValue("health", "Image/GameFile3.lua"));
				CPlayerInfo::GetInstance()->SetPoint(CLuaInterface::GetInstance()->getIntValue("points", "Image/GameFile3.lua"));
				cout << "Loading CGameState" << endl;
				SceneManager::GetInstance()->SetActiveScene("GameState");
			}
			else
			{
				CLuaInterface::GetInstance()->saveVector3Value("pos", CPlayerInfo::GetInstance()->GetPos(), "Image/GameFile3.lua", true);
				CLuaInterface::GetInstance()->saveVector3Value("tar", CPlayerInfo::GetInstance()->GetTarget(), "Image/GameFile3.lua");
				CLuaInterface::GetInstance()->saveVector3Value("up", CPlayerInfo::GetInstance()->GetUp(), "Image/GameFile3.lua");
				CLuaInterface::GetInstance()->saveIntValue("health", CPlayerInfo::GetInstance()->GetHealth(), "Image/GameFile3.lua");
				CLuaInterface::GetInstance()->saveIntValue("points", CPlayerInfo::GetInstance()->GetPoints(), "Image/GameFile3.lua");
			}
			break;
		};
	
	}
}

void CGameFileState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	//Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	//Render the required entities
	EntityManager::GetInstance()->Render();

	//Setup 2D  pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0,
		Application::GetInstance().GetWindowWidth(),
		0,
		Application::GetInstance().GetWindowHeight(),
		-10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	// Render the required entities 
	EntityManager::GetInstance()->RenderUI();
}

void CGameFileState::Exit()
{
	//Remove  the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(GameFileStateBackground);

	//Remove the meshes which are specific to CGameFileState
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}
