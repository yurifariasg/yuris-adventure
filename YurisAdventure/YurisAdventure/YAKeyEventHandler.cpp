/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	YAKeyEventHandler.cpp

	See YAKeyEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "YAKeyEventHandler.h"
#include "YAGame.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\game\WStringTable.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "SSSF_SourceCode\gsm\state\GameState.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gui\Cursor.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\gui\ScreenGUI.h"
#include "SSSF_SourceCode\input\GameInput.h"
#include "SSSF_SourceCode\timer\GameTimer.h"
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

#include "YASpriteManager.h"
#include "FadeScreen.h"

// THIS IS JUST FOR SHOWING HOW THE CURSOR CAN BE CHANGED
const unsigned int C_KEY = (unsigned int)'C';


const int MIN_FPS = 5;
const int MAX_FPS = 100;
const int FPS_INC = 1;

/*
	handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
	gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
	key states since the last frame, which can allow us to respond to key presses, including when keys
	are held down for multiple frames.
*/
void YAKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	PhysicalProperties *pp = player->getPhysicalProperties();
	
	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		// CHECK FOR WASD KEYS FOR MOVEMENT
		int incX = 0;
		int incY = 0;
		bool moveViewport = false;

		// Get our Player

		Player* player = ((YASpriteManager*) game->getGSM()->getSpriteManager())->getPlayer();

		if (!player->isDead()) { // isDead if statement

		// WASD AND DIRECTION KEY PRESSES WILL CONTROL THE PLAYER,
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
		float vX = pp->getVelocityX();
		float vY = pp->getVelocityY();


		if (input->isKeyDown(ACTION_KEY) && (player->canDoSomething() || !player->notCharging())) {

			if (holdingMagicKey) {
				player->charges();
				player->recoverMana(count == 30 ? 1 : 0);

				if (count > 30) count = 25;
			}

			player->processCombo(ACTION_KEY);

			if (count > 25) holdingMagicKey = true;

			count++;

		} else {
			player->stopCharging();
			holdingMagicKey = false;
			count = 0;
		}

		if (input->isKeyDownForFirstTime(W_KEY) && player->canDoSomething())
		{

			// Jump
			if (!pp->getBuoyancy()) {
				pp->setAccelerationY(-40);
				pp->setBuoyancy(true);
			}

			player->processCombo(W_KEY);

		}
		if (input->isKeyDown(A_KEY) && (player->canDoSomething() || pp->getBuoyancy()))
		{
			vX = -PLAYER_SPEED;
			player->processCombo(A_KEY);
		}
		if (input->isKeyDown(S_KEY))
		{
			//vY = PLAYER_SPEED;
			// Crouch
			player->crouch();

			if (input->isKeyDownForFirstTime(S_KEY))
				player->processCombo(S_KEY);
		}
		if (input->isKeyDown(D_KEY) && (player->canDoSomething() || pp->getBuoyancy()))
		{
			vX = PLAYER_SPEED;
			player->processCombo(D_KEY);
		}

		if (input->isKeyDownForFirstTime(D_KEY)) {

			if (player->isFacingLeft()) player->changeFacingDirection();

		} else if (input->isKeyDownForFirstTime(A_KEY)) {

			if (player->isFacingRight()) player->changeFacingDirection();

		}

		if (input->isKeyDownForFirstTime(VK_ESCAPE) || input->isKeyDownForFirstTime(VK_TAB)) {
			game->getGSM()->goToInGameMenu();
		}

		// Cannot Fly

		if (!player->getPhysicalProperties()->getBuoyancy()) {

			if (input->isKeyDownForFirstTime(SWORD_ATTACK_KEY) && !player->isAttacking()) {

				player->startAttack();
				player->processCombo(SWORD_ATTACK_KEY);

			} else if (input->isKeyDownForFirstTime(MAGIC_ATTACK_KEY) &&
				player->getCurrentMana() >= MAGIC_ATTACK_MANA) {

				int attackingPointX = player->getPhysicalProperties()->getX() +
					player->getBoundingVolume()->getX() +
					(player->getBoundingVolume()->getWidth() / 2),
					attackingPointY = player->getPhysicalProperties()->getY() +
					player->getBoundingVolume()->getY() +
					(player->getBoundingVolume()->getHeight() / 2);

				// Offset
				if (player->isFacingRight()) attackingPointX += player->getBoundingVolume()->getWidth() + 10;
				else attackingPointX -= player->getBoundingVolume()->getWidth() + 10;
				attackingPointY -= 20;

				dynamic_cast<YASpriteManager*>(game->getGSM()->getSpriteManager())->
					addProjectile(
					PROJECTILE_MAGIC, attackingPointX, attackingPointY,
					player->isFacingRight(), player->getPenetrationIsActive());

				player->useMana(MAGIC_ATTACK_MANA);
				player->casts();
				player->processCombo(MAGIC_ATTACK_KEY);


			}
		}

		// NOW SET THE ACTUAL VELOCITY
		Physics *physics = gsm->getPhysics();
		pp->setVelocitySafely(physics, vX, vY);

		} // is dead end if statement
		


		// NOTE THAT THE VIEWPORT SHOULD FOLLOW THE PLAYER, AND SO SHOULD BE CORRECTED AFTER PHYSICS
		// ARE APPLIED. I HAVE PROVIDED A SIMPLE WAY OF DOING SO, WHICH SHOULD BE IMPROVED, DEPENDING
		// ON THE GAME'S NEED

	} else if (gsm->getCurrentGameState() == GS_SPLASH_SCREEN) {
		// Splash Screen

		if (input->isKeyDownForFirstTime(VK_RETURN) ||
			input->isKeyDownForFirstTime(VK_SPACE)) {
				game->getGSM()->goToMainMenu();
		}

    } else if (gsm->getCurrentGameState() == GS_MAIN_MENU) {
		// Main Menu

        if (input->isKeyDownForFirstTime(S_KEY)) {
            game->getGUI()->getScreen(GS_MAIN_MENU)->nextGUIOption();
        } else if (input->isKeyDownForFirstTime(W_KEY)) {
            game->getGUI()->getScreen(GS_MAIN_MENU)->previousGUIOption();
        } else if (input->isKeyDownForFirstTime(VK_RETURN)) {
            
            int secOption = game->getGUI()->getScreen(GS_MAIN_MENU)->getCurrentGUIOption();
            game->getGUI()->getScreen(GS_MAIN_MENU)->getSelectedButton()->fireEvent(game);

		}


    } else if (gsm->getCurrentGameState() == GS_PAUSED) {
		// Paused State

        if (input->isKeyDownForFirstTime(S_KEY)) {
            game->getGUI()->getScreen(GS_PAUSED)->nextGUIOption();
        } else if (input->isKeyDownForFirstTime(W_KEY)) {
            game->getGUI()->getScreen(GS_PAUSED)->previousGUIOption();
        } else if (input->isKeyDownForFirstTime(VK_RETURN)) {
            
            int secOption = game->getGUI()->getScreen(GS_PAUSED)->getCurrentGUIOption();
            game->getGUI()->getScreen(GS_PAUSED)->getSelectedButton()->fireEvent(game);

		} else if (input->isKeyDownForFirstTime(VK_ESCAPE) || input->isKeyDownForFirstTime(VK_TAB)) {
			game->getGSM()->goToGame();
		}


	} else if (gsm->getCurrentGameState() == GS_IN_GAME_CONTROLS || gsm->getCurrentGameState() == GS_IN_GAME_ABOUT) {
		// InGame Controls and InGame About

		if (input->isKeyDownForFirstTime(VK_ESCAPE) || input->isKeyDownForFirstTime(VK_RETURN)) {
			game->getGSM()->goToInGameMenu();
        }


    } else if (gsm->getCurrentGameState() == GS_MENU_CONTROLS_MENU) {
		// Controls Menu

        if (input->isKeyDownForFirstTime(VK_ESCAPE) || input->isKeyDownForFirstTime(VK_RETURN)) {
            game->getGSM()->goToMainMenu();
        }
    } else if (gsm->getCurrentGameState() == GS_MENU_ABOUT_MENU) {
		// About Menu
		
        if (input->isKeyDownForFirstTime(VK_ESCAPE) || input->isKeyDownForFirstTime(VK_RETURN)) {
            game->getGSM()->goToMainMenu();
        }
	} else if (gsm->getCurrentGameState() == GS_STORY_BOARD) {

		if (input->isKeyDownForFirstTime(VK_SPACE) ||
			input->isKeyDownForFirstTime(VK_RETURN) ||
			input->isKeyDownForFirstTime(VK_ESCAPE) ||
			dynamic_cast<FadeScreen*>(game->getGUI()->getScreen(GS_STORY_BOARD))->isDone()) {
				game->startGame();

				// Resets data, for further uses...
				dynamic_cast<FadeScreen*>(game->getGUI()->getScreen(GS_STORY_BOARD))->reset();
		}

	} else if (gsm->getCurrentGameState() == GS_END_GAME) {

		if (input->isKeyDownForFirstTime(VK_SPACE) ||
			input->isKeyDownForFirstTime(VK_RETURN) ||
			input->isKeyDownForFirstTime(VK_ESCAPE) ||
			dynamic_cast<FadeScreen*>(game->getGUI()->getScreen(GS_END_GAME))->isDone()) {
				game->getGSM()->goToMainMenu();

				// Resets data, for further uses...
				dynamic_cast<FadeScreen*>(game->getGUI()->getScreen(GS_END_GAME))->reset();
		}

	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;		
		cursor->setActiveCursorID(id);
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);
}