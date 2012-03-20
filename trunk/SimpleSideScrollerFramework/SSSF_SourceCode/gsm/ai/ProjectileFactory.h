#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Projectile.h"

class ProjectileFactory
{
private:
	map<wstring, vector<Projectile*>*> recycledProjectiles;
	map<wstring, Projectile*> registeredProjectiles;

public:
	ProjectileFactory(void);
	~ProjectileFactory(void);
	void registerProjectile(wstring, Projectile*);
	Projectile* createProjectile(wstring);
	void recycleProjectile(wstring, Projectile*);

};

