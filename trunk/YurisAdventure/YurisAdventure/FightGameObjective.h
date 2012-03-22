#pragma once
#include "SSSF_SourceCode\gsm\world\ObjectiveSeeker.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"

class FightGameObjective :
	public ObjectiveSeeker
{
private:
	bool completed;

public:
	FightGameObjective();
	~FightGameObjective();

	bool hasCompletedObjective();
	void verifyObjective(Game*);
};

