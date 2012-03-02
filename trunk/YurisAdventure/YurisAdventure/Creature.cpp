#pragma
#include "Creature.h"

/*
	Creates a Creature with the given amount of health and attack
*/
Creature::Creature(unsigned int hp, unsigned int attack)
{
	this->maxHP = hp;
	this->currentHP = hp;
	this->attack = attack;
	this->defense = 0;
	this->facedLeft = false; // Initialize Facing Right
	this->attacking = false;
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
void Creature::takeDamage(unsigned int damage)
{
	currentHP -= damage;
	if (currentHP < 0) currentHP = 0;

}
/*
	Recover the given amount of Health for this Creature
*/
void Creature::recoverHealth(unsigned int recoveredHealth)
{
	currentHP += recoveredHealth;
	if (currentHP > maxHP) currentHP = maxHP;
}