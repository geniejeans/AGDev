#include <iostream>
using namespace std;

#include "HighscoreState.h"
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

CHighScoreState::CHighScoreState()
{

}

CHighScoreState::~CHighScoreState()
{

}

void CHighScoreState::Init()
{
	// create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("HIGHSCORESTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("HIGHSCORESTATE_BKGROUND")->textureID = LoadTGA("Image//HighscoreState.tga");

	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(0, 1, 0);

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 40.0f;
	float halfFontSize = fontSize / 2.0f;
	IntroStateBackground = Create::Sprite2DObject("HIGHSCORESTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0.0f));
	cout << "CIntroState loaded\n" << endl;

	for (int i = 0; i < 5; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(halfWindowWidth / 2, halfWindowHeight + fontSize*i + halfFontSize, 2.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}

	score1 = CLuaInterface::GetInstance()->getIntValue("points", "Image/GameFile1.lua");
	score2 = CLuaInterface::GetInstance()->getIntValue("points", "Image/GameFile2.lua");
	score3 = CLuaInterface::GetInstance()->getIntValue("points", "Image/GameFile3.lua");

	if (score1 < score2)
	{
		highscore = score2;
	}

	else
	{
		highscore = score1;
	}

	if (highscore < score3)
	{
		highscore = score3;
	}

	cout << score1 << endl;
	cout << score2 << endl;
	cout << score3 << endl;
	cout << highscore << endl;
	std::ostringstream ss1;
	ss1 << "HIGHSCORE:" << highscore;
	textObj[1]->SetText(ss1.str());
	textObj[1]->SetColor(Color(0, 1, 0));

}

void CHighScoreState::Update(double dt)
{


	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
	{
		cout << "Loading HighscoreState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
}

void CHighScoreState::Render()
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

void CHighScoreState::Exit()
{
	// Remove teh entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(IntroStateBackground);
	for (int i = 0; i < 5; ++i)
		EntityManager::GetInstance()->RemoveEntity(textObj[i]);

	// Remove the meshes which are specific to CIntroState
	MeshBuilder::GetInstance()->RemoveMesh("HIGHSCORESTATE_BKGROUND");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();

}