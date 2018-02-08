#include <iostream>
using namespace std;

#include "VideoState.h"
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

CVideoState::CVideoState()
{
}

CVideoState::~CVideoState()
{
}

void CVideoState::Init()
{
	// create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("VIDEOSTATE_1", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("VIDEOSTATE_1")->textureID = LoadTGA("Image//video1.tga");

	MeshBuilder::GetInstance()->GenerateQuad("VIDEOSTATE_2", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("VIDEOSTATE_2")->textureID = LoadTGA("Image//video2.tga");

	MeshBuilder::GetInstance()->GenerateQuad("VIDEOSTATE_3", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("VIDEOSTATE_3")->textureID = LoadTGA("Image//video3.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBackground = Create::Sprite2DObject("VIDEOSTATE_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0.0f));
	cout << "CVideoState loaded\n" << endl;
	Application::GetInstance().SetInGame(false);
	choice = 1;
}

void CVideoState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_LEFT))
	{
		if (choice == 3)
		{
			choice = 2;
			MenuStateBackground->SetMesh("VIDEOSTATE_2");
		}
	
		else if (choice == 2)
		{
			choice = 1;
			MenuStateBackground->SetMesh("VIDEOSTATE_1");
		}
		
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RIGHT))
	{
		if (choice == 1)
		{
			choice = 2;
			MenuStateBackground->SetMesh("VIDEOSTATE_2");
		}

		else if (choice == 2)
		{
			choice = 3;
			MenuStateBackground->SetMesh("VIDEOSTATE_3");
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
	{
		cout << "Loading COptionState" << endl;
		SceneManager::GetInstance()->SetActiveScene("OptionState");
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN))
	{
		switch (choice)
		{
		case 1:
			CLuaInterface::GetInstance()->saveIntValue("width", 800, "Image/Settings.lua", true);
			CLuaInterface::GetInstance()->saveIntValue("height", 600, "Image/Settings.lua");
			break;
		case 2:
			CLuaInterface::GetInstance()->saveIntValue("width", 1024, "Image/Settings.lua", true);
			CLuaInterface::GetInstance()->saveIntValue("height", 768, "Image/Settings.lua");
			break;
		case 3:
			CLuaInterface::GetInstance()->saveIntValue("width", 1600, "Image/Settings.lua", true);
			CLuaInterface::GetInstance()->saveIntValue("height", 1200, "Image/Settings.lua");
			break;
		};
	}
}

void CVideoState::Render()
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

void CVideoState::Exit()
{
	//Remove  the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);

	//Remove the meshes which are specific to CVideoState
	MeshBuilder::GetInstance()->RemoveMesh("VIDEOSTATE_1");
	MeshBuilder::GetInstance()->RemoveMesh("VIDEOSTATE_2");
	MeshBuilder::GetInstance()->RemoveMesh("VIDEOSTATE_3");

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}
