#include "stdafx.h"

// ya GAME INCLUDES
#include "YAButtonEventHandler.h"
#include "YADataLoader.h"
#include "YAGame.h"
#include "YAKeyEventHandler.h"
#include "YATextGenerator.h"
#include "EnemyBot.h"
#include "PlayerGUI.h"
#include "ImageShower.h"
#include "PenetrableProjectile.h"
#include "FightGameObjective.h"

#include "SSSF_SourceCode\text\BufferedTextFileReader.h"

// GAME OBJECT INCLUDES
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\gsm\state\GameState.h"
#include "SSSF_SourceCode\gsm\ai\RandomFloatingBot.h"
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
	WindowsOS *yaOS = new WindowsOS(	hInstance, 
										nCmdShow,
										useFullscreen,
										titleProp,
										screenWidth, screenHeight,
										game);
	
	int textFontSize;
	wstring textFontSizeProp = (*properties)[DG_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *yaGraphics = new DirectXGraphics(game);
	yaGraphics->init(screenWidth, screenHeight);
	yaGraphics->initGraphics(yaOS->getWindowHandle(), useFullscreen);
	yaGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(yaGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *yaInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *yaTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins(	(GameGraphics*)yaGraphics,
								(GameInput*)yaInput,
								(GameOS*)yaOS,
								(GameTimer*)yaTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	YATextGenerator *yaTextGenerator = new YATextGenerator();
	yaTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(DG_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)yaTextGenerator);

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
	// Load From File all the GUI
    game->getGUI()->addScreenGUI(GS_MAIN_MENU, loadGUIFromFile(game, guiInitFile, new ScreenGUI()));
    game->getGUI()->addScreenGUI(GS_MENU_CONTROLS_MENU, loadGUIFromFile(game, mainMenuControlsGUIFile, new ScreenGUI()));
    game->getGUI()->addScreenGUI(GS_MENU_ABOUT_MENU, loadGUIFromFile(game, mainMenuAboutGUIFile, new ScreenGUI()));
	game->getGUI()->addScreenGUI(GS_PAUSED, loadGUIFromFile(game, pausedGameFile, new ScreenGUI()));
	game->getGUI()->addScreenGUI(GS_IN_GAME_CONTROLS, loadGUIFromFile(game, controlsGameFile, new ScreenGUI()));
	game->getGUI()->addScreenGUI(GS_IN_GAME_ABOUT, loadGUIFromFile(game, aboutGameFile, new ScreenGUI()));
	game->getGUI()->addScreenGUI(GS_SPLASH_SCREEN, loadGUIFromFile(game, splashscreenFile, new ScreenGUI()));
	game->getGUI()->addScreenGUI(GS_STARTING_LEVEL, loadGUIFromFile(game, splashscreenFile, new ScreenGUI()));

	// History Screen
	FadeScreen* fScreen = new FadeScreen();
	loadStoryBoard(game, fScreen);
	game->getGUI()->addScreenGUI(GS_STORY_BOARD, fScreen);

	fScreen = new FadeScreen();
	loadEndGame(game, fScreen);
	game->getGUI()->addScreenGUI(GS_END_GAME, fScreen);

	// InGame GUI
	PlayerGUI* pGUI = new PlayerGUI();
	game->getGUI()->addScreenGUI(GS_GAME_IN_PROGRESS, pGUI);

	loadInGameGUI(game, pGUI);

	// Init Cursor (Gives error if removed) - FIX
	initCursor(game->getGUI(), (DirectXTextureManager*)game->getGraphics()->getGUITextureManager());
}

/*
	loadLevel - This method should load the data the level described by the
	levelInitFile argument in to the Game's game state manager.
*/
void YADataLoader::loadWorld(Game *game, int levelNumber)	
{
	// FIRST SETUP THE GAME WORLD DIMENSIONS
	GameStateManager *gsm = game->getGSM();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *worldTextureManager = graphics->getWorldTextureManager();

	// Set Viewport to initial pos
	game->getGUI()->getViewport()->setViewportX(0);

	// Take a look which level to load
	map<wstring,wstring> *levels = new map<wstring,wstring>();
	loadGameProperties(game, levels, LEVEL_FILE);

	int levelCount = 0;
	wstringstream((*levels)[LEVEL_COUNT]) << levelCount;

	if (levelNumber <= 0 && levelNumber > levelCount) return; // Incorrect Level Number

	// Initialize
	wstringstream stream;
	int levelColumns, levelRows;
	wstring levelBGPath, levelWorldPath, levelElementsPath;

	// Get Level Information
	stream << levelNumber << LEVEL_NUM_OF_COLUMNS;
	wstringstream((*levels)[stream.str()]) >> levelColumns;
	stream.str(L"");

	stream << levelNumber << LEVEL_NUM_OF_ROWS;
	wstringstream((*levels)[stream.str()]) >> levelRows;
	stream.str(L"");

	stream << levelNumber << LEVEL_BG_PATH;
	levelBGPath = (*levels)[stream.str()];
	stream.str(L"");

	stream << levelNumber << LEVEL_WORLD_PATH;
	levelWorldPath = (*levels)[stream.str()];
	stream.str(L"");

	stream << levelNumber << LEVEL_ELEMENT_PATH;
	levelElementsPath = (*levels)[stream.str()];
	stream.str(L"");

	// NOTE THAT THE WORLD WILL BE THE SAME DIMENSIONS AS OUR TILED LAYER,
	// WE COULD GET A PARALLAX SCROLLING EFFECT IF WE MADE A LAYER SMALLER,
	// AND THEN SCROLLED IT SLOWER
	World *world = gsm->getWorld();
	world->setWorldWidth(levelColumns * TILE_WIDTH);
	world->setWorldHeight(levelRows * TILE_HEIGHT);

	int backgroundID = graphics->getWorldTextureManager()->loadTexture(levelBGPath);
	world->setBackground(backgroundID);

	vector<int>* respawnPoints = new vector<int>();

	Point* changeLevelPosition = new Point(0, 0);

	// Initiate World
	world->addLayer(loadTiledLayerFromFile(game,
		levelColumns, levelRows,
		levelElementsPath, levelWorldPath, respawnPoints, changeLevelPosition));

	world->setObjectiveSeeker(new FightGameObjective());
	world->setEndOfLevelPosition(changeLevelPosition);

	AnimatedSprite *endGamePoint = new AnimatedSprite();
	loadSprite(game, L"data/Sprites/BrightnessSprite.txt", endGamePoint);
	endGamePoint->setCurrentState(L"CHANGE");
	endGamePoint->setAlpha(255);
	PhysicalProperties *egpp = endGamePoint->getPhysicalProperties();
	egpp->setX(changeLevelPosition->getX());
	egpp->setY(changeLevelPosition->getY());
	egpp->setAccelerationX(0);
	egpp->setAccelerationY(0);

	gsm->getSpriteManager()->addAnimatedObject(endGamePoint);


	srand(time(NULL));

	////////////////////////////////////////////
	SpriteManager *spriteManager = gsm->getSpriteManager();
	
	// Load Starting Things

	//SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSprite *player = spriteManager->getPlayer();
	loadSprite(game, L"data/Sprites/PlayerSprite.txt", player);
	PhysicalProperties *playerProps = player->getPhysicalProperties();
	playerProps->setX(PLAYER_STARTING_X);
	playerProps->setY(PLAYER_STARTING_Y);
	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);
	playerProps->setBuoyancy(true);

	// Load Player Aura
	AnimatedSprite* aura = new AnimatedSprite();
	loadSprite(game, L"data/Sprites/AuraSprite.txt", aura);
	aura->setCurrentState(AURA_SHINING_STATE);
	dynamic_cast<Player*>(player)->setAura(aura);
	spriteManager->addAnimatedObject(aura);

	// WE WILL SET LOTS OF OTHER PROPERTIES ONCE
	// WE START DOING COLLISIONS AND PHYSICS

	player->setAlpha(255);
	player->setCurrentState(IDLE_STATE_RIGHT);
	dynamic_cast<Player*>(player)->reloadData();

	// Load Projectiles

	// Lightning Ball
	PenetrableProjectile* p = new PenetrableProjectile(PROJECTILE_MAGIC, 20, 5);
	p->setPenetrable(true);
	loadSprite(game, L"data/Sprites/LightningBall.txt", p);
	spriteManager->registerProjectile(p);

	// Add our Bots
	if (true)
		addBots(game, respawnPoints,
		convertToInt((*levels)[concat(levelNumber, L"BOT_TYPE1")]),
		convertToInt((*levels)[concat(levelNumber, L"BOT_TYPE2")]),
		convertToInt((*levels)[concat(levelNumber, L"BOT_TYPE3")]),
		convertToInt((*levels)[concat(levelNumber, L"BOT_TYPE4")]),
		convertToInt((*levels)[concat(levelNumber, L"BOT_TYPE5")]));
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
						0, // Invisible Cursor
						32,
						32);
	gui->setCursor(cursor);
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
	Loads the GUI from guiFile and returns it
