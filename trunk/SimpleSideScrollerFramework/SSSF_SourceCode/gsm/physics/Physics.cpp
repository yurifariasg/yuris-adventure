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

	list<Projectile*>::iterator pIterator = sm->getProjectileIterator();
	while (pIterator != sm->getEndOfProjectileIterator())
	{			
		Projectile *p = (*pIterator);
		pp = p->getPhysicalProperties();
		doProjectilePhysics(p, (TiledLayer*) (gsm->getWorld()->getLayers()->at(0)), game->getGUI()->getViewport());
		pIterator++;
	}

	// Continuous Collision System
	//doDynamicPhysics(sm);

	// Fight Physics
	if (physicsE != NULL) physicsE->updatePhysics(game);
}

void Physics::doStaticPhysics(AnimatedSprite* sprite, TiledLayer* world, Viewport* viewport)
{
	// Starting of Big Function
	PhysicalProperties* pp = sprite->getPhysicalProperties();

	// Add Acelerations
	pp->setVelocity(pp->getVelocityX() + pp->getAccelerationX(),
		pp->getVelocityY() + pp->getAccelerationY());

	// Lets Add the Gravity

	if (IS_GRAVITY_ACTIVE) {

		// New BoundingBox Y
		int newBBY = (pp->getY() + sprite->getBoundingVolume()->getY()) + pp->getVelocityY() + getGravity();

		// See Collisions under it

		int bbW = sprite->getBoundingVolume()->getWidth();
		
		// Bottom, Right and Left Cell
		int topRow = (newBBY) / CELL_HEIGHT;
		int bottomRow = (newBBY + sprite->getBoundingVolume()->getHeight() ) / CELL_HEIGHT;
		int rightColumn = ((pp->getX() + sprite->getBoundingVolume()->getX()) + sprite->getBoundingVolume()->getWidth() ) / CELL_WIDTH;
		int leftColumn = (pp->getX() + sprite->getBoundingVolume()->getX()) / CELL_WIDTH;

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

			pp->setBuoyancy(true);

			// Gravity Commands...
			if (pp->getAccelerationY() < getGravity())
				pp->setAccelerationY(pp->getAccelerationY() + getGravity());

		}
	}

	// Move Player

	// Now Lets Verify the Left and Right Border...

	int newPlayerX = pp->getX() + (pp->getVelocityX() * (pp->getBuoyancy() ? AIR_VELOCITY_MULTIPLIER : 1));

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

void Physics::doProjectilePhysics(Projectile* sprite, TiledLayer* world, Viewport* viewport)
{


	PhysicalProperties* pp = sprite->getPhysicalProperties();

	if (!viewport->areWorldCoordinatesInViewport(
		pp->getX(), pp->getY(), sprite->getBoundingVolume()->getWidth(), sprite->getBoundingVolume()->getHeight())) {
		return;
	}

	// Add Acelerations
	pp->setVelocity(pp->getVelocityX() + pp->getAccelerationX(),
		pp->getVelocityY() + pp->getAccelerationY());

	// Move Player

	// Now Lets Verify the Left and Right Border...

	int newPlayerX = pp->getX() + (pp->getVelocityX() * (pp->getBuoyancy() ? AIR_VELOCITY_MULTIPLIER : 1));

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
	} else {
		sprite->disables();
	}

	pp->setVelocity(0,0);
	// Ending of Big Function

}

