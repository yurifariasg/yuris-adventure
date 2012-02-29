#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "EnemyBot.h"
#include "YAGame.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"

EnemyBot::EnemyBot(unsigned int hp, unsigned int attack, BotVelocity velocity)
{
	this->hp = hp;
	this->attack = attack;
	this->velocity = velocity;
	isFacingLeft = true;
	state = BOT_IDLE;
	actionTime = 0;

	srand((unsigned int)time(0));
}

void EnemyBot::think(Game *game)
{

	if (actionTime == 0) { // Action Time Over, Make a Decision!

		if (state == BOT_IDLE) { // It WAS IDLE

			// Face Randomly
			isFacingLeft = (rand() % 2) == 0;

			float botSpeed = 0;

			switch (velocity) {
				case BOT_SLOW: botSpeed = PLAYER_SPEED * 0.3; break;
				case BOT_NORMAL: botSpeed = PLAYER_SPEED * 0.6; break;
				case BOT_FAST: botSpeed = PLAYER_SPEED * 1.2; break;
			}

			if (isFacingLeft) {
				botSpeed = -botSpeed;
				setCurrentState(BOT_MOVING_LEFT);
			} else {
				setCurrentState(BOT_MOVING_RIGHT);
			}

			state = BOT_MOVING;

			this->pp.setVelocitySafely(
				game->getGSM()->getPhysics(),
				botSpeed, 0);

			// For some time...
			actionTime = 100 + (rand() % 100);

		} else if (state == BOT_MOVING) { // It WAS MOVING

			// Stay IDLE for a little...

			if (isFacingLeft) setCurrentState(BOT_IDLE_LEFT);
			else setCurrentState(BOT_IDLE_RIGHT);

			state = BOT_IDLE;

			this->pp.setVelocitySafely(
				game->getGSM()->getPhysics(),
				0, 0);

			// For some time...
			actionTime = 100 + (rand() % 100);
		}

	} else {
		actionTime--;
	}
}

Bot* EnemyBot::clone()
{
	EnemyBot *botClone = new EnemyBot(hp, attack, velocity);
	return botClone;
}