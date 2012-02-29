#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"
#include "BotVelocity.h"
#include "BotState.h"

class EnemyBot : public Bot
{
private:
	unsigned int hp;
	unsigned int attack;
	BotVelocity velocity;
	bool isFacingLeft;
	BotState state;

	unsigned int actionTime;

public:
	EnemyBot(unsigned int, unsigned int, BotVelocity);
	Bot*	clone();
	void	think(Game *game);
	unsigned int getHP() { return hp; };
	unsigned int getAttack() { return attack; };
};