void Physics::doCollisionPhysicsBetween(AnimatedSprite* as1, AnimatedSprite* as2)
{
	/*int as1X = as1->getPhysicalProperties()->getX() + as1->getBoundingVolume()->getX(),
		as1Y = as1->getPhysicalProperties()->getY() + as1->getBoundingVolume()->getY(),
		as1Width = as1->getBoundingVolume()->getWidth(),
		as1Height = as1->getBoundingVolume()->getHeight(),
		as1VelX = as1->getPhysicalProperties()->getVelocityX(),
		as1VelY = as1->getPhysicalProperties()->getVelocityY(),

		as2X = as2->getPhysicalProperties()->getX() + as2->getBoundingVolume()->getX(),
		as2Y = as2->getPhysicalProperties()->getY() + as2->getBoundingVolume()->getY(),
		as2Width = as2->getBoundingVolume()->getWidth(),
		as2Height = as2->getBoundingVolume()->getHeight(),
		as2VelX = as2->getPhysicalProperties()->getVelocityX(),
		as2VelY = as2->getPhysicalProperties()->getVelocityY();

	// Continuous System

	if (as1VelX == 0 && as1VelY == 0 &&
		as2VelX == 0 && as2VelY == 0) {

	} else {
		// THey re Moving !

		// Lets give theirs velocities and verify collision
		if (hasAABBCollision(as1X + as1VelX, as1Y + as1VelY, as1Width, as1Height,
			as2X + as2VelY, as2Y + as2VelY, as2Width, as2Height)) {
				// theres collision, we need to modify their velocities..

				// Lets Verify Collision on X Axis:

				int fCollision = firstTimeCollision(as1X, as2X, as1Width, as2Width, as1VelX, as2VelY);
				



		} else {
			// no collision, doesnt do a thing..
		}
	}*/


}

void Physics::doDynamicPhysics(SpriteManager* sm)
{
	AnimatedSprite* player = sm->getPlayer();
	int playerX = player->getPhysicalProperties()->getX() + player->getBoundingVolume()->getX(),
		playerY = player->getPhysicalProperties()->getY() + player->getBoundingVolume()->getY(),
		playerWidth = player->getBoundingVolume()->getWidth(),
		playerHeight = player->getBoundingVolume()->getHeight(),
		playerVelX = player->getPhysicalProperties()->getVelocityX(),
		playerVelY = player->getPhysicalProperties()->getVelocityY();

	// Get Cell that Player Occupies !
	// Analysis Phase
	vector<pair<int, int>*> occupiedCells;

	for (int i = 0 ; i <= abs(playerVelX / CELL_WIDTH) + 1 ; i++ ) {

		// Bottom, Right and Left Cell
		int topRow = (playerY) / CELL_HEIGHT;
		int bottomRow = (playerY + playerHeight ) / CELL_HEIGHT;

		int rightColumn = (playerX + playerWidth + (i * CELL_WIDTH))  / CELL_WIDTH;
		int leftColumn = (playerX + (i * CELL_WIDTH)) / CELL_WIDTH;

		if ( i == 0 ) {// get the left border till before the right

			for (int k = leftColumn ; k < rightColumn ; k++) {

				for (int j = topRow ; j <= bottomRow ; j++ ) { // From top to bottom...

					pair<int,int>* cell = new pair<int,int>(k, j);

					if (!findOnArray(&occupiedCells, cell))
						occupiedCells.push_back(cell);

				}
			}

		}

		// get the right border...
		for (int j = topRow ; j <= bottomRow ; j++ ) { // From top to bottom...

			pair<int,int>* cell = new pair<int,int>(rightColumn, j);

			if (!findOnArray(&occupiedCells, cell))
				occupiedCells.push_back(cell);

		}

	}

	// For each bot.. verify if it occupies a player's cell..
	// Filter Phase

	vector<Bot*> collidableBots;

	list<Bot*>::iterator currentBot = sm->getBotsIterator();
	while(currentBot != sm->getEndOfBotsIterator())
	{
		for (int i = 0 ; i < occupiedCells.size() ; i++) {
			if (occupiesCell((*currentBot), occupiedCells.at(i))) {
				collidableBots.push_back((*currentBot));

				currentBot++;
				continue;
			}
		}

		currentBot++;
	}

	// Now: "Find the first contact time between each object pair"

	vector<Bot*>::iterator cBot = collidableBots.begin();
	while (cBot != collidableBots.end())
	{
		Bot* b = (*cBot);

		double moveTo = 0;

		while (moveTo < 1) {
			// Find first contact time between them..
			
			// If they're moving...
			if (b->getPhysicalProperties()->
				getVelocityX() > 0 && b->getPhysicalProperties()->
				getVelocityY() > 0) {

			double moveTo = 
				firstTimeCollision(playerX, b->getPhysicalProperties()->getX() + b->getBoundingVolume()->getX(),
				playerY, b->getPhysicalProperties()->getY() + b->getBoundingVolume()->getY(),
				playerHeight, b->getBoundingVolume()->getHeight(),
				playerWidth, b->getBoundingVolume()->getWidth(),
				playerVelX, b->getPhysicalProperties()->getVelocityX(),
				playerVelY, b->getPhysicalProperties()->getVelocityY());

			//(int x1,int x2, int y1, int y2, int height1, int height2,
	//int width1, int width2, int v1, int v2)

			// Place them on the first collision velocity...
			if (moveTo > 1) moveTo = 1;

			playerVelX *= moveTo;
			playerVelY *= moveTo;
			b->getPhysicalProperties()->
				setVelocity(
				b->getPhysicalProperties()->getVelocityX() * moveTo,
				b->getPhysicalProperties()->getVelocityY() * moveTo);
			}



			

		}
		cBot++;
	}

	player->getPhysicalProperties()->setVelocity(playerVelX, playerVelY);
	
}

