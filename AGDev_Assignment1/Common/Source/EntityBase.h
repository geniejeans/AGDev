#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"
#include <string>

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value) { position = _value; };
	inline Vector3 GetPosition() { return position; };

	inline void SetScale(const Vector3& _value) { scale = _value; };
	inline Vector3 GetScale() { return scale; };

	bool IsDone();
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);
	// Set the flag, bLaser
	virtual void SetIsLaser(const bool bLaser);
	// Get the flag, bLaser
	virtual bool GetIsLaser(void) const;

	//Set collision with player
	virtual void SetCollidePlayer(const bool _value) { m_bCollidePlayer = _value; };
	virtual bool GetCollidePlayer() { return m_bCollidePlayer; };

	virtual void SetMeshName(std::string meshName) { this->meshName = meshName; };
	virtual std::string GetMeshName() { return meshName; };

	virtual void SetBaseName(const std::string baseName) { this->baseName = baseName; };
	virtual std::string GetBaseName() { return baseName; };

	virtual int GetHealth() { return health; };
	virtual void ChangeHealth(int value) { health += value; };

protected:
	Vector3 position;
	Vector3 scale;

	int health;
	bool isDone;
	bool m_bCollider;
	bool bLaser;
	bool m_bCollidePlayer;
	std::string meshName;
	std::string baseName;
};

#endif // ENTITY_BASE_H