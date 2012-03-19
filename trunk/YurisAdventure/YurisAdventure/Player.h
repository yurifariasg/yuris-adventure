#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "PlayerComboState.h"
#include "Creature.h"
#include "YAGame.h"

/*
	Class will hold properties for our Player
*/
class Player : public Creature, public AnimatedSprite
{
private:
	PlayerComboState comboState;
	int actionTime;
	bool isCharging;

public:
	Player(int hp, int mana, int attack);
	void processCombo(unsigned int pressedKey);
	void updateSprite();
	int getActionTime() { return actionTime; }

	void charges() { isCharging = true; }
	void stopCharging() { isCharging = false; }

	bool canDoSomething() { return !isDead() && !isCharging && !isAttacking(); }
	bool notCharging() { return !isCharging; }

	void reloadData() {
		actionTime = 0;
		comboState = COMBO_NONE;
		Creature::resetData();
	}
};