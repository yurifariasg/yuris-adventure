#pragma once

#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"

/*
	Class to hold informations about a Living Creature on the Game
	Living Creature which has HP, Attack, Defense and Facing Direction...
*/

class Creature
{
private:
	unsigned int maxHP;
	unsigned int currentHP;
	unsigned int attack;
	unsigned int defense;
	bool facedLeft;
	bool attacking;

public:
	Creature(unsigned int, unsigned int);
	Creature*	clone();
	unsigned int getCurrentHP() { return currentHP; };

	void takeDamage(unsigned int damage);
	void recoverHealth(unsigned int recoveredHealth);

	unsigned int getAttack() { return attack; };
	bool isDead() { return currentHP == 0; };
	bool isFacingRight() { return !facedLeft; };
	bool isFacingLeft() { return facedLeft; };
	void changeFacingDirection() { facedLeft = !facedLeft; }
	void startAttack() { attacking = true; };
	void stopAttack() { attacking = false; };
	bool isAttacking() { return attacking; }
};