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
}

/*
	Noting to destroy at the moment.
*/
Physics::~Physics()
{

}

void Physics::update(Game *game)
{
	// REMEMBER, AT THIS POINT, ALL PLAYER INPUT AND AI
	// HAVE ALREADY BEEN PROCESSED AND BOT AND PLAYER
	// STATES, VELOCITIES, AND ACCELERATIONS HAVE ALREADY
	// BEEN UPDATED. NOW WE HAVE TO PROCESS THE PHYSICS
	// OF ALL THESE OBJECTS INTERACTING WITH EACH OTHER
	// AND THE STATIC GAME WORLD. THIS MEANS WE NEED TO
	// DETECT AND RESOLVE COLLISIONS IN THE ORDER THAT
	// THEY WILL HAPPEN, AND WITH EACH COLLISION, EXECUTE
	// ANY GAMEPLAY RESPONSE CODE, UPDATE VELOCITIES, AND
	// IN THE END, UPDATE POSITIONS

	// FIRST, YOU SHOULD START BY ADDING ACCELERATION TO ALL 
	// VELOCITIES, WHICH INCLUDES GRAVITY, NOTE THE EXAMPLE
	// BELOW DOES NOT DO THAT


	// FOR NOW, WE'LL JUST ADD THE VELOCITIES TO THE
	// POSITIONS, WHICH MEANS WE'RE NOT APPLYING GRAVITY OR
	// ACCELERATION AND WE ARE NOT DOING ANY COLLISION 
	// DETECTION OR RESPONSE
	GameStateManager *gsm = game->getGSM();
	SpriteManager *sm = gsm->getSpriteManager();
	//AnimatedSprite *player;
	PhysicalProperties *pp;

	doStaticPhysics(sm->getPlayer(), (TiledLayer*) (gsm->getWorld()->getLayers()->at(0)), game->getGUI()->getViewport());

	// FOR NOW THE PLAYER IS DIRECTLY CONTROLLED BY THE KEYBOARD,
	// SO WE'LL NEED TO TURN OFF ANY VELOCITY APPLIED BY INPUT
	// SO THE NEXT FRAME IT DOESN'T GET ADDED
	//pp->setVelocity(0.0f, 0.0f);

	// AND NOW MOVE ALL THE BOTS
	list<Bot*>::iterator botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{			
		Bot *bot = (*botIterator);
		pp = bot->getPhysicalProperties();
		//pp->setPosition(pp->getX() + pp->getVelocityX(), pp->getY() + pp->getVelocityY());
		doStaticPhysics(bot, (TiledLayer*) (gsm->getWorld()->getLayers()->at(0)), game->getGUI()->getViewport());
		botIterator++;
	}
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

		if (leftColumn < 0 || rightColumn > world->getLayerWidth() / CELL_WIDTH) {

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

			} else {

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

	if (tile == NULL) { // Didnt Collide
		pp->setPosition(newPlayerX, pp->getY());
	}

	pp->setVelocity(0,0);


	// Ending of Big Function
}