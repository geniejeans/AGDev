#include "GrenadeThrow.h"
#include "../Projectile/Grenade.h"
#include "../Projectile/GrenadeEntityBase.h"

CGrenadeThrow::CGrenadeThrow()
{
}


CGrenadeThrow::~CGrenadeThrow()
{
}

// Initialise this instance to default values
void CGrenadeThrow::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 10;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 10;
	// The current total number of rounds currently carried by this player
	totalRounds = 20;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 20;

	// The time between shots
	timeBetweenShots = 1.0;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Discharge this weapon
void CGrenadeThrow::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			CProjectile* aProjectile = Create::Grenade("sphere",
														position,
														(target - position).Normalized(),
														7.0f,
														8.0f,
														_source);
			aProjectile->SetCollider(true);
			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			aProjectile->SetScale(Vector3(14.5f, 14.5f, 14.5f));
			bFire = false;
			magRounds--;
		}
	}
}

void CGrenadeThrow::Discharge(Vector3 position, Vector3 target, EntityBase * _source)
{
	// Create a projectile with a cube mesh. Its position and direction is same as the player.
	// It will last for 3.0 seconds and travel at 500 units per second
	CProjectile* aProjectile = Create::GrenadeEntityBase("sphere",
		position,
		(target - position).Normalized(),
		7.0f,
		8.0f,
		_source);
	aProjectile->SetMeshName("EnemyBullet");
	aProjectile->SetCollider(true);
	aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
}
