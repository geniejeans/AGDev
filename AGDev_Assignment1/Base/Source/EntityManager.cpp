#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile/Laser.h"
#include "SceneGraph\SceneGraph.h"
#include "Enemy\Enemy.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	std::list<EntityBase*>::iterator it2, end2;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		if ((*it)->GetMeshName() == "Player" || (*it)->GetMeshName() == "Enemy")
		{
			for (it2 = it; it2 != end; ++it2)
			{
				if (it2 == it)
					continue;
				if ((*it2)->GetMeshName() == "Player" || (*it2)->GetMeshName() == "Enemy")
				{
					CEnemy *enemy;
					if ((*it)->GetMeshName() == "Enemy")
					{
						enemy = static_cast<CEnemy*>(*it);
						enemy->SetPositionOfPlayer((*it2)->GetPosition());
					}
				
					else
					{
						enemy = static_cast<CEnemy*>(*it2);
						enemy->SetPositionOfPlayer((*it)->GetPosition());
					}
						
					enemy->SetDistanceFromPlayer(((*it)->GetPosition() - (*it2)->GetPosition()).Length());
		//			std::cout << enemy->GetDistanceFromPlayer() << std::endl;
					break;
				}
			}
		}
		(*it)->Update(_dt);
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Update();

	// Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Update();

	// Check for Collision amongst entities with collider properties
	CheckForCollision();

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	std::list<EntityBase*>::iterator it2, end2;
	end = entityList.end();
	end2 = treeList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}

	for (it2 = treeList.begin(); it2 != end2; ++it2)
	{
		(*it2)->Render();

	}
	// Render the Scene Graph
	CSceneGraph::GetInstance()->Render();

	// Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;

	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}


}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, string type, bool bAddToSpatialPartition)
{
	if (type == "tree")
		treeList.push_back(_newEntity);
	else
		entityList.push_back(_newEntity);

	// Add to the Spatial Partition
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);
	std::list<EntityBase*>::iterator findIter2 = std::find(treeList.begin(), treeList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);

		// Remove from SceneNode too
		if (CSceneGraph::GetInstance()->DeleteNode(_existingEntity)==false)
		{
			cout << "EntityManager::RemoveEntity: Unable to remove this entity from Scene Graph" << endl;
		}
		else
		{
			// Add to the Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}

		return true;	
	}

	// Delete the entity if found
	if (findIter2 != treeList.end())
	{
		delete *findIter2;
		findIter2 = treeList.erase(findIter);

		// Remove from SceneNode too
		if (CSceneGraph::GetInstance()->DeleteNode(_existingEntity) == false)
		{
			cout << "EntityManager::RemoveEntity: Unable to remove this entity from Scene Graph" << endl;
		}
		else
		{
			// Add to the Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}

		return true;
	}
	// Return false if not found
	return false;
}

// Mark an entity for deletion
bool EntityManager::MarkForDeletion(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);
	std::list<EntityBase*>::iterator findIter2 = std::find(treeList.begin(), treeList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		(*findIter)->SetIsDone(true);
		return true;
	}

	// Delete the entity if found
	if (findIter2 != treeList.end())
	{
		(*findIter2)->SetIsDone(true);
		return true;
	}
	// Return false if not found
	return false;
}

// Set a handle to the Spatial Partition to this class
void EntityManager::SetSpatialPartition(CSpatialPartition* theSpatialPartition)
{
	this->theSpatialPartition = theSpatialPartition;
}

void EntityManager::ChangePlayerHealth(int value)
{
	CPlayerInfo::GetInstance()->ChangeHealth(value);
}

// Constructor
EntityManager::EntityManager()
	: theSpatialPartition(NULL)
	, previousLaserStart(0.0f,0.0f,0.0f)
{
}

// Destructor
EntityManager::~EntityManager()
{
	// Drop the Spatial Partition instance
	CSpatialPartition::GetInstance()->DropInstance();

	// Clear out the Scene Graph
	CSceneGraph::GetInstance()->Destroy();
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	// Check if this object is overlapping that object
	/*
	if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	||
	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	*/
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	// Check if that object is overlapping this object
	/*
	if (((thisMinAABB.x >= thatMinAABB.x) && (thisMinAABB.x <= thatMaxAABB.x) &&
	(thisMinAABB.y >= thatMinAABB.y) && (thisMinAABB.y <= thatMaxAABB.y) &&
	(thisMinAABB.z >= thatMinAABB.z) && (thisMinAABB.z <= thatMaxAABB.z))
	||
	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	*/
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	// Check if this object is within that object
	/*
	if (((thisMinAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMinAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMinAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z))
	&&
	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	*/
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	// Check if that object is within this object
	/*
	if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	&&
	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	*/
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
		DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
	{
		return true;
	}

	return false;
}

