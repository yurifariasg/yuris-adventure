/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	YAButtonEventHandler.cpp

	See YAButtonEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "YAGame.h"
#include "YAButtonEventHandler.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"

void YAButtonEventHandler::handleButtonEvents(	Game *game, 
													wstring command)
{
	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(DG_EXIT_COMMAND) == 0)
	{
		game->shutdown();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(DG_GO_TO_MM_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToMainMenu();
	}
	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	else if (command.compare(DG_START_COMMAND) == 0)
	{
		//game->startGame();
		game->getGSM()->goToStoryBoard();
	}
	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(DG_QUIT_COMMAND) == 0)
	{
		game->quitGame();
	}
    else if (command.compare(L"Controls") == 0)
	{
		game->goToControls();
	}
    else if (command.compare(L"About") == 0)
	{
        game->getGSM()->goToAbout();
	}
    else if (command.compare(L"Continue") == 0)
	{
        game->getGSM()->goToGame();
	}
    else if (command.compare(L"Controls InGame") == 0)
	{
		game->getGSM()->goToControlsInGame();
	}
    else if (command.compare(L"About InGame") == 0)
	{
		game->getGSM()->goToAboutInGame();
	}
    else if (command.compare(L"Quit Game") == 0)
	{
		game->quitGame();
	}
}