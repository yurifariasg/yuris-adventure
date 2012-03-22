#include "PenetrableProjectile.h"
#include "SSSF_SourceCode\gsm\ai\Projectile.h"


PenetrableProjectile::PenetrableProjectile(wstring pType, int dmg, int speed) : Projectile(pType, dmg, speed)
{
	penetrable = false;
}


PenetrableProjectile::~PenetrableProjectile(void)
{
	vector<Creature*>::iterator cIterator = creaturesHit.begin();
	while (cIterator !=creaturesHit.end())
	{
		Creature* c = *cIterator;
		cIterator++;
	}

}

void PenetrableProjectile::hit(Creature* cHit)
{

	bool wasHitBefore = false;
	vector<Creature*>::iterator cIterator = creaturesHit.begin();
	while (cIterator !=creaturesHit.end())
	{
		Creature* c = *cIterator;
		if (cHit == c) wasHitBefore = true;
		cIterator++;
	}

	if (!wasHitBefore) {

		creaturesHit.push_back(cHit);
		cHit->takeDamage(getDamage());

	}

	if (!penetrable) disables();

}

PenetrableProjectile* PenetrableProjectile::clone()
{
	PenetrableProjectile* p = new PenetrableProjectile(getType(), getDamage(), getSpeed());
	p->setSpriteType(getSpriteType());

	p->getBoundingVolume()->setX(getBoundingVolume()->getX());
	p->getBoundingVolume()->setY(getBoundingVolume()->getY());
	p->getBoundingVolume()->setWidth(getBoundingVolume()->getWidth());
	p->getBoundingVolume()->setHeight(getBoundingVolume()->getHeight());
	
	// Penetration not passed

	return p;
}

void PenetrableProjectile::disables()
{
	creaturesHit.clear();
	Projectile::disables();
}