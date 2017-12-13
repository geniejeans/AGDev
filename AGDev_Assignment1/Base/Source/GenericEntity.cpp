#include "GenericEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

GenericEntity::GenericEntity(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Update(double _dt)
{

	// Does nothing here, can inherit & override or create your own version of this class :D
	if (theUpdateTransformation)
	{
		Vector3 previousMax = maxAABB;
		Vector3 previousMin = minAABB;
		Mtx44 matrix;
		float rotation = 0.0f;
		ApplyTransform(GetUpdateTransform());
		matrix = GetUpdateTransform();
		rotation = acos(matrix.a[0]);
		maxAABB.x = previousMax.x * cos(rotation) + previousMax.z * sin(rotation) ;
		maxAABB.z = -previousMax.x * sin(rotation) + previousMax.z * cos(rotation) ;
	
		minAABB.x = previousMin.x * cos(rotation) + previousMin.z * sin(rotation);
		minAABB.z = -previousMin.x * sin(rotation) + previousMin.z * cos(rotation);
	}
}

void GenericEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	modelStack.MultMatrix(this->GetTransform());
	if (GetLODStatus()==true)
	{
		if (theDetailLevel != NO_DETAILS)
			RenderHelper::RenderMesh(GetLODMesh());
	}
	else
		RenderHelper::RenderMesh(modelMesh);
	
	modelStack.PopMatrix();

}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

GenericEntity* Create::Entity(	const std::string& _meshName, 
								const Vector3& _position,
								const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	EntityManager::GetInstance()->AddEntity(result, "", true);
	return result;
}

GenericEntity* Create::TreeEntity(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	EntityManager::GetInstance()->AddEntity(result,"tree", true);
	return result;
}

GenericEntity* Create::Asset(	const std::string& _meshName,
								const Vector3& _position,
								const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	return result;
}

