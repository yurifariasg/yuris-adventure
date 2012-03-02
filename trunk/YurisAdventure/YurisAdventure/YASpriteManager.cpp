#include "YASpriteManager.h"

/*
	Creatures a YASpriteManager
*/
YASpriteManager::YASpriteManager()
{
	player = new Player(PLAYER_HP, PLAYER_ATTACK); // Initialize the Player
}