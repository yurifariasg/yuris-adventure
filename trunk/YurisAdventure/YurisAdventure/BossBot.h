#pragma once
#include "enemybot.h"
class BossBot : public EnemyBot
{
public:
	BossBot::BossBot(int hp, int attack, BotVelocity velocity) : EnemyBot(hp, attack, velocity) {};
	~BossBot(void);
	void think(Game*);
};

