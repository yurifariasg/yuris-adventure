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
	unsigned int actionTime;

public:
	Player(unsigned int hp, unsigned int attack);
	void processCombo(unsigned int pressedKey);
	void updateSprite();
};