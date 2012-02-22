#pragma once
#include "stdafx.h"

/*
	DummyGame.h

	This file contains setup data for making the dummy game. Note that nearly
	all of this data would better serve us if loaded from files.
*/

// HERE IS THE DUMMY GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
static const wstring	DG_INIT_FILE			= L"data/GameInit.txt";
static const wstring	DG_GUI_INIT_FILE		= L"data/MainMenuGUI.txt";
static const string		DG_DEBUG_FILE			= "DebugOutput.txt";
static const wstring mainMenuControlsGUIFile = L"data/MainMenuControlsGUI.txt";
static const wstring mainMenuAboutGUIFile = L"data/MainMenuAboutGUI.txt";
static const wstring pausedGameFile = L"data/PauseGUI.txt";
static const wstring controlsGameFile = L"data/InGameControlsGUI.txt";
static const wstring aboutGameFile = L"data/InGameAboutGUI.txt";
static const wstring splashscreenFile = L"data/SplashScreenGUI.txt";
static const wstring inGameFile = L"data/InGameGUI.txt";


// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const wstring	DG_TITLE				= L"TITLE";
static const wstring	DG_SCREEN_WIDTH			= L"SCREEN_WIDTH";
static const wstring	DG_SCREEN_HEIGHT		= L"SCREEN_HEIGHT";
static const wstring	DG_USE_FULLSCREEN_MODE	= L"USE_FULLSCREEN_MODE";
static const wstring	DG_TEXT_FONT_SIZE		= L"TEXT_FONT_SIZE";
static const wstring	DG_VIEWPORT_OFFSET_X	= L"VIEWPORT_OFFSET_X";
static const wstring	DG_VIEWPORT_OFFSET_Y	= L"VIEWPORT_OFFSET_Y";
static const wstring	DG_FONT_COLOR_RED		= L"FONT_COLOR_RED";
static const wstring	DG_FONT_COLOR_GREEN		= L"FONT_COLOR_GREEN";
static const wstring	DG_FONT_COLOR_BLUE		= L"FONT_COLOR_BLUE";
static const wstring	DG_COLOR_KEY_RED		= L"COLOR_KEY_RED";
static const wstring	DG_COLOR_KEY_GREEN		= L"COLOR_KEY_GREEN";
static const wstring	DG_COLOR_KEY_BLUE		= L"COLOR_KEY_BLUE";

// PROPERTIES FOR THE MAIN MENU GUI
static const wstring GUI_QUANTITY_OF_IMAGES = L"QUANTITY_OF_IMAGES";
static const wstring GUI_IMAGE_PATH = L"IMAGE_PATH";
static const wstring GUI_IMAGE_WIDTH = L"IMAGE_WIDTH";
static const wstring GUI_IMAGE_HEIGHT = L"IMAGE_HEIGHT";
static const wstring GUI_IMAGE_X = L"IMAGE_X";
static const wstring GUI_IMAGE_Y = L"IMAGE_Y";
static const wstring GUI_IMAGE_ANIMATED = L"IMAGE_ANIMATED";

static const wstring GUI_QUANTITY_OF_OPTIONS = L"QUANTITY_OF_OPTIONS";
static const wstring GUI_OPTION_PATH_UNSELECTED = L"OPTION_PATH_UNSELECTED";
static const wstring GUI_OPTION_PATH_SELECTED = L"OPTION_PATH_SELECTED";
static const wstring GUI_OPTION_WIDTH = L"OPTION_WIDTH";
static const wstring GUI_OPTION_HEIGHT = L"OPTION_HEIGHT";
static const wstring GUI_OPTION_X = L"OPTION_X";
static const wstring GUI_OPTION_Y = L"OPTION_Y";
static const wstring GUI_OPTION_COMMAND = L"OPTION_COMMAND";

// CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
static const wstring DG_GO_TO_MM_COMMAND	= L"Go To Main Menu";
static const wstring DG_START_COMMAND		= L"Start";
static const wstring DG_EXIT_COMMAND		= L"Exit";
static const wstring DG_QUIT_COMMAND		= L"Quit";
static const wstring DG_GREEN_CURSOR_PATH	= L"textures/gui/cursor/green_cursor.png";
static const wstring DG_RED_CURSOR_PATH		= L"textures/gui/cursor/red_cursor.png";
static const wstring DG_SPLASH_SCREEN_PATH	= L"textures/gui/overlays/splash_screen.png";
static const wstring DG_QUIT_IMAGE_PATH		= L"textures/gui/buttons/quit.png";
static const wstring DG_QUIT_IMAGE_MO_PATH	= L"textures/gui/buttons/quit_mo.png";


// EXAMPLE LEVEL DATA FOR THE HARD-CODED loadLevelExample

// FOR BACKGROUND TILES
static const wstring	GRASS_IMAGE_PATH = L"textures/world/tiles/grass.png";
static const wstring	WALL_IMAGE_PATH = L"textures/world/tiles/wall.png";

// FOR OUR SPRITE
static const wstring	IDLE_STATE_RIGHT = L"IDLE_STATE_RIGHT";
static const wstring	IDLE_STATE_LEFT = L"IDLE_STATE_LEFT";
static const wstring	MOVING_RIGHT_STATE = L"MOVING_RIGHT_STATE";
static const wstring	MOVING_LEFT_STATE = L"MOVING_LEFT_STATE";
static const wstring	PLAYER_IDLE_RIGHT1_PATH = L"textures/world/sprites/mainchar/idle_right1.png";
static const wstring	PLAYER_IDLE_RIGHT2_PATH = L"textures/world/sprites/mainchar/idle_right2.png";
static const wstring	PLAYER_IDLE_LEFT1_PATH = L"textures/world/sprites/mainchar/idle_left1.png";
static const wstring	PLAYER_IDLE_LEFT2_PATH = L"textures/world/sprites/mainchar/idle_left2.png";
static const wstring	PLAYER_MOVING_RIGHT1_PATH = L"textures/world/sprites/mainchar/moving_right_1.png";
static const wstring	PLAYER_MOVING_RIGHT2_PATH = L"textures/world/sprites/mainchar/moving_right_2.png";
static const wstring	PLAYER_MOVING_RIGHT3_PATH = L"textures/world/sprites/mainchar/moving_right_3.png";
static const wstring	PLAYER_MOVING_LEFT1_PATH = L"textures/world/sprites/mainchar/moving_left_1.png";
static const wstring	PLAYER_MOVING_LEFT2_PATH = L"textures/world/sprites/mainchar/moving_left_2.png";
static const wstring	PLAYER_MOVING_LEFT3_PATH = L"textures/world/sprites/mainchar/moving_left_3.png";

static const int		ANIM_SPEED = 6;
static const int		PLAYER_WIDTH = 64;
static const int		PLAYER_HEIGHT = 92;

// FOR OUR WORLD
static const int		NUM_COLUMNS = 30;
static const int		NUM_ROWS = 12;
static const int		TILE_WIDTH = 64;
static const int		TILE_HEIGHT = 64;
static const int		Z_LAYER = 0;

// Player Speed declared in Game.h