/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Viewport.h

	This class stores information about the viewport to help
	with rendering. The viewport may move around the game world
	by calling moveViewport, which changes viewportX and 
	viewportY. It also has information about the size of the 
	viewport. Note that the viewport width and height refer
	to the full rendering area of the game, including any GUI
	regions. To compute the size of the game rendering area,
	which might not include GUI toolbars, for example, one may
	subtract the viewport offsets from the width and height
	values.
*/
#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\game.h"

// Values in percentage !
const int FAST_SCROLLING_RIGHT = 70;
const int FAST_SCROLLING_LEFT = 30;
const int NORMAL_SCROLLING_RIGHT = 60;
const int NORMAL_SCROLLING_LEFT = 40;

class Viewport
{
private:
	int scrollSpeedX;
	int scrollSpeedY;
	int viewportX;
	int viewportY;
	int viewportWidth;
	int viewportHeight;
	int viewportOffsetX;
	int viewportOffsetY;

public:
	// INLINED ACCESSOR METHODS
	int getScrollSpeedX()		{ return scrollSpeedX;			}
	int getScrollSpeedY()		{ return scrollSpeedY;			}
	int getViewportX()			{ return viewportX;				}
	int getViewportY()			{ return viewportY;				}
	int getViewportWidth()		{ return viewportWidth;			}
	int getViewportHeight()		{ return viewportHeight;		}
	int getViewportOffsetX()	{ return viewportOffsetX;		}
	int getViewportOffsetY()	{ return viewportOffsetY;		}

	// INLINED MUTATOR METHODS
	void setScrollSpeedX(int initScrollSpeedX)
	{ scrollSpeedX = initScrollSpeedX;		}
	void setScrollSpeedY(int initScrollSpeedY)
	{ scrollSpeedY = initScrollSpeedY;		}
	void setViewportX(int initViewportX)				
	{ viewportX = initViewportX;			}
	void setViewportY(int initViewportY)				
	{ viewportY = initViewportY;			}
	void setViewportWidth(int initViewportWidth)
	{ viewportWidth = initViewportWidth;	}
	void setViewportHeight(int initViewportHeight)
	{ viewportHeight = initViewportHeight;	}
	void setViewportOffsetX(int initViewportOffsetX)	
	{ viewportOffsetX = initViewportOffsetX;}
	void setViewportOffsetY(int initViewportOffsetY)	
	{ viewportOffsetY = initViewportOffsetY;}

	// METHODS DEFINED IN Viewport.cpp
	Viewport();
	~Viewport();
	bool areWorldCoordinatesInViewport(float x, float y, int width, int height);
	bool areViewportCoordinatesInViewport(int x, int y, int width, int height);
	void moveViewport(int incX, int incY,	int worldWidth, int worldHeight);
	void update(Game *game);
};