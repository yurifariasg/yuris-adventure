#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Projectile.h"

Projectile::Projectile(wstring pType, int dmg, int speed)
{
	type = pType;
	dmgWhenHit = dmg;
	projectileSpeed = speed;
	toRight = true; // Starts facing right
	enabled = true;
}

Projectile* Projectile::clone()
{
	Projectile* p = new Projectile(type, dmgWhenHit, projectileSpeed);
	p->setSpriteType(getSpriteType());

	p->getBoundingVolume()->setX(getBoundingVolume()->getX());
	p->getBoundingVolume()->setY(getBoundingVolume()->getY());
	p->getBoundingVolume()->setWidth(getBoundingVolume()->getWidth());
	p->getBoundingVolume()->setHeight(getBoundingVolume()->getHeight());

	return p;
}

void Projectile::updateSprite()
{
	if (enabled) {
		getPhysicalProperties()->setVelocity(
			toRight ? projectileSpeed : -projectileSpeed, 0);
		if (toRight) setCurrentState(PROJECTILE_STATE_MOVING_RIGHT);
		else setCurrentState(PROJECTILE_STATE_MOVING_LEFT);
	}

	AnimatedSprite::updateSprite();
}