*/
ScreenGUI* YADataLoader::loadGUIFromFile(Game *game, wstring guiFile, ScreenGUI* screen)
{
    map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, guiFile);

    // Initialize All Variables
    int imageQuantity, optionQuantity, x, y, width, height;
    wstring imageQuantityS, optionQuantityS, xS, yS, widthS, heightS, path, pathUnselected, pathSelected, animated, command;

    imageQuantityS = (*properties)[GUI_QUANTITY_OF_IMAGES];
    wstringstream(imageQuantityS) >> imageQuantity;

    optionQuantityS = (*properties)[GUI_QUANTITY_OF_OPTIONS];
    wstringstream(optionQuantityS) >> optionQuantity;

    // Texture Stuff
    GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();
    unsigned int imageID, imageID2;

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

        // Path, Animated and Command

        stream << i << GUI_OPTION_PATH_UNSELECTED;
        pathUnselected = (*properties)[stream.str()];
        stream.str(L"");

        stream << i << GUI_OPTION_PATH_SELECTED;
        pathSelected = (*properties)[stream.str()];
        stream.str(L"");

        stream << i << GUI_OPTION_COMMAND;
        command = (*properties)[stream.str()];
        stream.str(L"");

        buttonToAdd = new Button();

	    // - GET THE BUTTON COMMAND AND IMAGE IDs

        imageID = guiTextureManager->loadTexture(pathUnselected);

        if (pathUnselected.compare(pathSelected) == 0) {
            imageID2 = imageID;
        } else {
            imageID2 = guiTextureManager->loadTexture(pathSelected);
        }

	    // - INIT THE BUTTON
	    buttonToAdd->initButton(imageID, 
							    imageID2, // Doesnt Change when mouse over
							    x,
							    y,
							    0,
							    255,
                                width,
                                height,
							    false,
                                command);

	    // AND NOW LOAD IT INTO A ScreenGUI
	    screen->addButton(buttonToAdd);
    }

    return screen;
}

