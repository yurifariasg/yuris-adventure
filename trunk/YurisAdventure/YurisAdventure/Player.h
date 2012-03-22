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
const int BUFF_TIME = 1000;
const int INCREASE_DEFENSE_PERCENTAGE = 50;
const int ATTACK_COMBO_MANA = 30;
const int MAGIC_COMBO_MANA = 30;
const int DEFENSE_COMBO_MANA = 30;

/*
	Class will hold properties for our Player
*/
class Player : public Creature, public AnimatedSprite
{
private:
	PlayerComboState comboState;
	int actionTime;
	int attackingTime;
	bool isCharging;
	bool isCasting;
	bool isPenetrationActive;
	bool isTakingDamage;

	int buffTimer;

	ImageShower* comboImageShower;

	void updateBuffs();

public:
	Player(int hp, int mana, int attack);
	void processCombo(unsigned int pressedKey);
	void updateSprite();
	int getActionTime() { return actionTime; }
	bool getDeathAnimationFinished() { return attackingTime == 1; }
	void setTakingDamage(bool isTaking) {
		if (isTaking) actionTime = 100;
		isTakingDamage = isTaking; }
	bool getIsTakingDamage() { return isTakingDamage; }

	PlayerComboState getCurrentComboState() { return comboState; }

	void charges() { isCharging = true; }
	void stopCharging() { isCharging = false; }

	bool canDoSomething() { return !isDead() && !isCharging && !isAttacking() && !isCasting; }
	bool notCharging() { return !isCharging; }

	bool getPenetrationIsActive() { return isPenetrationActive; }

	bool hasBuffActives() { return buffTimer != 0; }

	void setImageShower(ImageShower* i) { comboImageShower = i; }

	void casts() {
		attackingTime = 10;
		isCasting = true;
	}

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