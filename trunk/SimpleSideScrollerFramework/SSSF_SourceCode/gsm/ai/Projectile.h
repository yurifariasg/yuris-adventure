#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

// Projectile Sprite State

static const wstring	PROJECTILE_STATE_MOVING_RIGHT = L"PROJECTILE_MOVING_RIGHT";
static const wstring	PROJECTILE_STATE_MOVING_LEFT = L"PROJECTILE_MOVING_LEFT";

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