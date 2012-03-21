#pragma once
#include "stdafx.h"

/*
	YAGame.h

	This file contains setup data for making the game. Note that nearly
	all of this data would better serve us if loaded from files.
*/

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
static const wstring LEVEL_FILE = L"data/Levels.txt";


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

// PROPERTIES FOR LEVELS FILES
static const wstring LEVEL_COUNT = L"NUMBER_OF_LEVELS";
static const wstring LEVEL_NUM_OF_COLUMNS = L"LEVEL_NUM_COLUMNS";
static const wstring LEVEL_NUM_OF_ROWS = L"LEVEL_NUM_ROWS";
static const wstring LEVEL_BG_PATH = L"LEVEL_BG_PATH";
static const wstring LEVEL_WORLD_PATH = L"LEVEL_WORLD_PATH";
static const wstring LEVEL_ELEMENT_PATH = L"LEVEL_ELEMENT_PATH";

static const wstring LEVEL_NUMBER_OF_ELEMENTS = L"NUMBER_OF_ELEMENTS";
static const wstring ELEMENT_TILE_PATH = L"ELEMENT_TILE_PATH";
static const wstring ELEMENT_CODE = L"ELEMENT_CODE";

static const wstring EMPTY_TILE = L"textures/world/tiles/empty_tile.png";


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

// INGame GUI Images
static const wstring HP_BAR_PIECE	= L"textures/gui/overlays/hp_piece.png";
static const int HP_BAR_MAX = 30;
static const wstring MANA_BAR_PIECE	= L"textures/gui/overlays/mana_piece.png";
static const int MANA_BAR_MAX = 30;


// FOR OUR SPRITE
static const wstring	IDLE_STATE_RIGHT = L"IDLE_STATE_RIGHT";
static const wstring	IDLE_STATE_LEFT = L"IDLE_STATE_LEFT";
static const wstring	MOVING_RIGHT_STATE = L"MOVING_RIGHT_STATE";
static const wstring	MOVING_LEFT_STATE = L"MOVING_LEFT_STATE";
static const wstring	ATTACKING_RIGHT_STATE = L"SWORD_ATTACK_RIGHT";
static const wstring	ATTACKING_LEFT_STATE = L"SWORD_ATTACK_LEFT";
static const wstring	JUMPING_STATE_RIGHT = L"JUMPING_STATE_RIGHT";
static const wstring	JUMPING_STATE_LEFT = L"JUMPING_STATE_LEFT";
static const wstring	DYING_STATE_RIGHT = L"DYING_STATE_RIGHT";
static const wstring	DYING_STATE_LEFT = L"DYING_STATE_LEFT";
static const wstring	CHARGING_STATE_RIGHT = L"CHARGING_STATE_RIGHT";
static const wstring	CHARGING_STATE_LEFT = L"CHARGING_STATE_LEFT";
static const wstring	CASTING_STATE_RIGHT = L"CASTING_STATE_RIGHT";
static const wstring	CASTING_STATE_LEFT = L"CASTING_STATE_LEFT";

// BOTs STATES
static const wstring	BOT_IDLE_RIGHT = L"BOT_IDLE_RIGHT";
static const wstring	BOT_IDLE_LEFT = L"BOT_IDLE_LEFT";
static const wstring	BOT_MOVING_RIGHT = L"BOT_MOVING_RIGHT";
static const wstring	BOT_MOVING_LEFT = L"BOT_MOVING_LEFT";
static const wstring	BOT_ATTACKING_RIGHT = L"BOT_ATTACKING_RIGHT";
static const wstring	BOT_ATTACKING_LEFT = L"BOT_ATTACKING_LEFT";
static const wstring	BOT_STATE_TAKING_DAMAGE_LEFT = L"BOT_TAKING_DAMAGE_LEFT";
static const wstring	BOT_STATE_TAKING_DAMAGE_RIGHT = L"BOT_TAKING_DAMAGE_RIGHT";
static const wstring	BOT_STATE_DYING_LEFT = L"BOT_DYING_LEFT";
static const wstring	BOT_STATE_DYING_RIGHT = L"BOT_DYING_RIGHT";

// Projectile Types

static const wstring	PROJECTILE_MAGIC = L"MAGIC_PROJECTILE";

// Combo Images

static const wstring	ATTACK_INCREASED_IMAGE = L"ATTACK_INCREASED_IMAGE";
static const wstring	ATTACK_INCREASED_IMAGE_PATH = L"textures/gui/overlays/attack_increased_combo.png";

static const wstring	MAGIC_PENETRATION_IMAGE = L"MAGIC_PENETRATION_IMAGE";
static const wstring	MAGIC_PENETRATION_IMAGE_PATH = L"textures/gui/overlays/magic_penetration_image.png";

static const wstring	DEFENSE_INCREASE_IMAGE = L"DEFENSE_INCREASE_IMAGE";
static const wstring	DEFENSE_INCREASE_IMAGE_PATH = L"textures/gui/overlays/defense_increased_image.png";


// Keys
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
const unsigned int SWORD_ATTACK_KEY = (unsigned int) 'J';
const unsigned int MAGIC_ATTACK_KEY = (unsigned int) 'K';
const unsigned int ACTION_KEY = (unsigned int) 'L';
const unsigned int UP_KEY = VK_UP;
const unsigned int DOWN_KEY = VK_DOWN;
const unsigned int LEFT_KEY = VK_LEFT;
const unsigned int RIGHT_KEY = VK_RIGHT;
const unsigned int ENTER_KEY = VK_RETURN;

static const int		ANIM_SPEED = 6;
static const int		PLAYER_WIDTH = 64;
static const int		PLAYER_HEIGHT = 92;

// FOR OUR WORLD
static const int		TILE_WIDTH = 64;
static const int		TILE_HEIGHT = 64;
static const int		Z_LAYER = 0;

static const int PLAYER_HP = 100;
static const int PLAYER_MANA = 100;
static const int PLAYER_ATTACK = 50;

// Player Speed declared in Game.h