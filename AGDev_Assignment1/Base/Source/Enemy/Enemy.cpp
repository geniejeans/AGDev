#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../WeaponInfo/Pistol.h"
#include "../Waypoint/WaypointManager.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

CEnemy::CEnemy()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, positionOfPlayer(Vector3(0.0f, 0.0f, 0.0f))
	, previousWayPoint(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, m_iSeed(0)
	, elapsed_time(0.0f)
	, distanceFromPlayer(0.0f)
	, primaryWeapon(NULL)
	, m_iWayPointIndex(-1)
	, sm(NULL)
{
	listOfWaypoints.clear();
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set up the waypoints
	listOfWaypoints.push_back(0);
	listOfWaypoints.push_back(1);
	listOfWaypoints.push_back(2);

	m_iWayPointIndex = 0;

	// Set the current values
	position.Set(10.0f, 0.0f, 0.0f);
	//if (m_pTerrain)
	//	target = GenerateTarget();
	//else
	//	target.Set(10.0f, 0.0f, 450.0f);
	CWaypoint* nextWaypoint = GetNextWaypoint();
	if (nextWaypoint)
	{
		target = nextWaypoint->GetPosition();
		previousWayPoint = target;
	}
	else
		target = Vector3(0, 0, 0);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 18.0;

	// Initialise the LOD meshes
	InitLOD("tanktop", "tanktopmid", "tanktopmid");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	primaryWeapon = new CPistol();
	primaryWeapon->Init();
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, "", true);
}

void CEnemy::Init(float x, float y, float z)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set up the waypoints
	listOfWaypoints.push_back(0);
	listOfWaypoints.push_back(1);
	listOfWaypoints.push_back(2);

	m_iWayPointIndex = 0;

	// Set the current values
	position.Set(x, y, z);
	CWaypoint* nextWaypoint = GetNextWaypoint();
	if (nextWaypoint)
	{
		target = nextWaypoint->GetPosition();
		previousWayPoint = target;
	}
		
	else
		target = Vector3(0, 0, 0);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 18.0;

	// Initialise the LOD meshes
	InitLOD("tanktop", "tanktop", "tanktop");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	primaryWeapon = new CPistol();
	primaryWeapon->Init();

	distance = 2;
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, "", true);
}

// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CEnemy::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

CWaypoint * CEnemy::GetNextWaypoint(void)
{
	if ((int)listOfWaypoints.size() > 0)
	{
		std::cout << "YO MAN" << position << std::endl;
		m_iWayPointIndex++;
		if (m_iWayPointIndex >= (int)listOfWaypoints.size())
			m_iWayPointIndex = 0;
		return CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[m_iWayPointIndex]);
	}
	else
		return NULL;

}

// Update
void CEnemy::Update(double dt)
{
	elapsed_time += dt;
	Vector3 viewVector = (target - position).Normalized();
	if (sm->GetCurrentState() != "ATTACK")
		position += viewVector * (float)m_dSpeed * (float)dt;//This is added
	// Constrain the position
	Constrain();
	if (sm->GetCurrentState() != "PATROL" && elapsed_time >= 0.4f)
	{
		if (primaryWeapon)
			primaryWeapon->Discharge(position, EntityManager::GetInstance()->GetPlayerTarget(), this);
		elapsed_time = 0.0f;
	}
	sm->Update(dt);
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

//	if (abs(((target.x - position.x) * (target.x - position.x) - (target.z - position.z)*(target.z - position.z))) < m_dSpeed)
//	{
//		target = GenerateTarget();
//	}

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	//if (position.y != m_pTerrain->GetTerrainHeight(position))
	//	position.y = m_pTerrain->GreetTerrainHeight(position);
}

// Render
void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(Math::RadianToDegree(atan2(CPlayerInfo::GetInstance()->GetPosX() - position.x, CPlayerInfo::GetInstance()->GetPosZ() - position.z)), 0, 1, 0);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel << endl;
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}

// Generate New Target
Vector3 CEnemy::GenerateTarget(void)
{
	return Vector3(	rand() % (int)((maxBoundary.x - minBoundary.x)*0.5),	
					0.0f, 
					rand() % (int)((maxBoundary.x - minBoundary.x)*0.5));
}

// Set random seed
void CEnemy::SetRandomSeed(const int m_iSeed)
{
	this->m_iSeed = m_iSeed;
}
