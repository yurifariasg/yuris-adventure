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
	int maxMana;
	int cMana;
	int attack;
	int defense;
	bool facedLeft;
	bool attacking;

	void init(int hp, int mana, int atk);

public:
	Creature(int, int);
	Creature(int, int, int);
	Creature*	clone();
	int getCurrentHP() { return cHP; }
	int getCurrentMana() { return cMana; }

	int getMaxHP() { return maxHP; }
	int getMaxMana() { return maxMana; }

	void takeDamage(int damage);
	void recoverHealth(int recoveredHealth);
	void useMana(int manaUsed);
	void recoverMana(int manaRecovered);

	int getAttack() { return attack; };
	void setAttack(int newAttack) { attack = newAttack; };

	int getDefense() { return defense; }
	void setDefense(int newDefense) { defense = newDefense; }

	bool isDead() { return (cHP == 0); };
	bool isFacingRight() { return !facedLeft; };
	bool isFacingLeft() { return facedLeft; };
	void changeFacingDirection() { facedLeft = !facedLeft; }
	void startAttack() { attacking = true; };
	void stopAttack() { attacking = false; };
	bool isAttacking() { return attacking; }
	void recoverAllHealth() { cHP = maxHP; }
	void recoverAllMana() { cMana = maxMana; }

	void resetData() {
		facedLeft = false;
		attacking = false;
		recoverAllHealth();
		recoverAllMana();
	}
};