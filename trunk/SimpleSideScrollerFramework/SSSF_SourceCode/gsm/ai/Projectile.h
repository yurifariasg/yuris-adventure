#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

class Projectile : public AnimatedSprite
{
	private:
		int projectileSpeed;
		bool toRight;
		bool enabled;
		int dmgWhenHit;
		wstring type;

	public:
		Projectile(wstring, int, int);
		int getDamage() { return dmgWhenHit; }
		void setFacingDirectionRight(bool isRight) { toRight = isRight; }
		bool isFacingRight() { return toRight; }
		int getSpeed() { return projectileSpeed; }
		wstring getType() { return type; }
		Projectile* clone();
		void updateSprite();
		bool isEnabled() { return enabled; }
		void disables() { enabled = false; }
		void enables() { enabled = true; }
};