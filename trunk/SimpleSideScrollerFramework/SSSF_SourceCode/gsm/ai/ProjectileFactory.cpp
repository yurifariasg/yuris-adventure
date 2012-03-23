#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\ProjectileFactory.h"

ProjectileFactory::~ProjectileFactory(void)
{
	registeredProjectiles.clear();
	recycledProjectiles.clear();
}

void ProjectileFactory::registerProjectile(wstring type, Projectile* p)
{
	registeredProjectiles[type] = p;
	recycledProjectiles[type] = new vector<Projectile*>();

}

Projectile* ProjectileFactory::createProjectile(wstring type)
{

	vector<Projectile*>* list = recycledProjectiles[type];

	if (list->empty()) {

		return registeredProjectiles[type]->clone();

	} else {
		Projectile* p = list->back();
		list->pop_back();
		p->enables();
		return p;
	}
}

void ProjectileFactory::recycleProjectile(wstring type, Projectile* p)
{
	recycledProjectiles[type]->push_back(p);
}