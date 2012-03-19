#pragma
#include "Creature.h"

/*
	Creates a Creature with the given amount of health and attack
*/
Creature::Creature(int hp, int atk)
{
	init(hp, 0, atk);
}

Creature::Creature(int hp, int mana, int atk)
{
	init(hp, mana, atk);
}

void Creature::init(int hp, int mana, int atk)
{
	maxHP = hp;
	cHP = hp;
	maxMana = mana;
	cMana = mana;
	attack = atk;
	defense = 0;
	facedLeft = false; // Initialize Facing Right
	attacking = false;
}

/*
	Clones this Creature
*/
Creature* Creature::clone()
{
	Creature* creature = new Creature(maxHP, attack);
	return creature;
}

/*
	Creature takes the given amount of damage
*/
void Creature::takeDamage(int damage)
{
	cHP -= damage;
	if (cHP < 0) cHP = 0;

}
/*
	Recover the given amount of Health for this Creature
*/
void Creature::recoverHealth(int recoveredHealth)
{
	cHP += recoveredHealth;
	if (cHP > maxHP) cHP = maxHP;
}