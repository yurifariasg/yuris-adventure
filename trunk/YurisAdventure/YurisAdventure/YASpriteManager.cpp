#include "YASpriteManager.h"
#include "EnemyBot.h"
#include "PenetrableProjectile.h"

/*
	Creatures a YASpriteManager
*/
YASpriteManager::YASpriteManager()
{
	player = new Player(PLAYER_HP, PLAYER_MANA, PLAYER_ATTACK); // Initialize the Player
}

YASpriteManager::~YASpriteManager()
{
	delete player;
}

void YASpriteManager::update(Game* game)
{
	// Verify if some bot is dead
	list<Bot*>::iterator botIterator;
	botIterator = getBotsIterator();
	Bot* toRemove = NULL;
	while (botIterator != getEndOfBotsIterator())
	{
		EnemyBot *c = dynamic_cast<EnemyBot*>(*botIterator);

		if (c->isDead() && c->getActionTime() == 0 && c->getState() == BOT_DYING) {

			toRemove = c;
		} else toRemove = NULL;

		botIterator++;

		if (toRemove != NULL)
			removeBot(toRemove);
	}

	SpriteManager::update(game);
}

void YASpriteManager::unloadSprites()
{

	/*list<Bot*>::iterator it = getBotsIterator();
	Bot* prev = *(it);
	while (it != getEndOfBotsIterator())
	{
		it++;
		delete prev;
		prev = *(it);
	}
	clearBots();*/
	//layers = new vector<WorldLayer*>();


	SpriteManager::unloadSprites();
}

void YASpriteManager::reloadPlayer()
{
	player->reloadData();
}

void YASpriteManager::addProjectile(wstring type, int x, int y, bool facingRight, bool isPenetrating)
{
	SpriteManager::addProjectile(type, x, y, facingRight);

	list<Projectile*>::iterator endIterator = getEndOfProjectileIterator();
	endIterator--;
	dynamic_cast<PenetrableProjectile*>(*endIterator)->setPenetrable(isPenetrating);

}