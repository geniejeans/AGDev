#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
//class Text ENtity
class CInventoryState :public Scene
{
public:
	CInventoryState();
	~CInventoryState();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* IntroStateBackground;
	// TextEntity* textObj[3];
};