#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "EnemyBot.h"
#include "YAGame.h"
#include "Player.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"

EnemyBot::EnemyBot(int hp, int attack, BotVelocity velocity) : Creature(hp, attack)
{
	this->velocity = velocity;
	state = BOT_IDLE;
	actionTime = 0;

	srand((unsigned int)time(0));
}

void EnemyBot::think(Game *game)
{
	if (state == BOT_TAKING_DAMAGE) {
		
		if (isFacingRight()) (BOT_STATE_TAKING_DAMAGE_RIGHT);
		else setCurrentState(BOT_STATE_TAKING_DAMAGE_LEFT);
		
		actionTime = 20;
		state = BOT_IDLE;
	}

	// Verify if is Dead
	if (isDead() && state != BOT_DYING) { // When Stops, verify if is dead
		state = BOT_DYING;
		
		if (isFacingRight()) setCurrentState(BOT_STATE_DYING_RIGHT);
		else setCurrentState(BOT_STATE_DYING_LEFT);

		botSpeed = 0;
		actionTime = 100;
	}

	if (state == BOT_ATTACKING && actionTime == 10)
		startAttack();

	if (actionTime == 0) { // Action Time Over, Make a Decision!

		// Search Player
		Point* p = searchPlayerPosition(game);
		int x = getTrueX(), y = getTrueY();

		if (state == BOT_CHASING) { // Try to find Player again, in a bigger range

			// if found in bigger range
			// continue chasing
			if (p->getX() > x - 300 && p->getX() < x + 300
			&& p->getY() > y - 200 && p->getY() < y + 200) {

				// if Close Attack !
				if (p->getX() > x - 60 && p->getX() < x + 60
					&& p->getY() > y - 50 && p->getY() < y + 50 && !isAttacking()) {

						botSpeed = 0;
						state = BOT_ATTACKING;
						
						if (x - p->getX() > 0) {
							setCurrentState(BOT_ATTACKING_LEFT);
							if (isFacingRight()) changeFacingDirection();
						} else {
							setCurrentState(BOT_ATTACKING_RIGHT);
							if (isFacingLeft()) changeFacingDirection();
						}


				} else {

				// else, chase...

					if (p->getX() < x) {
						setCurrentState(BOT_MOVING_LEFT);
						if (botSpeed > 0) botSpeed = -botSpeed;
						if (isFacingRight()) changeFacingDirection();
					} else {
						setCurrentState(BOT_MOVING_RIGHT);
						if (botSpeed < 0) botSpeed = -botSpeed;
						if (isFacingLeft()) changeFacingDirection();
					}
				}


				actionTime = 15;

			} else {
				state = BOT_MOVING;
			}


		} else if (p->getX() > x - 300 && p->getX() < x + 300
			&& p->getY() > y - 50 && p->getY() < y + 50) {

				// Found Player for the First Time
				botSpeed = getBotSpeedVelocity();
				setCurrentState(BOT_MOVING_RIGHT);
				if (p->getX() < x) {
					setCurrentState(BOT_MOVING_LEFT);
					botSpeed = -botSpeed;
				}

				state = BOT_CHASING;
				actionTime = 5;

		}


		if (state == BOT_IDLE) { // It WAS IDLE

			// Face Randomly
			if((rand() % 2) == 0)
				changeFacingDirection();

			botSpeed = getBotSpeedVelocity();

			if (isFacingLeft()) {
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
			actionTime = 50 + (rand() % 100);

		} else if (state == BOT_MOVING) { // It WAS MOVING

			// Stay IDLE for a little...

			if (isFacingLeft()) setCurrentState(BOT_IDLE_LEFT);
			else setCurrentState(BOT_IDLE_RIGHT);

			state = BOT_IDLE;
			botSpeed = 0;

			// For some time...
			actionTime = 100 + (rand() % 100);
		}

	} else {
		actionTime--;

		this->pp.setVelocity(botSpeed, 0);
	}
}

Point* EnemyBot::searchPlayerPosition(Game* game)
{
	int x, y;

	Player* p = dynamic_cast<Player*>(game->getGSM()->getSpriteManager()->getPlayer());

	x = p->getPhysicalProperties()->getX() + p->getBoundingVolume()->getX() +
		(p->getBoundingVolume()->getWidth() / 2);
	y = p->getPhysicalProperties()->getY() + p->getBoundingVolume()->getY() +
		(p->getBoundingVolume()->getHeight() / 2);

	return new Point(x, y);
}

EnemyBot* EnemyBot::clone()
{
	EnemyBot* newBot = new EnemyBot(getMaxHP(), getAttack(), velocity);
	newBot->setSpriteType(getSpriteType());

	newBot->getBoundingVolume()->setX(getBoundingVolume()->getX());
	newBot->getBoundingVolume()->setY(getBoundingVolume()->getY());
	newBot->getBoundingVolume()->setWidth(getBoundingVolume()->getWidth());
	newBot->getBoundingVolume()->setHeight(getBoundingVolume()->getHeight());

	return newBot;
}