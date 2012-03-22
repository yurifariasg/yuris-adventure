/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.cpp

	See SpriteManager.h for a class description.
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSpriteType.h"
#include "SSSF_SourceCode\gsm\sprite\SpriteManager.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"

/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	PhysicalProperties *pp = sprite->getPhysicalProperties();

	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	
									pp->getX(),
									pp->getY(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(	sprite->getCurrentImageID(),
									pp->round(pp->getX()-viewport->getViewportX()),
									pp->round(pp->getY()-viewport->getViewportY()),
									pp->round(pp->getZ()),
									sprite->getAlpha(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight());
	}
}

/*
	addSpriteItemsToRenderList - This method goes through all of the sprites,
	including the player sprite, and adds the visible ones to the render list.
	This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = (*botIterator);
			addSpriteToRenderList(bot, renderList, viewport);
			botIterator++;
		}

		// ADD THE PLAYER SPRITE
		addSpriteToRenderList(getPlayer(), renderList, viewport);

		// NOW ADD THE REST OF THE SPRITES
		list<Projectile*>::iterator pIterator;
		pIterator = projectiles.begin();
		while (pIterator != projectiles.end())
		{			
			Projectile *p = (*pIterator);
			addSpriteToRenderList(p, renderList, viewport);
			pIterator++;
		}

		// NOW ADD THE REST OF THE SPRITES
		list<AnimatedSprite*>::iterator aoIterator;
		aoIterator = animatedObjects.begin();
		while (aoIterator != animatedObjects.end())
		{			
			AnimatedSprite *as = (*aoIterator);
			addSpriteToRenderList(as, renderList, viewport);
			aoIterator++;
		}
	}
}

/*
	addSprite - This method is for adding a new sprite to 
	this sprite manager. Once a sprite is added it can be 
	scheduled for rendering.
*/
void SpriteManager::addBot(Bot *botToAdd)
{
	bots.push_back(botToAdd);
}

/*
	addSpriteType - This method is for adding a new sprite
	type. Note that one sprite type can have many sprites. For
	example, we may say that there may be a "Bunny" type of
	sprite, and specify properties for that type. Then there might
	be 100 different Bunnies each with their own properties, but that
	share many things in common according to what is defined in
	the shared sprite type object.
*/
unsigned int SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd)
{
	spriteTypes.push_back(spriteTypeToAdd);
	return spriteTypes.size()-1;
}

/*
	clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	spriteTypes.clear();
	bots.clear();
	projectiles.clear();
	animatedObjects.clear();
}

/*
	getSpriteType - This gets the sprite type object that corresponds
	to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(unsigned int typeIndex)
{
	if (typeIndex < spriteTypes.size())
		return spriteTypes.at(typeIndex);
	else
		return NULL;
}

/*
	unloadSprites - This method removes all artwork from memory that
	has been allocated for game sprites.
*/
void SpriteManager::unloadSprites()
{

	// NOW ADD THE REST OF THE SPRITES
	/*list<AnimatedSprite*>::iterator aoIterator;
	aoIterator = animatedObjects.begin();
	while (aoIterator != animatedObjects.end())
	{			
		AnimatedSprite *as = (*aoIterator);
		aoIterator++;
		delete as;
	}

	// NOW ADD THE REST OF THE SPRITES
	list<Projectile*>::iterator pIterator;
	pIterator = projectiles.begin();
	while (pIterator != projectiles.end())
	{			
		Projectile *p = (*pIterator);
		pIterator++;
		delete p;
	}

	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		botIterator++;
		delete bot;
	}*/

	clearSprites();
}

Bot* SpriteManager::removeBot(Bot *botToRemove)
{

	bots.remove(botToRemove);

	//delete botToRemove;
	return NULL;
}

/*
	update - This method should be called once per frame. It
	goes through all of the sprites, including the player, and calls their
	update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{
	// UPDATE THE PLAYER SPRITE
	getPlayer()->updateSprite();

	// NOW UPDATE THE REST OF THE SPRITES
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		bot->updateSprite();
		bot->think(game);
		botIterator++;
	}

	// NOW ADD THE REST OF THE SPRITES
	list<Projectile*>::iterator pIterator;
	pIterator = projectiles.begin();
	while (pIterator != projectiles.end())
	{			
		Projectile *p = (*pIterator);
		p->updateSprite();
		pIterator++;
	}

	// NOW ADD THE REST OF THE SPRITES
	list<AnimatedSprite*>::iterator aoIterator;
	aoIterator = animatedObjects.begin();
	while (aoIterator != animatedObjects.end())
	{			
		AnimatedSprite *as = (*aoIterator);
		as->updateSprite();
		aoIterator++;
	}
}

void SpriteManager::addProjectile(wstring type, int x, int y, bool facingRight)
{
	Projectile* p = pFactory.createProjectile(type);

	p->getPhysicalProperties()->setX(x);
	p->getPhysicalProperties()->setY(y);
	p->setFacingDirectionRight(facingRight);
	p->setAlpha(255);

	projectiles.push_back(p);
}

void SpriteManager::registerProjectile(Projectile* p)
{
	pFactory.registerProjectile(p->getType(), p);
}

void SpriteManager::removeProjectile(Projectile* projectileToRemove)
{
	projectiles.remove(projectileToRemove);
	pFactory.recycleProjectile(projectileToRemove->getType(), projectileToRemove);
}