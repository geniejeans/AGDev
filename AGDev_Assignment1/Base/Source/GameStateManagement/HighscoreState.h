#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
class TextEntity;
//class Text ENtity
class CHighScoreState :public Scene
{
public:
	CHighScoreState();
	~CHighScoreState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();



private:
	FPSCamera camera;
	SpriteEntity* IntroStateBackground;
	// TextEntity* textObj[3];

	int score1 = 0;
	int score2 = 0;
	int score3 = 0;
	int highscore = 0;
	TextEntity* textObj[5];
};