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
#include "SSSF_SourceCode\game\Game.h"
//#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
//#include "SSSF_SourceCode\gsm\world\TiledLayer.h"



const float DEFAULT_MAX_VELOCITY = 100.0f;
const float DEFAULT_GRAVITY = 4.0f;

class AnimatedSprite;
class TiledLayer;
class Viewport;

class Physics
{
private:
	// USED TO APPLY TO GAME OBJECTS EACH FRAME
	float gravity;

	// THINK OF THIS AS THE WORLD'S TERMINAL VELOCITY
	float maxVelocity;

	void doStaticPhysics(AnimatedSprite*, TiledLayer*, Viewport*);
	

public:
	// INLINED METHODS
	float	getGravity()							{ return gravity;					}
	float	getMaxVelocity()						{ return maxVelocity;				}
	void	setGravity(float initGravity)			{ gravity = initGravity;			}
	void	setMaxVelocity(float initMaxVelocity)	{ maxVelocity = initMaxVelocity;	}
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
	void update(Game *game);

};