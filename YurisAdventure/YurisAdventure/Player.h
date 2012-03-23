#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "PlayerComboState.h"
#include "Creature.h"
#include "YAGame.h"
#include "ImageShower.h"

const int TIME_FOR_DEATH_ANIMATION = 300;
const int TIME_FOR_ATTACK_ANIMATION = 20;
const int TIME_FOR_COMBO_ANIMATION = 200;
const int BUFF_TIME = 1000;
const int INCREASE_DEFENSE_PERCENTAGE = 50;
const int ATTACK_COMBO_MANA = 40;
const int MAGIC_COMBO_MANA = 25;
const int DEFENSE_COMBO_MANA = 30;

const int MAGIC_ATTACK_MANA = 10;

/*
	Class will hold properties for our Player
*/
class Player : public Creature, public AnimatedSprite
{
private:
	PlayerComboState comboState;
	AnimatedSprite* aura;
	int actionTime;
	int attackingTime;
	bool isCharging;
	bool isCasting;
	bool isCrouched;
	bool isPenetrationActive;
	bool isTakingDamage;
	int buffTimer;
	int comboAnimationTimer;

	ImageShower* comboImageShower;

	// Update Player's Buff (Decrements time or Deactivate, if timed out)
	void updateBuffs();

public:
	// Creates a Player
	Player(int hp, int mana, int attack);

	// Process Player's Combo using the given pressed key
	void processCombo(unsigned int pressedKey);

	// Updates Player's Sprite
	void updateSprite();

	// Get Current ActionTime
	int getActionTime() { return actionTime; }

	// Sets players Animated Aura
	void setAura(AnimatedSprite* as) { aura = as; }

	// Verifies if the death animation is finished
	bool getDeathAnimationFinished() { return attackingTime == 1 && isDead(); }

	// Activate or Deactivate Taking Damage State
	void setTakingDamage(bool isTaking) {
		if (isTaking) actionTime = 100;
		isTakingDamage = isTaking; }

	// Verifies if the player is on the 'Taking Damage' state
	bool getIsTakingDamage() { return isTakingDamage; }

	// Activates Player's Crouch State
	void crouch();

	// Gets Player's current combo state
	PlayerComboState getCurrentComboState() { return comboState; }

	// Activates Charging State
	void charges() { isCharging = true; }

	// Stops Charging State
	void stopCharging() { isCharging = false; }

	// Verifies if the Player is doing some action
	bool canDoSomething() { return !isDead() && !isCharging && !isAttacking() && !isCasting; }

	// Verifies if the player is not charging
	bool notCharging() { return !isCharging; }

	// Verifies if magic penetration is active
	bool getPenetrationIsActive() { return isPenetrationActive; }

	// Verifies if has buff active
	bool hasBuffActives() { return buffTimer != 0; }

	// Sets a ImageShower, which will show Player's Combo Image
	void setImageShower(ImageShower* i) { comboImageShower = i; }

	// Activate a Casting Animation
	void casts() {
		attackingTime = 10;
		isCasting = true;
	}

	// Reload all player data (resets)
	void reloadData() {
		actionTime = 0;
		attackingTime = 0;
		buffTimer = 0;
		comboState = COMBO_NONE;
		isCharging = false;
		isCasting = false;
		isPenetrationActive = false;
		isTakingDamage = false;
		Creature::resetData();
	}
};