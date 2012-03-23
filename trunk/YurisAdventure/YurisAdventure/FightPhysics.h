#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "Player.h"
#include "EnchancedPhysics.h"

const int ATTACK_POINT_OFFSET = 30;
const int TIME_TO_ATTACK_AFTER_START_ATTACKING = 10;

class FightPhysics : public EnchancedPhysics
{
	private:
		bool hasAttacked;
		bool wasAttacking;
		int counter; 


	public:
		FightPhysics() { counter = 0; }
		void updatePhysics(Game*);
};