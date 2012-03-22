#pragma once

#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"
#include "BotVelocity.h"
#include "BotState.h"
#include "Creature.h"
#include "SSSF_SourceCode\gsm\world\Point.h"

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


	Point*	searchPlayerPosition(Game*);

	int getTrueX() {
		return getPhysicalProperties()->getX() + getBoundingVolume()->getX() +
			(getBoundingVolume()->getWidth() / 2); }

	int getTrueY() {
		return getPhysicalProperties()->getY() + getBoundingVolume()->getY() +
			(getBoundingVolume()->getHeight() / 2); }

	int getBotSpeedVelocity() {
		switch (velocity) {
				case BOT_SLOW: return PLAYER_SPEED * 0.3;
				case BOT_NORMAL: return PLAYER_SPEED * 0.6;
				case BOT_FAST: return PLAYER_SPEED * 1.2;
		}
		return 0;
	}

public:
	EnemyBot(int hp, int attack, BotVelocity);
	EnemyBot*	clone();
	void	think(Game *game);
	void	setState(BotState newState) { state = newState; }
	void	setBotSpeed(int newSpeed) { botSpeed = newSpeed; }
	int		getActionTime() { return actionTime; }
	BotState getState() { return state; }
};