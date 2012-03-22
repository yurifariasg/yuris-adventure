#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Projectile.h"
#include "Creature.h"


class PenetrableProjectile : public Projectile
{
private:
	bool penetrable;
	vector<Creature*> creaturesHit;

public:
	PenetrableProjectile(wstring, int, int);
	~PenetrableProjectile(void);

	bool isPenetrable() { return penetrable; }
	void setPenetrable(bool newPenetrable) { penetrable = newPenetrable; }

	void hit(Creature*);

	void disables();

	PenetrableProjectile* clone();
};

