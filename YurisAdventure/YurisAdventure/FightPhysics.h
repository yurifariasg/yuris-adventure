#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "Player.h"
#include "EnchancedPhysics.h"

const int ATTACK_POINT_OFFSET = 30;

class FightPhysics : public EnchancedPhysics
{
	private:
		bool hasAABBCollision(int,int,int,int,int,int,int,int);

		bool wasAttacking;


	public:
		void updatePhysics(Game*);
};