/*
	Loads the World from the given Files using the worldColumns and worldRows from that given
	files. It inserts the found respawnPoints into the given vector and returns the for the
	correspoding loaded World
*/

TiledLayer* YADataLoader::loadTiledLayerFromFile(Game *game,
		int worldColumns, int worldRows,
		wstring worldFile, wstring worldMapFile, vector<int>* respawnPoints,
		Point* endLevelPos)
{

	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, worldFile);

	// Load World File Map

	// NOW LOAD OUR TILED BACKGROUND
	TiledLayer *tiledLayer = new TiledLayer(	worldColumns,	worldRows, 
												TILE_WIDTH,		TILE_HEIGHT, 
												0, true, 
												worldColumns * TILE_WIDTH,
												worldRows * TILE_HEIGHT);

	map<wstring,int> *texturesID = new map<wstring,int>();

	int texturesCount, emptyTileCode;
	wstring textureCountS, elementTilePath, elementCode;

	emptyTileCode = game->getGraphics()->getWorldTextureManager()->loadTexture(EMPTY_TILE);

	wstringstream stream;

	// Count
    textureCountS = (*properties)[LEVEL_NUMBER_OF_ELEMENTS];
    wstringstream(textureCountS) >> texturesCount;
    stream.str(L"");

	// Load Textures
	for (int i = 1 ; i <= texturesCount ; i++) {
		stream << i << ELEMENT_TILE_PATH;
        elementTilePath = (*properties)[stream.str()];
		stream.str(L"");
		stream << i << ELEMENT_CODE;

		wstring blabla = (*properties)[stream.str()];
		int texCode = game->getGraphics()->getWorldTextureManager()->loadTexture(elementTilePath);

		// Insert the Code and ID
		(*texturesID)[(*properties)[stream.str()]] = texCode;
		stream.str(L"");
	}

	// Lets load our World

	BufferedTextFileReader reader;
	reader.initFile(worldMapFile);
	wstring line;

	int numberOfTilesTotal = 0;

	int tileCounterPerLine = 0;

	while (reader.hasMoreLines())
	{
		line = reader.getNextLine();

		int lineSize = line.size();

		tileCounterPerLine = 0;

		for (int i = 0 ; i < line.size() ; i++) {

			if (&(line[i]) == L"\n" || (line[i]) == 'N') {
				// This is not being used...
				while (tileCounterPerLine != worldColumns) {
					// Fill with Empty Tiles
					Tile *tileToAdd = new Tile();
					tileToAdd->collidable = false;
					tileToAdd->textureID = emptyTileCode;
					tiledLayer->addTile(tileToAdd);
					tileCounterPerLine++;
					numberOfTilesTotal++;
				}
				continue;
			}

		if (line[i] == ' ') { // Empty tile if blank space
			Tile *tileToAdd = new Tile();
			tileToAdd->collidable = false;
			tileToAdd->textureID = emptyTileCode;
			tiledLayer->addTile(tileToAdd);
			tileCounterPerLine++;
			numberOfTilesTotal++;
			continue;
		} else if (line[i] == '*') { // Respawn Point

			int columnNumber = (numberOfTilesTotal % worldColumns);
			int x = columnNumber * TILE_WIDTH;

			int rowNumber = ((numberOfTilesTotal / worldColumns));
			int y = rowNumber * TILE_HEIGHT;

			respawnPoints->push_back(x); // X
			respawnPoints->push_back(y); // Y

			// Add an Empty Tile there

			Tile *tileToAdd = new Tile();
			tileToAdd->collidable = false;
			tileToAdd->textureID = emptyTileCode;
			tiledLayer->addTile(tileToAdd);
			tileCounterPerLine++;
			numberOfTilesTotal++;

			continue;
		} else if (line[i] == '^') {
			// Change Level Position
			int columnNumber = (numberOfTilesTotal % worldColumns);
			int x = columnNumber * TILE_WIDTH;

			int rowNumber = ((numberOfTilesTotal / worldColumns));
			int y = rowNumber * TILE_HEIGHT;

			endLevelPos->setX(x);
			endLevelPos->setY(y);

			// Add an Empty Tile there

			Tile *tileToAdd = new Tile();
			tileToAdd->collidable = false;
			tileToAdd->textureID = emptyTileCode;
			tiledLayer->addTile(tileToAdd);
			tileCounterPerLine++;
			numberOfTilesTotal++;
			continue;
		}

		// Gets the Tile related to that Code and add the layer
		Tile *tileToAdd = new Tile();

		tileToAdd->collidable = true;

		stream << line[i];
		wstring blabla = stream.str();
		tileToAdd->textureID = (*texturesID)[ stream.str() ];
		stream.str(L"");

		tiledLayer->addTile(tileToAdd);
		numberOfTilesTotal++;

		}

	}

	return tiledLayer;
}

