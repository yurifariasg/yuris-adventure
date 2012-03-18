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

const bool BOTS_ACTIVE = true;

class YADataLoader : public GameDataLoader
{
private:
	// WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
	HINSTANCE hInstance;
	wstringstream convertStream;
	int nCmdShow;

	// Add the bots into our game
	void addBots(Game*, vector<int>*);

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
    ScreenGUI* loadGUIFromFile(Game *game, wstring guiFile);
	TiledLayer* loadTiledLayerFromFile(Game *game, int worldColumns, int worldRows, wstring worldFile, wstring worldMapFile, vector<int>* respawnPoints);
};