bool EntityManager::CheckPlayerSphereCollision(CPlayerInfo * Player, EntityBase * ThisEntity)
{
	double distance = sqrt(((ThisEntity->GetPosition().x - Player->GetPos().x) * (ThisEntity->GetPosition().x - Player->GetPos().x))

		+ ((ThisEntity->GetPosition().z - Player->GetPos().z) * (ThisEntity->GetPosition().z - Player->GetPos().z)));

	double PlayerRadius = 5;

	double ObjOneRadius = ThisEntity->GetScale().x;

	double sumOfRadius = PlayerRadius + ObjOneRadius;

	if (distance > sumOfRadius + 5)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
//	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
//	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

// Check where a line segment between two positions intersects a plane
bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if ((fDst1 * fDst2) >= 0.0f) 
		return false;
	if (fDst1 == fDst2) 
		return false;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

// Check two positions are within a box region
bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}

// Check for intersection between a line segment and a plane
bool EntityManager::CheckLineSegmentPlane(	Vector3 line_start, Vector3 line_end, 
											Vector3 minAABB, Vector3 maxAABB,
											Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	bool playerCollide = false;

	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;
	std::list<EntityBase*>::iterator colliderTree, colliderTreeEnd;

	colliderThisEnd = entityList.end();
	colliderTreeEnd = treeList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		// Check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			// Dynamic cast it to a CLaser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			//Checking with spatial partitioning 
			//int newGrid = 0;
			//vector<EntityBase*> ExportList;
			//if (previousGrid == 0)
			//	ExportList = CSpatialPartition::GetInstance()->GetObjects((*colliderThis)->GetPosition(), 0.05f, previousGrid);
			//else
			//	ExportList = CSpatialPartition::GetInstance()->GetObjects((*colliderThis)->GetPosition(), 0.05f, newGrid);
			if ((*colliderThis)->GetPosition().z > 500 || (*colliderThis)->GetPosition().z < -500
				|| (*colliderThis)->GetPosition().x > 500 || (*colliderThis)->GetPosition().x < -500)
				continue;
			vector<EntityBase*> ExportList = CSpatialPartition::GetInstance()->GetObjects((*colliderThis)->GetPosition(), 0.05f);
			if (ExportList.size() != 0)
			{
				if (previousLaserStart.IsZero())
				{
					for (int i = 0; i < ExportList.size(); ++i)
					{
						if (ExportList[i]->HasCollider() && !ExportList[i]->IsDone())
						{
							Vector3 hitPosition = Vector3(0, 0, 0);

							// Get the minAABB and maxAABB for (*colliderThat)
							CCollider *thatCollider = dynamic_cast<CCollider*>(ExportList[i]);
							Vector3 thatMinAABB = (ExportList[i])->GetPosition() + thatCollider->GetMinAABB();
							Vector3 thatMaxAABB = (ExportList[i])->GetPosition() + thatCollider->GetMaxAABB();

							if (CheckLineSegmentPlane(thisEntity->GetPosition(),
								thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
								thatMinAABB, thatMaxAABB,
								hitPosition) == true)
							{
								if ((ExportList[i])->GetMeshName() == "Enemy")
								{
									//	CEnemy *enemy = dynamic_cast<CEnemy*>(ExportList[i]);

									ExportList[i]->ChangeHealth(-20);
									thisEntity->SetIsDone(true);
									//std::cout << ExportList[i]->GetHealth() << std::endl;
									if ((ExportList[i])->GetHealth() <= 0)
									{
										ExportList[i]->SetIsDone(true);
										// Remove from Scene Graph
										if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
										{
											cout << "*** This Entity removed ***" << endl;
										}
										// Remove from Scene Graph
										if (CSceneGraph::GetInstance()->DeleteNode(ExportList[i]) == true)
										{
											cout << "*** That Entity removed ***" << endl;
										}
									}
									continue;
								}
								(*colliderThis)->SetIsDone(true);
								(ExportList[i])->SetIsDone(true);


								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
								{
									cout << "*** This Entity removed ***" << endl;
								}
								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode(ExportList[i]) == true)
								{
									cout << "*** That Entity removed ***" << endl;
								}

							}
						}
					}
					previousLaserStart = thisEntity->GetPosition();
				}
				else
				{
					for (int i = 0; i < ExportList.size(); ++i)
					{
						if (ExportList[i]->HasCollider() && !ExportList[i]->IsDone())
						{
							Vector3 hitPosition = Vector3(0, 0, 0);

							// Get the minAABB and maxAABB for (*colliderThat)
							CCollider *thatCollider = dynamic_cast<CCollider*>(ExportList[i]);
							Vector3 thatMinAABB = (ExportList[i])->GetPosition() + thatCollider->GetMinAABB();
							Vector3 thatMaxAABB = (ExportList[i])->GetPosition() + thatCollider->GetMaxAABB();

							if (CheckLineSegmentPlane(previousLaserStart,
								thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
								thatMinAABB, thatMaxAABB,
								hitPosition) == true)
							{
								if ((ExportList[i])->GetMeshName() == "Enemy")
								{
									//	CEnemy *enemy = dynamic_cast<CEnemy*>(ExportList[i]);

									ExportList[i]->ChangeHealth(-20);
									thisEntity->SetIsDone(true);
									//std::cout << ExportList[i]->GetHealth() << std::endl;
									if ((ExportList[i])->GetHealth() <= 0)
									{
										ExportList[i]->SetIsDone(true);
										// Remove from Scene Graph
										if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
										{
											cout << "*** This Entity removed ***" << endl;
										}
										// Remove from Scene Graph
										if (CSceneGraph::GetInstance()->DeleteNode(ExportList[i]) == true)
										{
											cout << "*** That Entity removed ***" << endl;
										}
									}
									continue;
								}
								(*colliderThis)->SetIsDone(true);
								(ExportList[i])->SetIsDone(true);


								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
								{
									cout << "*** This Entity removed ***" << endl;
								}
								// Remove from Scene Graph
								if (CSceneGraph::GetInstance()->DeleteNode(ExportList[i]) == true)
								{
									cout << "*** That Entity removed ***" << endl;
								}

							}
						}
					}
			
				}
				changePosition++; 
				if (changePosition > 20)
				{
					previousLaserStart = thisEntity->GetPosition();
					changePosition = 0;
				}
					
			}
		//	if (previousGrid != newGrid)
		//		CSpatialPartition::GetInstance()->DeRenderGrids(previousGrid);
		}
		else if ((*colliderThis)->HasCollider())
		{
			// This object was derived from a CCollider class, then it will have Collision Detection methods
			//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);
			int newGrid = 0;
			if (previousGrid == 0)
				CSpatialPartition::GetInstance()->GetObjects(CPlayerInfo::GetInstance()->GetPos(), 0.05f, previousGrid);
			else
				CSpatialPartition::GetInstance()->GetObjects(CPlayerInfo::GetInstance()->GetPos(), 0.05f, newGrid);

			if (newGrid != 0 && previousGrid != newGrid)
			{
				CSpatialPartition::GetInstance()->DeRenderGrids(previousGrid);
				previousGrid = newGrid;
			}
			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;

			if (CheckPlayerSphereCollision(CPlayerInfo::GetInstance(), thisEntity))
			{
				CollisionPlayerResponse(CPlayerInfo::GetInstance(), thisEntity);
				playerCollide = true;
			}
			else if (!playerCollide)
			{
				CPlayerInfo::GetInstance()->SetCollision(false);
				(thisEntity)->SetCollidePlayer(false);
			}

			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis || ((*colliderThat)->GetMeshName() == "EnemyBullet" && (*colliderThis)->GetMeshName() == "Enemy")
					|| ((*colliderThis)->GetMeshName() == "EnemyBullet" && (*colliderThat)->GetMeshName() == "Enemy")
					|| ((*colliderThis)->GetMeshName() == "Dummy" && (*colliderThat)->GetMeshName() == "Dummy")
					|| ((*colliderThis)->GetMeshName() == "Enemy" && (*colliderThat)->GetMeshName() == "Enemy")
					|| ((*colliderThis)->GetMeshName() == "Enemy" && (*colliderThat)->GetMeshName() == "Barrel")
					|| ((*colliderThis)->GetMeshName() == "Barrel" && (*colliderThat)->GetMeshName() == "Enemy"))
					continue;

				if ((*colliderThat)->HasCollider())
				{
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (CheckSphereCollision(thisEntity, thatEntity))
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);
							// Remove from Scene Graph
							if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
							{
								cout << "*** This Entity removed ***" << endl;
							}
							// Remove from Scene Graph
							if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
							{
								cout << "*** That Entity removed ***" << endl;
							}
						}
					}
				}
			}
		}
	}

	for (colliderTree = treeList.begin(); colliderTree != colliderTreeEnd; ++colliderTree)
	{
		EntityBase *treeEntity = dynamic_cast<EntityBase*>(*colliderTree);

		if (CheckPlayerSphereCollision(CPlayerInfo::GetInstance(), treeEntity))
		{
			CollisionPlayerResponse(CPlayerInfo::GetInstance(), treeEntity);
			playerCollide = true;
		}
		else if (!playerCollide)
		{
			CPlayerInfo::GetInstance()->SetCollision(false);
			(treeEntity)->SetCollidePlayer(false);
		}
	}
	return false;
}

void EntityManager::CollisionPlayerResponse(CPlayerInfo * Player, EntityBase * ThisEntity)
{
	Player->SetCollision(true);
	if (ThisEntity->GetMeshName() == "Flag")
	{
		float newX = 1.0f + (rand() % 1000 - 500.0f);
		float newZ = 1.0f + (rand() % 1000 - 500.0f);
		Player->ChangePoints(10);
		ThisEntity->SetPosition(Vector3(newX, 6.0f, newZ));
	}
	else if (ThisEntity->GetMeshName() == "EnemyBullet")
	{
		Player->ChangeHealth(-10);
		ThisEntity->SetIsDone(true);
	}
}

Vector3 EntityManager::GetPlayerTarget()
{
	return CPlayerInfo::GetInstance()->GetTarget();
}

Vector3 EntityManager::GetPlayerPosition()
{
	return CPlayerInfo::GetInstance()->GetPos();
}

