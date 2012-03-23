/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Modified by: Yuri Farias Gomes

	YADataLoader.h

	This class provides a custom importer for the Yuris Adventure to import
	game data, gui data, and world (i.e. level) data.
*/
#pragma once
#include "stdafx.h"
#include <stdio.h>
#include "SSSF_SourceCode\dataLoader\GameDataLoader.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"
#include "SSSF_SourceCode\gui\ScreenGUI.h"
#include "Player.h"
#include "SSSF_SourceCode\gsm\world\Point.h"
#include "BotVelocity.h"
#include "PlayerGUI.h"
#include "FadeScreen.h"

const bool BOTS_ACTIVE = true;
const int BOTS_RESPAWN_OFFSET_Y = 50;
const int MONSTERS_PER_SPOT = 4;

const wstring TYPE1_BOT_FILE = L"data/Sprites/GreenMonster.txt";
const int TYPE1_HP = 70;
const int TYPE1_ATTACK = 5;
const BotVelocity TYPE1_SPEED = BOT_SLOW;

const wstring TYPE2_BOT_FILE = L"data/Sprites/BanditSprite.txt";
const int TYPE2_HP = 140;
const int TYPE2_ATTACK = 10;
const BotVelocity TYPE2_SPEED = BOT_NORMAL;

const wstring TYPE3_BOT_FILE = L"data/Sprites/SkeletonSprite.txt";
const int TYPE3_HP = 150;
const int TYPE3_ATTACK = 5;
const BotVelocity TYPE3_SPEED = BOT_NORMAL;

const wstring TYPE4_BOT_FILE = L"data/Sprites/KnightSprite.txt";
const int TYPE4_HP = 300;
const int TYPE4_ATTACK = 5;
const BotVelocity TYPE4_SPEED = BOT_FAST;

const wstring TYPE5_BOT_FILE = L"data/Sprites/BossSprite.txt";
const int TYPE5_HP = 1000;
const int TYPE5_ATTACK = 10;
const BotVelocity TYPE5_SPEED = BOT_FAST;

const int PLAYER_STARTING_X = 100;
const int PLAYER_STARTING_Y = 500;

class YADataLoader : public GameDataLoader
{
private:
	// WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
	HINSTANCE hInstance;
	wstringstream convertStream;
	int nCmdShow;

	// Add the bots into our game
	void addBots(Game*, vector<int>*, int,int,int,int,int);

	int convertToInt(wstring convertString)
	{
		int y;
		wstringstream(convertString) >> y;
		return y;
	}

	wstring concat(int i, wstring string)
	{
		wstring finalStream;
		convertStream.str(L"");
		convertStream << i << string;
		finalStream.assign(convertStream.str());
        convertStream.str(L"");
		return finalStream;
	}

	void addBot(Bot* bot,int quantity, vector<int>* respawnPoints, map<int,int>* monstersOnRespawn, SpriteManager* sm);

public:
	YADataLoader()	{}
	~YADataLoader()	{}

	// INLINED MUTATOR METHOD
	void initWinHandle(HINSTANCE initHInstance, int initNCmdShow)
	{
		hInstance = initHInstance;
		nCmdShow = initNCmdShow;
	}


	// THESE SHOULD BE LOADING DATA FROM FILES. THEY
	// ARE ALL DEFINED IN DummyDataLoader.cpp
	void loadGame(Game *game, wstring gameInitFile);
	void loadGUI(Game *game, wstring guiInitFile);
	void loadWorld(Game *game, int levelNumber);
	void loadSprite(Game* game, wstring fileName, AnimatedSprite* sprite);

	// SOME HELPER METHODS FOR SETTING UP THE GAME
	void initColors(GameGraphics *graphics,	map<wstring,wstring> *properties);
	void initViewport(GameGUI *gui, map<wstring,wstring> *properties);

	// AND SOME HELPER METHODS FOR LOADING THE GUI
	void initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager);
    
    // New Methods
	void loadInGameGUI(Game*,PlayerGUI*);
	void loadStoryBoard(Game*, FadeScreen*);
	void loadEndGame(Game*, FadeScreen*);
    ScreenGUI* loadGUIFromFile(Game *game, wstring guiFile, ScreenGUI*);
	TiledLayer* loadTiledLayerFromFile(Game *game, int worldColumns, int worldRows, wstring worldFile, wstring worldMapFile, vector<int>* respawnPoints, Point* changeLevelPos);
};