#pragma once
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

class Projectile : public AnimatedSprite
{
	private:
		int projectileSpeed;
		int dmgWhenHit;

	public:
		Projectile(int, int);
		int getDamage() { return dmgWhenHit; }
		int getSpeed() { return projectileSpeed; }
};