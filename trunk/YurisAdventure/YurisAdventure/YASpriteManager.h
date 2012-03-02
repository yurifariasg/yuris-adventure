#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\SpriteManager.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "PlayerComboState.h"
#include "Creature.h"
#include "YAGame.h"
#include "Player.h"

/*
	Creates our own SpriteManager which has a Player Associated to it
*/
class YASpriteManager : public SpriteManager
{
private:
	Player* player;

public:
	YASpriteManager();
	~YASpriteManager();
	Player* getPlayer() { return player; }

};