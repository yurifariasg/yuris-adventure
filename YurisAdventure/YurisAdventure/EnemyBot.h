#pragma once

#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"
#include "BotVelocity.h"
#include "BotState.h"
#include "Creature.h"

/*
	Class will hold properties of a bot
*/

class EnemyBot : public Bot, public Creature
{
private:
	BotVelocity velocity;
	BotState state;
	int actionTime;

	int botSpeed;

public:
	EnemyBot(int hp, int attack, BotVelocity);
	Bot*	clone();
	void	think(Game *game);
	void	setState(BotState newState) { state = newState; };
	void	setBotSpeed(int newSpeed) { botSpeed = newSpeed; };
};