/*
	Add the Bots to our Game
*/
void YADataLoader::addBots(Game* game, vector<int>* respawnPoints,
	int type1Quantity, int type2Quantity, int type3Quantity,
	int type4Quantity, int type5Quantity)
{
	SpriteManager* spriteManager = game->getGSM()->getSpriteManager();

	// This map will hold the respawnPoint and how many monsters it has
	map<int, int>* monstersOnRespawn = new map<int, int>();
	EnemyBot *sample;
	
	
	sample= new EnemyBot(TYPE1_HP, TYPE1_ATTACK, TYPE1_SPEED);
	loadSprite(game, TYPE1_BOT_FILE, sample);

	addBot(sample, type1Quantity, respawnPoints, monstersOnRespawn, spriteManager);
	
	delete sample; // Free the last bot...
	sample = new EnemyBot(TYPE2_HP, TYPE2_ATTACK, TYPE2_SPEED);
	loadSprite(game, TYPE2_BOT_FILE, sample);

	addBot(sample, type2Quantity, respawnPoints, monstersOnRespawn, spriteManager);

	delete sample;
	sample = new EnemyBot(TYPE3_HP, TYPE3_ATTACK, TYPE3_SPEED);
	loadSprite(game, TYPE3_BOT_FILE, sample);

	addBot(sample, type3Quantity, respawnPoints, monstersOnRespawn, spriteManager);

	delete sample;
	sample = new EnemyBot(TYPE4_HP, TYPE4_ATTACK, TYPE4_SPEED);
	loadSprite(game, TYPE4_BOT_FILE, sample);

	addBot(sample, type4Quantity, respawnPoints, monstersOnRespawn, spriteManager);

	delete sample;
	sample = new EnemyBot(TYPE5_HP, TYPE5_ATTACK, TYPE5_SPEED);
	loadSprite(game, TYPE5_BOT_FILE, sample);

	addBot(sample, type5Quantity, respawnPoints, monstersOnRespawn, spriteManager);

	delete sample;
	delete monstersOnRespawn;
}

