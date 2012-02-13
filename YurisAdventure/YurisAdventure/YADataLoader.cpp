#include "stdafx.h"

// DUMMY GAME INCLUDES
#include "YAButtonEventHandler.h"
#include "YADataLoader.h"
#include "YAGame.h"
#include "YAKeyEventHandler.h"
#include "YATextGenerator.h"

// GAME OBJECT INCLUDES
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\gsm\state\GameState.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"
#include "SSSF_SourceCode\gui\Cursor.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\gui\ScreenGUI.h"
#include "SSSF_SourceCode\input\GameInput.h"
#include "SSSF_SourceCode\os\GameOS.h"
#include "SSSF_SourceCode\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsOS.h"
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsInput.h"
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

// DIRECTX INCLUDES
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXGraphics.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"

/*
	loadGame - This method loads the setup game data into the game and
	constructs all the needed objects for the game to work.
*/
void YADataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[DG_TITLE];
	wstring useFullscreenProp = (*properties)[DG_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[DG_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[DG_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *dummyOS = new WindowsOS(	hInstance, 
										nCmdShow,
										useFullscreen,
										titleProp,
										screenWidth, screenHeight,
										game);
	
	int textFontSize;
	wstring textFontSizeProp = (*properties)[DG_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *dummyGraphics = new DirectXGraphics(game);
	dummyGraphics->init(screenWidth, screenHeight);
	dummyGraphics->initGraphics(dummyOS->getWindowHandle(), useFullscreen);
	dummyGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(dummyGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *dummyInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *dummyTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins(	(GameGraphics*)dummyGraphics,
								(GameInput*)dummyInput,
								(GameOS*)dummyOS,
								(GameTimer*)dummyTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	YATextGenerator *dummyTextGenerator = new YATextGenerator();
	dummyTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(DG_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)dummyTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);	

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

/*
	initColors - this helper method loads the color key, used for loading
	images, and the font color, used for rendering text.
*/
void YADataLoader::initColors(	GameGraphics *graphics,
									map<wstring,wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[DG_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[DG_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[DG_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[DG_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[DG_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[DG_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
	loadGUI - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void YADataLoader::loadGUI(Game *game, wstring guiInitFile)
{
    game->getText()->writeDebugOutput("LoadGUI\n");
	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	hardCodedLoadGUIExample(game);

    game->getGUI()->addScreenGUI(GS_MAIN_MENU, loadGUIFromFile(game, guiInitFile));

    /*gui->addScreenGUI(GS_MAIN_MENU,		mainMenuGUI);

    map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, guiInitFile);

    int imageQuantity, screenHeight;
	wstring screenWidthProp = (*properties)[DG_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[DG_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;*/




}

/*
	loadLevel - This method should load the data the level described by the
	levelInitFile argument in to the Game's game state manager.
*/
void YADataLoader::loadWorld(Game *game, wstring levelInitFile)	
{
	// NOTE:	I AM DEMONSTRATING HOW TO LOAD A LEVEL
	//			PROGRAMICALLY. YOU SHOULD DO THIS
	//			USING CSV FILES.
	hardCodedLoadLevelExample(game);
}

/*
	initDummyGUI - This method builds a GUI for the Dummy Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void YADataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	//initMainMenu(gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
}

/*
	initCursor - initializes a simple little cursor for the gui.
*/
void YADataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(DG_GREEN_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	imageID = guiTextureManager->loadTexture(DG_RED_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);
}

/*
	initSplashScreen - initializes the game's splash screen gui.
*/
void YADataLoader::initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(DG_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							game->getGraphics()->getScreenWidth(),
							game->getGraphics()->getScreenHeight(),
							false,
							DG_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
*/
void YADataLoader::initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(DG_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
    imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

    
    unsigned int logoID;

    // Add Clouds
    logoID = guiTextureManager->loadTexture(CLOUDS);
	OverlayImage *cloudToAdd = new OverlayImage();
	cloudToAdd->x = 0;
	cloudToAdd->y = 0;
	cloudToAdd->z = 0;
	cloudToAdd->alpha = 255;
    cloudToAdd->width = 2024;
	cloudToAdd->height = 430;
	cloudToAdd->imageID = logoID;
	mainMenuGUI->addOverlayImage(cloudToAdd);

    // Add the Logo
    logoID = guiTextureManager->loadTexture(MM_LOGO_PATH);
	OverlayImage *logoToAdd = new OverlayImage();
	logoToAdd->x = 250;
	logoToAdd->y = 290;
	logoToAdd->z = 0;
	logoToAdd->alpha = 255;
    logoToAdd->width = 750;
	logoToAdd->height = 100;
	logoToAdd->imageID = logoID;
	mainMenuGUI->addOverlayImage(logoToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
    int normalTextureID;
    int selectedTextureID;

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

    normalTextureID = guiTextureManager->loadTexture(MM_START_PATH);
    selectedTextureID = guiTextureManager->loadTexture(MM_START_SELECT_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							selectedTextureID, // Doesnt Change when mouse over
							40,
							420,
							0,
							255,
							240,
							40,
							false,
							DG_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

    // AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

    // - GET THE BUTTON COMMAND AND IMAGE IDs

    normalTextureID = guiTextureManager->loadTexture(MM_CONTROLS_PATH);
    selectedTextureID = guiTextureManager->loadTexture(MM_CONTROLS_SELECT_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							selectedTextureID, // Doesnt Change when mouse over
							40,
							420 + 15 + 40,
							0,
							255,
							200,
							40,
							false,
							DG_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

    // AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

    // - GET THE BUTTON COMMAND AND IMAGE IDs

    normalTextureID = guiTextureManager->loadTexture(MM_ABOUT_PATH);
    selectedTextureID = guiTextureManager->loadTexture(MM_ABOUT_SELECT_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							selectedTextureID,
							40,
							420 + 40 + 15 + 40 + 15,
							0,
							255,
							140,
							40,
							false,
							DG_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

    // AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

    normalTextureID = guiTextureManager->loadTexture(MM_EXIT_GAME_PATH);
    selectedTextureID = guiTextureManager->loadTexture(MM_EXIT_GAME_SELECT_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							selectedTextureID,
							40,
							420 + 40 + 15 + 40 + 15 + 40 + 15, // Initial Pos + Above Height + Margin
							0,
							255,
							210,
							40,
							false,
							DG_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU,		mainMenuGUI);
}

/*
	initInGameGUI - initializes the game's in-game gui.
*/
void YADataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int normalTextureID = guiTextureManager->loadTexture(DG_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(DG_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							200,
							100,
							false,
							DG_QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS,	inGameGUI);
}

/*
	initViewport - initializes the game's viewport.
*/
void YADataLoader::initViewport(GameGUI *gui, map<wstring,wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[DG_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[DG_VIEWPORT_OFFSET_Y];
	wstring screenWidthProp = (*properties)[DG_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[DG_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
}

/*
	loadLevelExample - This method loads an example level via a hard-coded
	approach. This can be useful for testing code, but is not a practical
	solution. Game levels should be loaded from files.
*/
void YADataLoader::hardCodedLoadLevelExample(Game *game)
{
	// FIRST SETUP THE GAME WORLD DIMENSIONS
	GameStateManager *gsm = game->getGSM();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *worldTextureManager = graphics->getWorldTextureManager();

	// NOTE THAT THE WORLD WILL BE THE SAME DIMENSIONS AS OUR TILED LAYER,
	// WE COULD GET A PARALLAX SCROLLING EFFECT IF WE MADE A LAYER SMALLER,
	// AND THEN SCROLLED IT SLOWER
	World *world = gsm->getWorld();
	world->setWorldWidth(NUM_COLUMNS * TILE_WIDTH);
	world->setWorldHeight(NUM_ROWS * TILE_HEIGHT);

	// NOW LOAD OUR TILED BACKGROUND
	TiledLayer *tiledLayer = new TiledLayer(	NUM_COLUMNS,	NUM_ROWS, 
												TILE_WIDTH,		TILE_HEIGHT, 
												0, true, 
												NUM_COLUMNS * TILE_WIDTH,
												NUM_ROWS * TILE_HEIGHT);
	int grassID = worldTextureManager->loadTexture(GRASS_IMAGE_PATH);
	int wallID = worldTextureManager->loadTexture(WALL_IMAGE_PATH);
	srand(1);

	// LET'S GENERATE A RANDOM BACKGROUND USING OUR TWO TILES
	for (int i = 0; i < (NUM_COLUMNS * NUM_ROWS); i++)
	{
		bool isCollidable = false;
		int tileIDToUse = grassID;
		int randomInt = rand() % 100;
		if (randomInt >= 50)
			isCollidable = true;

		randomInt = rand() % 100;
		if (randomInt >= 80)
			tileIDToUse = wallID;

		Tile *tileToAdd = new Tile();
		tileToAdd->collidable = isCollidable;
		tileToAdd->textureID = tileIDToUse;
		tiledLayer->addTile(tileToAdd);
	}

	// BUT LET'S CLEAR OUT THE TOP-LEFT AREA
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Tile *tile = tiledLayer->getTile(i,j);
			tile->textureID = grassID;
		}
	}	

	world->addLayer(tiledLayer);

	// AND NOW LET'S MAKE A MAIN CHARACTER SPRITE
	AnimatedSpriteType *ast = new AnimatedSpriteType();
	int spriteImageID0 = worldTextureManager->loadTexture(PLAYER_IDLE0_PATH);
	int spriteImageID1 = worldTextureManager->loadTexture(PLAYER_IDLE1_PATH);
	int spriteImageID2 = worldTextureManager->loadTexture(PLAYER_IDLE2_PATH);

	// SIZE OF SPRITE IMAGES
	ast->setTextureSize(PLAYER_WIDTH, PLAYER_HEIGHT);

	// NOW LET'S ADD AN ANIMATION STATE
	// FIRST THE NAME
	ast->addAnimationSequence(IDLE_STATE);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID0, 50);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID1, 20);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID2, 40);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID0, 100);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID1, 20);

	SpriteManager *spriteManager = gsm->getSpriteManager();
	unsigned int spriteTypeID = spriteManager->addSpriteType(ast);
	ast->setSpriteTypeID(spriteTypeID);

	AnimatedSprite *player = spriteManager->getPlayer();
	player->setSpriteType(ast);
	PhysicalProperties *playerProps = player->getPhysicalProperties();
	playerProps->setX(10);
	playerProps->setY(10);
	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);

	// WE WILL SET LOTS OF OTHER PROPERTIES ONCE
	// WE START DOING COLLISIONS AND PHYSICS

	player->setAlpha(255);
	player->setCurrentState(IDLE_STATE);
}

ScreenGUI* YADataLoader::loadGUIFromFile(Game *game, wstring guiFile)
{
    map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, guiFile);

    // Initialize All Variables
    int imageQuantity, optionQuantity, x, y, width, height;
    wstring imageQuantityS, optionQuantityS, xS, yS, widthS, heightS, path, pathUnselected, pathSelected, animated;

    imageQuantityS = (*properties)[GUI_QUANTITY_OF_IMAGES];
    wstringstream(imageQuantityS) >> imageQuantity;

    optionQuantityS = (*properties)[GUI_QUANTITY_OF_OPTIONS];
    wstringstream(optionQuantityS) >> optionQuantity;


    // Texture Stuff
    GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();
    unsigned int imageID, imageID2;

    // Initialize screen
    ScreenGUI* screen = new ScreenGUI();

    // Add Images
    for ( int i = 1 ; i <= imageQuantity ; i++) {

        wstringstream stream;

        // Get Size and Position
        stream << i << GUI_IMAGE_WIDTH;       
        widthS = (*properties)[stream.str()];
        wstringstream(widthS) >> width;
        stream.str(L"");

        stream << i << GUI_IMAGE_HEIGHT;       
        heightS = (*properties)[stream.str()];
        wstringstream(heightS) >> height;
        stream.str(L"");

        stream << i << GUI_IMAGE_X;       
        xS = (*properties)[stream.str()];
        wstringstream(xS) >> x;
        stream.str(L"");

        stream << i << GUI_IMAGE_Y;       
        yS = (*properties)[stream.str()];
        wstringstream(yS) >> y;
        stream.str(L"");

        // Path and Animated

        stream << i << GUI_IMAGE_PATH;
        path = stream.str();
        stream.str(L"");

        stream << i << GUI_IMAGE_ANIMATED;
        animated = stream.str();
        stream.str(L"");

        // Create and Add Image

        imageID = guiTextureManager->loadTexture((*properties)[path]);
	    OverlayImage *imageToAdd = new OverlayImage();
	    imageToAdd->x = x;
	    imageToAdd->y = y;
	    imageToAdd->z = 0;
	    imageToAdd->alpha = 255;
        imageToAdd->width = width;
        imageToAdd->height = height;
	    imageToAdd->imageID = imageID;
        screen->addOverlayImage(imageToAdd);

    }

    Button *buttonToAdd;

    for (int i = 1 ; i <= optionQuantity ; i++) {

        wstringstream stream;

        // Get Size and Position
        stream << i << GUI_OPTION_WIDTH;       
        widthS = (*properties)[stream.str()];
        wstringstream(widthS) >> width;
        stream.str(L"");

        stream << i << GUI_OPTION_HEIGHT;       
        heightS = (*properties)[stream.str()];
        wstringstream(heightS) >> height;
        stream.str(L"");

        stream << i << GUI_OPTION_X;       
        xS = (*properties)[stream.str()];
        wstringstream(xS) >> x;
        stream.str(L"");

        stream << i << GUI_OPTION_Y;       
        yS = (*properties)[stream.str()];
        wstringstream(yS) >> y;
        stream.str(L"");

        // Path and Animated

        stream << i << GUI_OPTION_PATH_UNSELECTED;
        pathUnselected = (*properties)[stream.str()];
        stream.str(L"");

        stream << i << GUI_OPTION_PATH_SELECTED;
        pathSelected = (*properties)[stream.str()];
        stream.str(L"");


        buttonToAdd = new Button();

	    // - GET THE BUTTON COMMAND AND IMAGE IDs

        imageID = guiTextureManager->loadTexture(pathUnselected);
        imageID2 = guiTextureManager->loadTexture(pathSelected);

	    // - INIT THE START BUTTON
	    buttonToAdd->initButton(imageID, 
							    imageID2, // Doesnt Change when mouse over
							    x,
							    y,
							    0,
							    255,
                                width,
                                height,
							    false,
                                DG_EXIT_COMMAND);

	    // AND NOW LOAD IT INTO A ScreenGUI
	    screen->addButton(buttonToAdd);
    }

    return screen;
}

