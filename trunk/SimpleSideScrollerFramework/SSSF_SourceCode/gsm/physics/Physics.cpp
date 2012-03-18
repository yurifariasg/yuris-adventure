#include "stdafx.h"
#include "Physics.h"
#include <cmath>

#pragma once

#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\SpriteManager.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"
#include "SSSF_SourceCode\gui\Viewport.h"

/*
	Default constructor, it initializes all data using default values.
*/
Physics::Physics()
{
	maxVelocity = DEFAULT_MAX_VELOCITY;
	gravity = DEFAULT_GRAVITY;
	physicsE = NULL;
}

/*
	Noting to destroy at the moment.
*/
Physics::~Physics()
{
	delete physicsE;
}

void Physics::update(Game *game)
{
	GameStateManager *gsm = game->getGSM();
	SpriteManager *sm = gsm->getSpriteManager();
	PhysicalProperties *pp;

	doStaticPhysics(sm->getPlayer(), (TiledLayer*) (gsm->getWorld()->getLayers()->at(0)), game->getGUI()->getViewport());

	// AND NOW MOVE ALL THE BOTS
	list<Bot*>::iterator botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{			
		Bot *bot = (*botIterator);
		pp = bot->getPhysicalProperties();
		doStaticPhysics(bot, (TiledLayer*) (gsm->getWorld()->getLayers()->at(0)), game->getGUI()->getViewport());
		botIterator++;
	}

	// Dynamic Physics
	if (physicsE != NULL) physicsE->updatePhysics(game);
}

void Physics::doStaticPhysics(AnimatedSprite* sprite, TiledLayer* world, Viewport* viewport)
{
	// Stating of Big Functio

	//player = sm->getPlayer();
	PhysicalProperties* pp = sprite->getPhysicalProperties();

	if (!viewport->areWorldCoordinatesInViewport(
		pp->getX(), pp->getY(), sprite->getBoundingVolume()->getWidth(), sprite->getBoundingVolume()->getHeight())) {
		return;
	}

	// Add Acelerations
	pp->setVelocity(pp->getVelocityX() + pp->getAccelerationX(),
		pp->getVelocityY() + pp->getAccelerationY());

	const int CELL_WIDTH = 64;
	const int CELL_HEIGHT = 64;

	//pp->setVelocity(10, 0);

	// Lets Add the Gravity

	if (true) { // has Gravity ?

		// New BoundingBox Y
		int newBBY = (pp->getY() + sprite->getBoundingVolume()->getY()) + pp->getVelocityY() + getGravity();

		// See Collisions under it

		int bbW = sprite->getBoundingVolume()->getWidth();
		
		// Bottom, Right and Left Cell
		int topRow = (newBBY) / CELL_HEIGHT;
		int bottomRow = (newBBY + sprite->getBoundingVolume()->getHeight() ) / CELL_HEIGHT;
		int rightColumn = ((pp->getX() + sprite->getBoundingVolume()->getX()) + sprite->getBoundingVolume()->getWidth() ) / CELL_WIDTH;
		int leftColumn = (pp->getX() + sprite->getBoundingVolume()->getX()) / CELL_WIDTH;

		//int worldWidth = world->getLayerWidth();

		if (leftColumn < 0 || rightColumn > world->getWorldWidth() / CELL_WIDTH) {

			// Invalid Position

			// Cancel Velocity and ends
			pp->setVelocity(0,0);

			return;
		}
		
		int currentBottomCell = leftColumn;
		int affectedCellsSize = abs(leftColumn - rightColumn) + 1;
		int *affectedCells = new int[affectedCellsSize];
		int p = 0;

		while (currentBottomCell <= rightColumn) {
			affectedCells[p++] = currentBottomCell++;
		}

		Tile* tile;

		bool collided = false;

		for (p = 0 ; p < affectedCellsSize ; p++) {
			// Verify for each bottom cell if they collide...

			// FIX THE MAGIC NUMBERS HERE
			tile = world->getTile(bottomRow, affectedCells[p]);

			if (tile->collidable) {
				collided = true;
				break;
			}

			tile = world->getTile(topRow, affectedCells[p]);

			if (tile->collidable) {
				collided = true;
				break;
			}

		}

		if (collided) {

			// Doesnt Move !

			if (pp->getVelocityY() > 0) {

				pp->setVelocity(pp->getVelocityX(), 0);
				pp->setAccelerationY(0);
				pp->setBuoyancy(false);

			} else if (pp->getVelocityY() < 0) {

				// Lose all aceleration hit top...
				pp->setAccelerationY(0);

			}

		} else {

			// Ok to move..
			pp->setPosition(pp->getX(), newBBY - sprite->getBoundingVolume()->getY());

			// Gravity Commands...
			if (pp->getAccelerationY() < getGravity())
				pp->setAccelerationY(pp->getAccelerationY() + getGravity());

		}
	}

	// Move Player

	// Now Lets Verify the Left and Right Border...

	int newPlayerX = pp->getX() + (pp->getVelocityX() * (pp->getBuoyancy() ? 1.5 : 1));

	// Top Cell
	int topRow = divideSafe((pp->getY() + sprite->getBoundingVolume()->getY()), CELL_HEIGHT);
	int bottomRow = divideSafe(((pp->getY() + sprite->getBoundingVolume()->getY()) + sprite->getBoundingVolume()->getHeight() ), CELL_HEIGHT);
	int leftColumn = divideSafe(newPlayerX + sprite->getBoundingVolume()->getX(),  CELL_WIDTH);
	int rightColumn = divideSafe(((newPlayerX + sprite->getBoundingVolume()->getX()) + sprite->getBoundingVolume()->getWidth() ), CELL_WIDTH);

	int *affectedCells = new int[bottomRow - topRow + 1];
	int currentCell = topRow;
	int p = 0;

	while (currentCell <= bottomRow) {
		affectedCells[p++] = currentCell++;
	}

	Tile* tile = NULL;

	for (p = 0 ; p < bottomRow - topRow + 1; p++) {

		// Left Cell
		tile = world->getTile(affectedCells[p], leftColumn);
		if (tile->collidable) { // Collided
			break;
		}

		// Right Cell
		tile = world->getTile(affectedCells[p], rightColumn);
		if (tile->collidable) { // Collided
			break;
		}
		tile = NULL;
	}

	if (tile == NULL
			&& newPlayerX + sprite->getBoundingVolume()->getX() > 0
			&& newPlayerX + sprite->getBoundingVolume()->getX()
			+ sprite->getBoundingVolume()->getWidth() < world->getWorldWidth()) {
				// Dont go to invalid position

		pp->setPosition(newPlayerX, pp->getY());
	}

	pp->setVelocity(0,0);
	// Ending of Big Function
}