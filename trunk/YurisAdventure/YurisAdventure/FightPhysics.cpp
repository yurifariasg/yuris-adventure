#include "FightPhysics.h"
#include "EnemyBot.h"

void FightPhysics::updatePhysics(Game* game)
{
	Player* player = (Player*) game->getGSM()->getSpriteManager()->getPlayer();
	GameStateManager *gsm = game->getGSM();
	SpriteManager *sm = gsm->getSpriteManager();
	PhysicalProperties *pp;

	list<Bot*>::iterator botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{
		Bot *bot = (*botIterator);
		pp = bot->getPhysicalProperties();
		int bbX = pp->getX() + bot->getBoundingVolume()->getX(),
			bbY = pp->getY() + bot->getBoundingVolume()->getY(),
			bbWidth = bot->getBoundingVolume()->getWidth(),
			bbHeight = bot->getBoundingVolume()->getHeight();
		Bot* toRemove = NULL;

		if (wasAttacking && !player->isAttacking()) {

			int attackingPointX = player->getPhysicalProperties()->getX() +
				player->getBoundingVolume()->getX() +
				(player->getBoundingVolume()->getWidth() / 2),
				attackingPointY = player->getPhysicalProperties()->getY() +
				player->getBoundingVolume()->getY() +
				(player->getBoundingVolume()->getHeight() / 2);

			if (player->isFacingRight()) attackingPointX += 30;
			else attackingPointX -= 30;


			if (hasAABBCollision(
				attackingPointX, attackingPointY, 20, 20,
				bbX, bbY, bbWidth, bbHeight)) {

				dynamic_cast<EnemyBot*>(bot)->takeDamage(10);
				dynamic_cast<EnemyBot*>(bot)->setState(BOT_TAKING_DAMAGE);

				if (player->isFacingLeft()) dynamic_cast<EnemyBot*>(bot)->setBotSpeed(-PLAYER_SPEED);
				else dynamic_cast<EnemyBot*>(bot)->setBotSpeed(PLAYER_SPEED);

			}

			if (dynamic_cast<EnemyBot*>(bot)->isDead()) {
				toRemove = bot;
			} else {
				toRemove = NULL;
			}


		}

		botIterator++;
		if (toRemove != NULL)
			sm->removeBot(toRemove);
	}

	wasAttacking = player->isAttacking();
}

bool FightPhysics::hasAABBCollision(int x1,int y1, int width1, int height1,int x2,int y2, int width2, int height2)
{
	bool collisionX = false, collisionY = false;

	if (x2 > x1 && x2 < x1 + width1 ||
		x1 > x2 && x1 < x2 + width2) collisionX = true;

	if (y2 > y1 && y2 < y1 + height1 ||
		y1 > y2 && y1 < y2 + height2) collisionY = true;

	return collisionX && collisionY;
}