#include "BossBot.h"



BossBot::~BossBot(void)
{
}

void BossBot::think(Game* game)
{
	if (state == BOT_TAKING_DAMAGE) {
		
		if (isFacingRight()) setCurrentState(BOT_STATE_TAKING_DAMAGE_RIGHT);
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
			if (p->getX() > x - 1000 && p->getX() < x + 1000
			&& p->getY() > y - 400 && p->getY() < y + 400) {

				int distanceXToAttack = getBoundingVolume()->getWidth() * 3;
				int distanceYToAttack = getBoundingVolume()->getHeight() * 2;

				// if Close Attack !
				if (p->getX() > x - distanceXToAttack &&
					p->getX() < x + distanceXToAttack &&
					p->getY() > y - distanceYToAttack &&
					p->getY() < y + distanceYToAttack &&
					!isAttacking()) {

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

			getPhysicalProperties()->setVelocity(botSpeed, 0);

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