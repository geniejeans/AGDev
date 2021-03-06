#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
#include "Collider/Collider.h"
#include "LevelOfDetails\LevelOfDetails.h"
#include "SceneGraph\Transform.h"

class Mesh;

class GenericEntity : public EntityBase, public CCollider, public CLevelOfDetails, public CTransform
{
public:
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

	virtual void Update(double _dt);
	virtual void Render();

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);
	Vector3 GetMaxAABB(){ return maxAABB; };
	Vector3 GetMinAABB(){ return minAABB; };
	void SetAngleOfRotation(float angle) { angleOfRotation = angle; };
private:
	Mesh* modelMesh;
	float angleOfRotation = 0.0f;
	float angle = 0.0f;
};

namespace Create
{
	GenericEntity* Entity(	const std::string& _meshName, 
							const Vector3& _position, 
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
	GenericEntity* TreeEntity(  const std::string& _meshName,
								const Vector3& _position,
								const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
	GenericEntity* Asset(	const std::string& _meshName,
							const Vector3& _position,
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif // GENERIC_ENTITY_H