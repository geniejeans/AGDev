#include "ProjectileEntityBase.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CProjectileEntityBase::CProjectileEntityBase(void)
	: modelMesh(NULL)
	, m_bStatus(false)
	, theDirection(0, 0, 0)
	, m_fLifetime(-1.0f)
	, m_fSpeed(10.0f)
	, theSource(NULL)
{
}

CProjectileEntityBase::CProjectileEntityBase(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
	, m_bStatus(false)
	, theDirection(0, 0, 0)
	, m_fLifetime(-1)
	, m_fSpeed(10.0f)
	, theSource(NULL)
{
}

CProjectileEntityBase::~CProjectileEntityBase(void)
{
	modelMesh = NULL;
	theSource = NULL;
}

// Activate the projectile. true == active, false == inactive
void CProjectileEntityBase::SetStatus(const bool m_bStatus)
{
	if (m_bStatus == false)
		m_fLifetime = -1;
	this->m_bStatus = m_bStatus;
}

// get status of the projectile. true == active, false == inactive
bool CProjectileEntityBase::GetStatus(void) const
{
	return m_bStatus;
}

// Set the position and direction of the projectile
void CProjectileEntityBase::Set(Vector3 theNewPosition, Vector3 theNewDirection, const float m_fLifetime, const float m_fSpeed)
{
	position = theNewPosition;
	theDirection = theNewDirection;
	this->m_fLifetime = m_fLifetime;
	this->m_fSpeed = m_fSpeed;
}

// Get the direction of the projectile
Vector3 CProjectileEntityBase::GetDirection(void)
{
	return theDirection;
}

// Get the lifetime of the projectile
float CProjectileEntityBase::GetLifetime(void) const
{
	return m_fLifetime;
}

// Get the speed of the projectile
float CProjectileEntityBase::GetSpeed(void) const
{
	return m_fSpeed;
}

// Set the direction of the projectile
void CProjectileEntityBase::SetDirection(Vector3 theNewDirection)
{
	theDirection = theNewDirection;
}

// Set the lifetime of the projectile
void CProjectileEntityBase::SetLifetime(const float m_fLifetime)
{
	this->m_fLifetime = m_fLifetime;
}

// Set the speed of the projectile
void CProjectileEntityBase::SetSpeed(const float m_fSpeed)
{
	this->m_fSpeed = m_fSpeed;
}

// Set the source of the projectile
void CProjectileEntityBase::SetSource(EntityBase* _source)
{
	theSource = _source;
}

// Get the source of the projectile
EntityBase* CProjectileEntityBase::GetSource(void) const
{
	return theSource;
}

// Update the status of this projectile
void CProjectileEntityBase::Update(double dt)
{
	if (m_bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	m_fLifetime -= (float)dt;
	if (m_fLifetime < 0.0f)
	{
		SetStatus(false);
		SetIsDone(true);	// This method is to inform the EntityManager that it should remove this instance
		return;
	}

	// Update Position
	position.Set(	position.x + (float)(theDirection.x * dt * m_fSpeed),
					position.y + (float)(theDirection.y * dt * m_fSpeed),
					position.z + (float)(theDirection.z * dt * m_fSpeed));
}


// Render this projectile
void CProjectileEntityBase::Render(void)
{
	if (m_bStatus == false)
		return;

	if (m_fLifetime < 0.0f)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	//modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

// Create a projectile and add it into EntityManager
CProjectileEntityBase* Create::ProjectileEntityBase(const std::string& _meshName, 
								const Vector3& _position, 
								const Vector3& _direction, 
								const float m_fLifetime, 
								const float m_fSpeed,
								EntityBase* _source)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CProjectileEntityBase* result = new CProjectileEntityBase(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetSource(_source);
	EntityManager::GetInstance()->AddEntity(result);

	return result;
}