void YADataLoader::loadSprite(Game* game, wstring fileName, AnimatedSprite* sprite)
{
	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, fileName);

	int spriteWidth, spriteHeight, bbX, bbY, bbWidth, bbHeight, spriteCount, frameCount, frameTime;
	wstring stateName, frameSource;
	AnimatedSpriteType *ast = new AnimatedSpriteType();

	spriteWidth = convertToInt((*properties)[L"SPRITE_WIDTH"]);
	spriteHeight = convertToInt((*properties)[L"SPRITE_HEIGHT"]);
	bbX = convertToInt((*properties)[L"BOUNDING_BOX_X"]);
	bbY = convertToInt((*properties)[L"BOUNDING_BOX_Y"]);
	bbWidth = convertToInt((*properties)[L"BOUNDING_BOX_WIDTH"]);
	bbHeight = convertToInt((*properties)[L"BOUNDING_BOX_HEIGHT"]);
	spriteCount = convertToInt((*properties)[L"NUMBER_OF_SPRITES"]);

	ast->setTextureSize(spriteWidth, spriteHeight);

	for (int i = 1 ; i <= spriteCount ; i++) {

		// Load Each State

		stateName = (*properties)[concat(i, L"STATE_NAME")];
		frameCount = convertToInt((*properties)[concat(i, L"FRAME_COUNT")]);

		ast->addAnimationSequence(stateName);

		for (int j = 1 ; j <= frameCount ; j++) {
			// Load Each Frame

			frameSource = (*properties)[concat(i, concat(j, L"FRAME_SOURCE"))];
			frameTime = convertToInt((*properties)[concat(i, concat(j, L"FRAME_TIME"))]);

			ast->addAnimationFrame(stateName, game->getGraphics()->getWorldTextureManager()->loadTexture(frameSource), frameTime);

		}
	}

	unsigned int spriteTypeID = game->getGSM()->getSpriteManager()->addSpriteType(ast);
	ast->setSpriteTypeID(spriteTypeID);
	sprite->setSpriteType(ast);
	sprite->getBoundingVolume()->setX(bbX);
	sprite->getBoundingVolume()->setY(bbY);
	sprite->getBoundingVolume()->setWidth(bbWidth);
	sprite->getBoundingVolume()->setHeight(bbHeight);

	delete properties;
}

