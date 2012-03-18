#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "Player.h"
#include "EnchancedPhysics.h"

class FightPhysics : public EnchancedPhysics
{
	private:
		bool hasAABBCollision(int,int,int,int,int,int,int,int);

		bool wasAttacking;


	public:
		void updatePhysics(Game*);
};