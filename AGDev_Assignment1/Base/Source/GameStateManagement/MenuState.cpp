#include <iostream>
using namespace std;

#include "MenuState.h"
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

CMenuState::CMenuState()
{
}

CMenuState::~CMenuState()
{
}

void CMenuState::Init()
{
	// create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_PLAY", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_PLAY")->textureID = LoadTGA("Image//MenuStatePlay.tga");

	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_OPTIONS", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_OPTIONS")->textureID = LoadTGA("Image//MenuStateOptions.tga");

	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_HIGHSCORE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_HIGHSCORE")->textureID = LoadTGA("Image//MenuStateHighScore.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBackground = Create::Sprite2DObject("MENUSTATE_PLAY",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f));
	cout << "CMenuState loaded\n" << endl;
	Application::GetInstance().SetInGame(false);
	choice = 1;
}

void CMenuState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_UP))
	{
		if (choice == 3)
		{
			choice = 2;
			MenuStateBackground->SetMesh("MENUSTATE_OPTIONS");
		}
	
		else if (choice == 2)
		{
			choice = 1;
			MenuStateBackground->SetMesh("MENUSTATE_PLAY");
		}
		
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_DOWN))
	{
		if (choice == 1)
		{
			choice = 2;
			MenuStateBackground->SetMesh("MENUSTATE_OPTIONS");
		}

		else if (choice == 2)
		{
			choice = 3;
			MenuStateBackground->SetMesh("MENUSTATE_HIGHSCORE");
		}
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN))
	{
		switch (choice)
		{
		case 1:
			cout << "Loading CMenuState" << endl;
			SceneManager::GetInstance()->SetActiveScene("GameState");
			break;
		case 2:
			cout << "Loading COptionState" << endl;
			SceneManager::GetInstance()->SetActiveScene("OptionState");
			break;
		case 3:
			break;
		};
	}
}

void CMenuState::Render()
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

void CMenuState::Exit()
{
	//Remove  the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);

	//Remove the meshes which are specific to CMenuState
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}
