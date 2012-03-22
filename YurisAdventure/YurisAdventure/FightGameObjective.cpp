#include "FightGameObjective.h"


FightGameObjective::FightGameObjective(void)
{
	completed = false;
}


FightGameObjective::~FightGameObjective(void)
{
}

bool FightGameObjective::hasCompletedObjective()
{
	return completed;
}

void FightGameObjective::verifyObjective(Game* game)
{
	completed = game->getGSM()->getSpriteManager()->getNumberOfBots() == 0;
}