void YADataLoader::loadInGameGUI(Game* game, PlayerGUI* pGUI)
{
	// This should be on a special file...

	// HP BAR ... 
	OverlayImage *hpBar = new OverlayImage();
	hpBar->x = 157; // Specific Position, Refactoring Needed
	hpBar->y = 54; // Specific Position, Refactoring Needed
	hpBar->z = 0;
	hpBar->alpha = 255;
    hpBar->width = 5;
    hpBar->height = 15;
	hpBar->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(HP_BAR_PIECE);

	pGUI->addHPBar(hpBar, HP_BAR_MAX);

	// MANA BAR ...
	OverlayImage *manaBar = new OverlayImage();
	manaBar->x = 157; // Specific Position, Refactoring Needed
	manaBar->y = 79; // Specific Position, Refactoring Needed
	manaBar->z = 0;
	manaBar->alpha = 255;
    manaBar->width = 5;
    manaBar->height = 15;
	manaBar->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(MANA_BAR_PIECE);

	pGUI->addManaBar(manaBar, MANA_BAR_MAX);

	OverlayImage *centerMessage = new OverlayImage();
	centerMessage->x = 0; // Any
	centerMessage->y = 0; // Any
	centerMessage->z = 0;
	centerMessage->alpha = 255;
    centerMessage->width = 420;
    centerMessage->height = 50;
	centerMessage->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(ATTACK_INCREASED_IMAGE_PATH);

	pGUI->addCenterMessage(ATTACK_INCREASED_IMAGE, centerMessage);

	centerMessage = new OverlayImage();
	centerMessage->x = 0; // Any
	centerMessage->y = 0; // Any
	centerMessage->z = 0;
	centerMessage->alpha = 255;
    centerMessage->width = 460;
    centerMessage->height = 50;
	centerMessage->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(MAGIC_PENETRATION_IMAGE_PATH);
	pGUI->addCenterMessage(MAGIC_PENETRATION_IMAGE, centerMessage);

	centerMessage = new OverlayImage();
	centerMessage->x = 0; // Any
	centerMessage->y = 0; // Any
	centerMessage->z = 0;
	centerMessage->alpha = 255;
    centerMessage->width = 460;
    centerMessage->height = 50;
	centerMessage->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(DEFENSE_INCREASE_IMAGE_PATH);
	pGUI->addCenterMessage(DEFENSE_INCREASE_IMAGE, centerMessage);

	dynamic_cast<Player*>(game->getGSM()->getSpriteManager()->getPlayer())->setImageShower(pGUI);

	// Player Bars
	OverlayImage *playerBar = new OverlayImage();
	playerBar->x = 25;
	playerBar->y = 15;
	playerBar->z = 0;
	playerBar->width = 300;
	playerBar->height = 64;
	playerBar->alpha = 255;
	playerBar->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(L"textures/gui/overlays/player_bar.png");

	OverlayImage *altPlayerBar = new OverlayImage();
	altPlayerBar->x = 25;
	altPlayerBar->y = 15;
	altPlayerBar->z = 0;
	altPlayerBar->width = 300;
	altPlayerBar->height = 64;
	altPlayerBar->alpha = 255;
	altPlayerBar->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(L"textures/gui/overlays/player_bar_red.png");

	pGUI->setPlayerBars(playerBar, altPlayerBar);

	pGUI->setBlackAndGameOverImageID(game->getGraphics()->getGUITextureManager()->loadTexture(L"textures/gui/overlays/blackscreen.png"),
		game->getGraphics()->getGUITextureManager()->loadTexture(L"textures/gui/overlays/gameover.png"));

}

