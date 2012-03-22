#pragma once
#include "stdafx.h"

#include "SSSF_SourceCode\game\Game.h"

class ObjectiveSeeker
{
public:
	virtual void verifyObjective(Game*) = 0;
	virtual bool hasCompletedObjective() = 0;
};