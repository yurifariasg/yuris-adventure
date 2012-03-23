#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Projectile.h"
/*
	Projectile Factory, recycles and creates projectiles
*/
class ProjectileFactory
{
private:
	map<wstring, vector<Projectile*>*> recycledProjectiles;
	map<wstring, Projectile*> registeredProjectiles;

public:
	ProjectileFactory() {};
	~ProjectileFactory();
	void registerProjectile(wstring, Projectile*);
	Projectile* createProjectile(wstring);
	void recycleProjectile(wstring, Projectile*);

};