void YADataLoader::addBot(Bot* bot,int quantity, vector<int>* respawnPoints, map<int,int>* monstersOnRespawn,
	SpriteManager* sm)
{

	// Add Bots
	for (int i = 0; i < quantity; i++)
	{
		Bot *newBot = bot->clone();
		newBot->setCurrentState(BOT_IDLE_LEFT);
		newBot->setAlpha(255);
		PhysicalProperties *pp = newBot->getPhysicalProperties();
		pp->setCollidable(false);

		int x;
		int y;

		int numberOfRespawnPoints = respawnPoints->size() / 2;
		int chosenRespawnPoint = (rand() % (numberOfRespawnPoints)) + 1;

		bool foundHisPlace = false;

		while (!foundHisPlace) {

			if (monstersOnRespawn->find(chosenRespawnPoint) == monstersOnRespawn->end()) {
				monstersOnRespawn->insert( pair<int, int> (chosenRespawnPoint, 1) );
				foundHisPlace = true;
				continue;
			}

			if (monstersOnRespawn->find(chosenRespawnPoint)->second > MONSTERS_PER_SPOT) {
				chosenRespawnPoint = (rand() % (numberOfRespawnPoints)) + 1; // Get another one
			} else {
				monstersOnRespawn->find(chosenRespawnPoint)->second++;
				foundHisPlace = true;
			}

		}

		x = respawnPoints->at((chosenRespawnPoint * 2) - 2);
		y = respawnPoints->at((chosenRespawnPoint * 2) - 1) - BOTS_RESPAWN_OFFSET_Y; // Put a little up

		pp->setX(x);
		pp->setY(y);
		pp->setAccelerationX(0.0f);
		pp->setAccelerationY(0.0f);
		sm->addBot(newBot);
	}
}

void YADataLoader::loadStoryBoard(Game* game, FadeScreen* fScreen)
{
	OverlayImage* image = new OverlayImage();

	image->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(
		L"textures/gui/overlays/blackscreen.png");
	image->x = 0;
	image->y = 0;
	image->z = 0;
	image->width = 1024;
	image->height = 768;

	fScreen->setBackground(image);

	// First Image of StoryBoard
	image = new OverlayImage();
	image->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(
		L"textures/gui/overlays/story1.png");
	image->x = 0;
	image->y = 0;
	image->z = 0;
	image->width = 1024;
	image->height = 768;

	fScreen->addImage(image);

	image = new OverlayImage();
	image->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(
		L"textures/gui/overlays/story2.png");
	image->x = 0;
	image->y = 0;
	image->z = 0;
	image->width = 1024;
	image->height = 768;

	fScreen->addImage(image);

	image = new OverlayImage();
	image->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(
		L"textures/gui/overlays/story3.png");
	image->x = 0;
	image->y = 0;
	image->z = 0;
	image->width = 1024;
	image->height = 768;

	fScreen->addImage(image);


}

void YADataLoader::loadEndGame(Game* game, FadeScreen* fScreen)
{
	OverlayImage* image = new OverlayImage();

	image->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(
		L"textures/gui/overlays/blackscreen.png");
	image->x = 0;
	image->y = 0;
	image->z = 0;
	image->width = 1024;
	image->height = 768;

	fScreen->setBackground(image);

	// First Image of StoryBoard
	image = new OverlayImage();
	image->imageID = game->getGraphics()->getGUITextureManager()->loadTexture(
		L"textures/gui/overlays/endgame.png");
	image->x = 0;
	image->y = 0;
	image->z = 0;
	image->width = 1024;
	image->height = 768;

	fScreen->addImage(image);

}