bool Physics::hasAABBCollision(int x1,int y1, int width1, int height1,int x2,int y2, int width2, int height2)
{
	bool collisionX = false, collisionY = false;

	if (x2 > x1 && x2 < x1 + width1 ||
		x1 > x2 && x1 < x2 + width2) collisionX = true;

	if (y2 > y1 && y2 < y1 + height1 ||
		y1 > y2 && y1 < y2 + height2) collisionY = true;

	return collisionX && collisionY;
}

double Physics::firstTimeCollision(int x1,int x2, int y1, int y2, int height1, int height2,
	int width1, int width2, int vx1, int vx2, int vy1, int vy2)

{
	double tx_first_contact = 0;
	double tx_last_contact = 0;

	if (x2 > x1 + width1) {// Esquerda de B eh MAIOR q a direita de A

		tx_first_contact = (x2 - x1 + width1) / (vx1 - vx2);
		tx_last_contact = ((x2 + width2) - x1) / (vx1 - vx2);

	} else {
		tx_first_contact = (x1 - x2 + width2) / (vx2 - vx1);
		tx_last_contact = ((x1 + width1) - x2) / (vx2 - vx1);
	}

	double ty_first_contact = 0;
	double ty_last_contact = 0;

	if (y2 > y1 + height1) {

		ty_first_contact = (y2 - y1 + height1) / (vy1 - vy2);
		ty_last_contact = ((y2 + height2) - y1) / (vy1 - vy2);

	} else {

		ty_first_contact = (y1 - y2 + height2) / (vy2 - vy1);
		ty_last_contact = ((y1 + height1) - y2) / (vy2 - vy1);
	}

	if (tx_first_contact > ty_first_contact && tx_first_contact < ty_last_contact) {
		// tx first inside y
		return tx_first_contact > 1 ? 1 : tx_first_contact;

	} else if (ty_first_contact > tx_first_contact && ty_first_contact < tx_last_contact) {
		// ty inside x
		return ty_first_contact > 1 ? 1: ty_first_contact;

	} else {
		return 1;
	}
}

bool Physics::findOnArray(vector<pair<int,int>*>* v, pair<int,int>* elemnt)
{
	vector<pair<int,int>*>::iterator currentElement = v->begin();

	while (currentElement != v->end())
	{
		if ((*currentElement) == elemnt) return true;

		currentElement++;
	}

	return false;

}

bool Physics::occupiesCell(AnimatedSprite* as, pair<int,int>* cell)
{
	int topRow = (as->getPhysicalProperties()->getY()) / CELL_HEIGHT;
	int bottomRow = (as->getPhysicalProperties()->getY() + as->getBoundingVolume()->getHeight() ) / CELL_HEIGHT;
	int rightColumn = ((as->getPhysicalProperties()->getX() + as->getBoundingVolume()->getX()) + as->getBoundingVolume()->getWidth() ) / CELL_WIDTH;
	int leftColumn = (as->getPhysicalProperties()->getX() + as->getBoundingVolume()->getX()) / CELL_WIDTH;

	if (leftColumn <= cell->first && cell->first <= rightColumn
		&& topRow <= cell->second && cell->second <= bottomRow)
		return true;
	else return false;

}