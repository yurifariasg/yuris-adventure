/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.h

	This class manages all of the sprites in a given game level. Note
	that the player sprite is also managed by this class.
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\ai\BotRecycler.h"
#include "SSSF_SourceCode\gsm\ai\ProjectileFactory.h"
#include "SSSF_SourceCode\gsm\ai\Projectile.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSpriteType.h"

class SpriteManager
{
private:
	// NOTE THAT MULTIPLE SPRITES MAY SHARE ARTWORK, SO SPRITE TYPES
	// SPECIFIES A TYPE OF SPRITE, OF WHICH THERE MAY BE MANY INSTANCES
	vector<AnimatedSpriteType*> spriteTypes;

	// THESE ARE THE BOTS IN THE GAME, LIKE ENEMIES, ROCKETS, OR ANYTHING
	// THAT MOVES AROUND AND IS NOT THE PLAYER
	list<Bot*> bots;

	// Animated Objects that doesnt move
	list<AnimatedSprite*> animatedObjects;

	// Active Projectiles
	list<Projectile*> projectiles;

	// AND THIS IS THE PLAYER. AS-IS, WE ONLY ALLOW FOR ONE PLAYER AT A TIME
	//AnimatedSprite player;

	// THE BotRecycler MAKES SURE WE DON'T HAVE TO CONSTRUCT BOTS WHENEVER
	// WE NEED TO SPAWN THEM, INSTEAD IT WILL RECYCLE THEM FOR US
	BotRecycler recyclableBots;

	// Projectile Factory
	ProjectileFactory pFactory;

public:
	// NOTHING TO INIT OR DESTROY
	SpriteManager()		{}
	~SpriteManager()	{ clearSprites(); }

	// INLINED ACCESSOR METHODS
	int						getNumberOfSprites()	{ return bots.size();		}
	virtual AnimatedSprite*			getPlayer() = 0;
	virtual void reloadPlayer() = 0;
	list<Bot*>::iterator	getBotsIterator()		{ return bots.begin();		}
	list<Bot*>::iterator	getEndOfBotsIterator()	{ return bots.end();		}
	int getNumberOfBots() { return bots.size(); }

	list<Projectile*>::iterator	getProjectileIterator()		{ return projectiles.begin();		}
	list<Projectile*>::iterator	getEndOfProjectileIterator()	{ return projectiles.end();		}

	// METHODS DEFINED IN SpriteManager.cpp
	void				addBot(Bot *botToAdd);
	void				addSpriteItemsToRenderList(Game *game);
	unsigned int		addSpriteType(AnimatedSpriteType *spriteTypeToAdd);
	void				addSpriteToRenderList(AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport);
	void				clearSprites();
	AnimatedSpriteType* getSpriteType(unsigned int typeIndex);
	Bot*				removeBot(Bot *botToRemove);
	void clearBots() { bots.clear(); }
	virtual void				unloadSprites();
	virtual void				update(Game *game);
	void addProjectile(wstring, int, int, bool);
	void registerProjectile(Projectile*);
	void removeProjectile(Projectile* projectileToRemove);

	void addAnimatedObject(AnimatedSprite* as) { animatedObjects.push_back(as); }
};