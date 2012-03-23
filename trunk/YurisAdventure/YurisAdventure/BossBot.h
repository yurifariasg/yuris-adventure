#pragma once
#include "enemybot.h"
#include "SSSF_SourceCode\game\Game.h"
#include "YAGame.h"

/*
	Different bot that has a bigger range to follow the player
*/

class BossBot : public EnemyBot
{
public:
	BossBot::BossBot(int hp, int attack, BotVelocity velocity) : EnemyBot(hp, attack, velocity) {};
	~BossBot(void);
	void think(Game*);
};

