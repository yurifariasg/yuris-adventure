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

/*
	Class will hold properties for our Player
*/
class Player : public Creature, public AnimatedSprite
{
private:
	PlayerComboState comboState;
	int actionTime;
	bool isCharging;
	bool isCasting;

	int buffTimer;

	ImageShower* comboImageShower;

	void updateBuffs();

public:
	Player(int hp, int mana, int attack);
	void processCombo(unsigned int pressedKey);
	void updateSprite();
	int getActionTime() { return actionTime; }

	PlayerComboState getCurrentComboState() { return comboState; }

	void charges() { isCharging = true; }
	void stopCharging() { isCharging = false; }

	bool canDoSomething() { return !isDead() && !isCharging && !isAttacking() && !isCasting; }
	bool notCharging() { return !isCharging; }

	void setImageShower(ImageShower* i) { comboImageShower = i; }

	void casts() {
		actionTime = 10;
		isCasting = true;
	}

	void reloadData() {
		actionTime = 0;
		comboState = COMBO_NONE;
		Creature::resetData();
	}
};