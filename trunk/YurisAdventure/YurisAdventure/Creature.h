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
	int maxHP;
	int cHP;
	int attack;
	int defense;
	bool facedLeft;
	bool attacking;

public:
	Creature(int, int);
	Creature*	clone();
	int getCurrentHP() { return cHP; };

	void takeDamage(int damage);
	void recoverHealth(int recoveredHealth);

	int getAttack() { return attack; };
	bool isDead() { return (cHP == 0); };
	bool isFacingRight() { return !facedLeft; };
	bool isFacingLeft() { return facedLeft; };
	void changeFacingDirection() { facedLeft = !facedLeft; }
	void startAttack() { attacking = true; };
	void stopAttack() { attacking = false; };
	bool isAttacking() { return attacking; }
};