#include "FightPhysics.h"
#include "EnemyBot.h"
#include "PenetrableProjectile.h"

void FightPhysics::updatePhysics(Game* game)
{
	Player* player = dynamic_cast<Player*>(game->getGSM()->getSpriteManager()->getPlayer());
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

		if (wasAttacking && !player->isAttacking()) {

			// Calculate Center of Player
			int attackingPointX = player->getPhysicalProperties()->getX() +
				player->getBoundingVolume()->getX() +
				(player->getBoundingVolume()->getWidth() / 2),
				attackingPointY = player->getPhysicalProperties()->getY() +
				player->getBoundingVolume()->getY() +
				(player->getBoundingVolume()->getHeight() / 2);

			// Put the Attacking Point in front of player...
			if (player->isFacingRight()) attackingPointX += player->getBoundingVolume()->getWidth() * 2;
			else attackingPointX -= player->getBoundingVolume()->getWidth() * 2;


			// Attack "Boxes"
			// is in front of creature and has 2 times creature width and 1 time creature height
			
			// Verify if it has collision
			if (hasAABBCollision(
				attackingPointX - player->getBoundingVolume()->getWidth(), attackingPointY -
				(player->getBoundingVolume()->getHeight()/2),
				player->getBoundingVolume()->getWidth() * 2, // Twice his Width
				player->getBoundingVolume()->getHeight(),
				bbX, bbY, bbWidth, bbHeight) && !dynamic_cast<EnemyBot*>(bot)->isDead()) {

				dynamic_cast<EnemyBot*>(bot)->takeDamage(player->getAttack());
				dynamic_cast<EnemyBot*>(bot)->setState(BOT_TAKING_DAMAGE);

				if (player->isFacingLeft()) dynamic_cast<EnemyBot*>(bot)->setBotSpeed(-PLAYER_SPEED);
				else dynamic_cast<EnemyBot*>(bot)->setBotSpeed(PLAYER_SPEED);

			}
		}

		botIterator++;
	}

	// Bots Against Player

	botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{
		EnemyBot *bot = dynamic_cast<EnemyBot*>(*botIterator);
		pp = player->getPhysicalProperties();
		int bbX = pp->getX() + player->getBoundingVolume()->getX(),
			bbY = pp->getY() + player->getBoundingVolume()->getY(),
			bbWidth = player->getBoundingVolume()->getWidth(),
			bbHeight = player->getBoundingVolume()->getHeight();

		if (bot->isAttacking() && !bot->isDead()) {

			int attackingPointX = bot->getPhysicalProperties()->getX() +
				bot->getBoundingVolume()->getX() +
				(bot->getBoundingVolume()->getWidth() / 2),
				attackingPointY = bot->getPhysicalProperties()->getY() +
				bot->getBoundingVolume()->getY() +
				(bot->getBoundingVolume()->getHeight() / 2);

			if (bot->isFacingRight()) attackingPointX += bot->getBoundingVolume()->getWidth() * 2;
			else attackingPointX -= bot->getBoundingVolume()->getWidth() * 2;


			if (hasAABBCollision(
				attackingPointX, attackingPointY,
				bot->getBoundingVolume()->getWidth() * 2, // Twice his Width
				bot->getBoundingVolume()->getHeight(),
				bbX, bbY, bbWidth, bbHeight) && !player->isDead()) {

				player->takeDamage(bot->getAttack());
				player->setTakingDamage(true);
				bot->stopAttack();				

				if (bot->isFacingRight()) player->setCurrentState(TAKING_DAMAGE_LEFT);
				else player->setCurrentState(TAKING_DAMAGE_RIGHT);

			}
		}

		botIterator++;
	}



	// Projectiles Against People...

	list<Projectile*>::iterator pIterator = sm->getProjectileIterator();
	while (pIterator != sm->getEndOfProjectileIterator())
	{
		Projectile *p = (*pIterator);
		pp = p->getPhysicalProperties();
		int bbX = pp->getX() + p->getBoundingVolume()->getX(),
			bbY = pp->getY() + p->getBoundingVolume()->getY(),
			bbWidth = p->getBoundingVolume()->getWidth(),
			bbHeight = p->getBoundingVolume()->getHeight();

		// First, Test Against Player..

		if (hasAABBCollision(
			player->getPhysicalProperties()->getX() + player->getBoundingVolume()->getX(),
			player->getPhysicalProperties()->getX() + player->getBoundingVolume()->getX(),
			player->getBoundingVolume()->getWidth(), player->getBoundingVolume()->getHeight(),
			bbX, bbY, bbWidth, bbHeight) && p->isEnabled()) {
			// Collision with Player
			
			PenetrableProjectile* pProjectile = dynamic_cast<PenetrableProjectile*>(p);

			if (pProjectile != NULL && pProjectile->isPenetrable()) {
				// Penetrable Projectile
				pProjectile->hit(player);

			} else {
				// Simple Projectile
				p->disables();
				player->takeDamage(p->getDamage());
			}


		}

		botIterator = sm->getBotsIterator();
		while (botIterator != sm->getEndOfBotsIterator())
		{
			EnemyBot *bot = dynamic_cast<EnemyBot*>(*botIterator);
			pp = bot->getPhysicalProperties();

			if (hasAABBCollision(
				pp->getX() + bot->getBoundingVolume()->getX(),
				pp->getY() + bot->getBoundingVolume()->getY(),
				bot->getBoundingVolume()->getWidth(), bot->getBoundingVolume()->getHeight(),
				bbX, bbY, bbWidth, bbHeight) && !bot->isDead() && p->isEnabled()) {

				PenetrableProjectile* pProjectile = dynamic_cast<PenetrableProjectile*>(p);
				EnemyBot* eBot = dynamic_cast<EnemyBot*>(bot);

				if (pProjectile != NULL && pProjectile->isPenetrable()) {
					// Penetrable Projectile
					pProjectile->hit(bot);

				} else {
					// Simple Projectile
					p->disables();
					dynamic_cast<EnemyBot*>(bot)->takeDamage(p->getDamage());
				}

				dynamic_cast<EnemyBot*>(bot)->setState(BOT_TAKING_DAMAGE);
				if (!p->isFacingRight()) dynamic_cast<EnemyBot*>(bot)->setBotSpeed(-PLAYER_SPEED);
				else dynamic_cast<EnemyBot*>(bot)->setBotSpeed(PLAYER_SPEED);

			}

			botIterator++;
		}

		Projectile* toRemove = NULL;

		if (!p->isEnabled()) toRemove = p;

		pIterator++;

		if (toRemove != NULL) sm->removeProjectile(toRemove);
	}

	if (player->isDead() && player->getDeathAnimationFinished() && !player->isAttacking()) {
		game->getGSM()->playerKilled(game);
		return;
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