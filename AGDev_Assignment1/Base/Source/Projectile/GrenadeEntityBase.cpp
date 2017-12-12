#include "GrenadeEntityBase.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL\glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "MyMath.h"
#include "../SpatialPartition/SpatialPartition.h"
#include "../SceneGraph/SceneGraph.h"

#include <iostream>
using namespace std;

CGrenadeEntityBase::CGrenadeEntityBase(void)
	: CProjectile(NULL)
	, m_fGravity(-10.0f)
	, m_fElapsedTime(0.0f)
	, m_pTerrain(NULL)
{
}

CGrenadeEntityBase::CGrenadeEntityBase(Mesh* _modelMesh)
	: CProjectile(_modelMesh)
	, m_fGravity(-10.0f)
	, m_fElapsedTime(0.0f)
	, m_pTerrain(NULL)
{
}

CGrenadeEntityBase::~CGrenadeEntityBase(void)
{
	m_pTerrain = NULL; // Don't delete this as the terrain is deleted in CPlayerInfo
	modelMesh = NULL;
	theSource = NULL;
}

// Update the status of this projectile
void CGrenadeEntityBase::Update(double dt)
{
	if (m_bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	m_fLifetime -= (float)dt;
	if (m_fLifetime < 0.0f)
	{
		SetStatus(false);
		SetIsDone(true);	// This method informs EntityManager to remove this instance

		// Check the SpatialPartition to destroy nearby objects
		vector<EntityBase*> ExportList = CSpatialPartition::GetInstance()->GetObjects(position, 1.0f);
		for (int i = 0; i < ExportList.size(); ++i)
		{
			if (ExportList[i]->GetMeshName() == "Player")
				EntityManager::GetInstance()->ChangePlayerHealth(-10);
			// Remove from Scene Graph
			else if (CSceneGraph::GetInstance()->DeleteNode(ExportList[i]) == true)
			{
				cout << "*** This Entity removed ***" << endl;
			}


		}
		return;
	}

	// Check if the Grenade is already on the ground
	if (position.y >= 0 - 10.0f + Math::EPSILON)
	{
		// Update Position
		m_fElapsedTime += dt;

		position.Set(	position.x + (float)(theDirection.x * m_fElapsedTime * m_fSpeed),
						position.y + (float)(theDirection.y * m_fElapsedTime * m_fSpeed) + (0.5 * m_fGravity * m_fElapsedTime * m_fElapsedTime),
						position.z + (float)(theDirection.z * m_fElapsedTime * m_fSpeed));

		if (position.y < 0 - 10.0f)
		{
			position.y = 0 - 10.0f;
			m_fSpeed = 0.0f;
			return;
		}
	}
}

// Set the terrain for the player info
void CGrenadeEntityBase::SetTerrain(GroundEntity* m_pTerrain)
{
	this->m_pTerrain = m_pTerrain;
}

// Create a projectile and add it into EntityManager
CGrenadeEntityBase* Create::GrenadeEntityBase(	const std::string& _meshName,
							const Vector3& _position, 
							const Vector3& _direction, 
							const float m_fLifetime, 
							const float m_fSpeed,
							EntityBase* _source)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CGrenadeEntityBase* result = new CGrenadeEntityBase(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	//result->SetSource(_source);
	//result->SetTerrain(_source->GetTerrain());
	EntityManager::GetInstance()->AddEntity(result, "", true);
	return result;
}