#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../WeaponInfo/WeaponInfo.h"
#include "../Waypoint/Waypoint.h"
#include "../States/StateMachine.h"

#include <vector>
using namespace std;

class Mesh;

class CEnemy :
	public GenericEntity
{
protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	Vector3 positionOfPlayer;
	Vector3 previousWayPoint;
	GroundEntity* m_pTerrain;

	//vector containing IDs of Waypoints
	vector<int> listOfWaypoints;
	//Current ID of Waypoint
	int m_iWayPointIndex;

	double m_dSpeed;
	double m_dAcceleration;

	int m_iSeed;

	CWeaponInfo* primaryWeapon;

	float elapsed_time;
	float distance = 1;
	float distanceFromPlayer;

public:
	CEnemy(void);
	virtual ~CEnemy();

	void Init(void);
	void Init(float x, float y, float z);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);
	// Set the distance from player
	void SetDistanceFromPlayer(float distance) { distanceFromPlayer = distance; };
	// Set the position of the player
	void SetPositionOfPlayer(Vector3 playerPos) { positionOfPlayer = playerPos; };
	// Set previous waypoint
	void SetPreviousWaypoint(Vector3 previousWaypoint) { previousWayPoint = previousWaypoint; };

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get postion of player
	Vector3 GetPositionOfPlayer() { return positionOfPlayer; };
	// Get previous waypoint
	Vector3 GetPreviousWaypoint() { return previousWayPoint; };
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);
	// Get next Waypoint for this CEnemy
	CWaypoint* GetNextWaypoint(void);
	// Get distance from player
	float GetDistanceFromPlayer() {return distanceFromPlayer;};


	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);
	// Set random seed
	void SetRandomSeed(const int m_iSeed);
	// Generate New Target
	Vector3 GenerateTarget(void);
	//State machine
	StateMachine *sm;
};
