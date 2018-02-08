#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
class TextEntity;

class CVideoState : public Scene
{
public:
	CVideoState();
	~CVideoState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* MenuStateBackground;
	int choice;
	bool changekeys;
	TextEntity* textObj[5];
};