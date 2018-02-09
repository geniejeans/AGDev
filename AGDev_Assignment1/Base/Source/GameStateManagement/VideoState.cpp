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
#include <sstream>
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

	MeshBuilder::GetInstance()->GenerateQuad("VIDEOSTATE_4", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("VIDEOSTATE_4")->textureID = LoadTGA("Image//video4.tga");

	MeshBuilder::GetInstance()->GenerateQuad("VIDEOSTATE_5", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("VIDEOSTATE_5")->textureID = LoadTGA("Image//video5.tga");

	MeshBuilder::GetInstance()->GenerateQuad("VIDEOSTATE_6", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("VIDEOSTATE_6")->textureID = LoadTGA("Image//video6.tga");

	MeshBuilder::GetInstance()->GenerateQuad("VIDEOSTATE_7", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("VIDEOSTATE_7")->textureID = LoadTGA("Image//video7.tga");

	MeshBuilder::GetInstance()->GenerateQuad("VIDEOSTATE_8", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("VIDEOSTATE_8")->textureID = LoadTGA("Image//video8.tga");

	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(0, 1, 0);

	float fontSize = 40.0f;
	float halfFontSize = fontSize / 2.0f;
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBackground = Create::Sprite2DObject("VIDEOSTATE_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0.0f));
	cout << "CVideoState loaded\n" << endl;
	Application::GetInstance().SetInGame(false);
	choice = 1;
	changekeys = false;

	for (int i = 0; i < 5; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(halfWindowWidth / 2 - fontSize*i - i * 3.2 * fontSize + halfWindowWidth * 1.1f, halfWindowHeight / 2 + halfWindowHeight / 4, 2.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	}

}

void CVideoState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_LEFT))
	{
		switch (choice)
		{
		case 1:
			break;
		case 2:
			choice = 1;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_1");
			break;
		case 3:
			choice = 2;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_2");
			break;
		case 4:
			choice = 3;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_3");
			break;
		case 5:
			choice = 4;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_4");
			break;
		case 6:
			choice = 5;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_5");
			break;
		case 7:
			choice = 6;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_6");
			break;
		case 8:
			choice = 7;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_7");
			break;
		};
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RIGHT))
	{
		switch (choice)
		{
		case 1:
			choice = 2;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_2");
			break;
		case 2:
			choice = 3;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_3");
			break;
		case 3:
			choice = 4;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_4");
			break;
		case 4:
			choice = 5;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_5");
			break;
		case 5:
			choice = 6;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_6");
			break;
		case 6:
			choice = 7;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_7");
			break;
		case 7:
			choice = 8;
			changekeys = false;
			MenuStateBackground->SetMesh("VIDEOSTATE_8");
			break;
		case 8:
			break;
		};
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
			CLuaInterface::GetInstance()->updateResolution("width", 800, 89);
			CLuaInterface::GetInstance()->updateResolution("height", 600, 90);
			Application::GetInstance().ChangeScene();
			break;
		case 2:
			CLuaInterface::GetInstance()->updateResolution("width", 1024, 89);
			CLuaInterface::GetInstance()->updateResolution("height", 768, 90);
			Application::GetInstance().ChangeScene();
			break;
		case 3:
			CLuaInterface::GetInstance()->updateResolution("width", 1600, 89);
			CLuaInterface::GetInstance()->updateResolution("height", 1200, 90);
			Application::GetInstance().ChangeScene();
			break;
		case 4:
			if (changekeys)
			changekeys = false;
			else
			changekeys = true;
			break;
		case 5:
			if (changekeys)
				changekeys = false;
			else
				changekeys = true;
			break;
		case 6:
			if (changekeys)
				changekeys = false;
			else
				changekeys = true;
			break;
		case 7:
			if (changekeys)
				changekeys = false;
			else
				changekeys = true;
			break;
		case 8:
			if (changekeys)
				changekeys = false;
			else
				changekeys = true;
			break;
		};
	}

	if (changekeys)
	{
		for (int i = 'A'; i <= 'Z'; i++)
		{
			if (KeyboardController::GetInstance()->IsKeyReleased(i))
			{
				switch (choice)
				{
				case 4:
					CLuaInterface::GetInstance()->replaceForward("moveForward", i, 93);
					break;
				case 5:
					CLuaInterface::GetInstance()->replaceForward("moveBackward", i, 94);
					break;
				case 6:
					CLuaInterface::GetInstance()->replaceForward("moveLeft", i, 95);
					break;
				case 7:
					CLuaInterface::GetInstance()->replaceForward("moveRight", i, 96);
					break;
				case 8:
					CLuaInterface::GetInstance()->replaceForward("moveForward", i, 93);
					break;
				};
			}
		}
	}

	std::ostringstream ss1;
	//if (changekeys)
	ss1 << CLuaInterface::GetInstance()->getCharValue("moveRight");
	textObj[1]->SetText(ss1.str());

	std::ostringstream ss2;
	ss2.precision(3);
	ss2 << CLuaInterface::GetInstance()->getCharValue("moveLeft");
	textObj[2]->SetText(ss2.str());

	ss2.str("");
	ss2.precision(3);
	ss2 << CLuaInterface::GetInstance()->getCharValue("moveBackward");
	textObj[3]->SetText(ss2.str());

	std::ostringstream ss3;
	ss3.precision(3);
	ss3 << CLuaInterface::GetInstance()->getCharValue("moveForward");
	textObj[4]->SetText(ss3.str());

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
	for (int i = 0; i < 5; ++i)
		EntityManager::GetInstance()->RemoveEntity(textObj[i]);


	//Remove the meshes which are specific to CVideoState
	MeshBuilder::GetInstance()->RemoveMesh("VIDEOSTATE_1");
	MeshBuilder::GetInstance()->RemoveMesh("VIDEOSTATE_2");
	MeshBuilder::GetInstance()->RemoveMesh("VIDEOSTATE_3");

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}
