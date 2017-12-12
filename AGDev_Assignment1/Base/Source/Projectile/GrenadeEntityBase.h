#pragma once
#include "Projectile.h"
#include "EntityBase.h"
#include "Vector3.h"
#include "Collider/Collider.h"
#include "../GroundEntity.h"

class Mesh;

class CGrenadeEntityBase : public CProjectile
{
protected:
	float m_fGravity;
	float m_fElapsedTime;
	GroundEntity* m_pTerrain;

public:
	CGrenadeEntityBase(void);
	CGrenadeEntityBase(Mesh* _modelMesh);
	~CGrenadeEntityBase(void);

	// Update the status of this projectile
	void Update(double dt = 0.0333f);

	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);
};

namespace Create
{
	CGrenadeEntityBase* GrenadeEntityBase(	const std::string& _meshName, 
						const Vector3& _position, 
						const Vector3& _direction, 
						const float m_fLifetime, 
						const float m_fSpeed,
						EntityBase* _source = NULL);
};

