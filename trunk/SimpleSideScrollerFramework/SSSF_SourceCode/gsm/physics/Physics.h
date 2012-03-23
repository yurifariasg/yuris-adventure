/*	
	Author: _______________

	Physics.h

	This class will be used to manage all game collisions
	and physics. This includes collision detection, and resolution.
	Each frame we will update the collided objects velocities and
	positions accordingly.

	Students will implement this as part of their projects.
*/

#pragma once
#include "stdafx.h"
#include "EnchancedPhysics.h"
#include "SSSF_SourceCode\game\Game.h"

const float DEFAULT_MAX_VELOCITY = 100.0f;
const float DEFAULT_GRAVITY = 4.0f;
const bool IS_GRAVITY_ACTIVE = true;
const double AIR_VELOCITY_MULTIPLIER = 2;

class AnimatedSprite;
class TiledLayer;
class Viewport;
class Projectile;
class SpriteManager;

class Physics
{
private:
	// USED TO APPLY TO GAME OBJECTS EACH FRAME
	float gravity;

	// THINK OF THIS AS THE WORLD'S TERMINAL VELOCITY
	float maxVelocity;

	EnchancedPhysics* physicsE;

	void doStaticPhysics(AnimatedSprite*, TiledLayer*, Viewport*);
	void doProjectilePhysics(Projectile*, TiledLayer*, Viewport*);
	void doCollisionPhysicsBetween(AnimatedSprite*, AnimatedSprite*);

public:
	// INLINED METHODS
	float	getGravity()							{ return gravity;					}
	float	getMaxVelocity()						{ return maxVelocity;				}
	void	setGravity(float initGravity)			{ gravity = initGravity;			}
	void	setMaxVelocity(float initMaxVelocity)	{ maxVelocity = initMaxVelocity;	}
	void	setEnchancedPhysics(EnchancedPhysics* e) { physicsE = e; }
	int divideSafe(int i, int j) {

		double division = (double) i / (double) j;
		
		int intVersion = (int) division;
		double remainder = division - intVersion;


		if (remainder >= 0.9) { return intVersion; }
		else return intVersion;
	
	}

	// METHODS TO BE DEFINED IN Physics.cpp
	Physics();
	~Physics();
	static bool hasAABBCollision(int,int,int,int,int,int,int,int);
	void update(Game *game);
	void doDynamicPhysics(SpriteManager*);
	double firstTimeCollision(int,int,int,int,int,int,int,int,int,int,int,int);
	bool findOnArray(vector<pair<int,int>*>*, pair<int,int>*);
	bool occupiesCell(AnimatedSprite*, pair<int,int>*);

};