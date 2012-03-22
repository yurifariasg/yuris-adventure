/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	YATextGenerator.cpp

	See YATextGenerator.h for a class description.
*/

#include "stdafx.h"
#include "YAGame.h"
#include "YATextGenerator.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\text\GameText.h"
#include "SSSF_SourceCode\timer\GameTimer.h"
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsTimer.h"
#include "Player.h"

static const wstring	DG_TARGET_FPS_TEXT = L"TARGET FPS: ";
static const int		DG_TARGET_FPS_X	= 2;
static const int		DG_TARGET_FPS_Y	= 2;

/*
	appendTargetFPS - this method gets and appends the target frame rate to
	the text we are generating.
*/
void YATextGenerator::appendTargetFPS(Game *game)
{
	// DEBUGGING DISABLED !

	// WE'LL BUILD OUR WIDE STRING USING A WIDE STRING STREAM
	wstringstream wss;
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();

	// BUILD THE WIDE STRING
	//textToGenerate.append(DG_TARGET_FPS_TEXT);
	wss << timer->getTargetFPS();
	//textToGenerate.append(wss.str());

	// Here
	float playerX = game->getGSM()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getX();

	wstringstream stream;

	Player* player = dynamic_cast<Player*>(game->getGSM()->getSpriteManager()->getPlayer());

	int attackingPointX = player->getPhysicalProperties()->getX() +
		player->getBoundingVolume()->getX() +
		(player->getBoundingVolume()->getWidth() / 2),
		attackingPointY = player->getPhysicalProperties()->getY() +
		player->getBoundingVolume()->getY() +
		(player->getBoundingVolume()->getHeight() / 2);

	stream << " Player Location: " << attackingPointX;

	stream << " Attack: " << player->getAttack();

	wstring text = stream.str();
	// Debug Disabled!
	//textToGenerate.append(stream.str());
}

/*
	initText - Provides an example of how to render text to our screen every frame,
	allowing for the text to continually change. This method adds a text object
	to the text manager (GameText), which allows it to be updated each frame. This
	method should be called only once for each object.
*/
void YATextGenerator::initText(Game *game)
{
	// FIRST UPDATE THE TEXT THIS GENERATOR IS USING
	appendTargetFPS(game);

	// AND THEN HAND IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
	GameText *text = game->getText();
	text->addText(&textToGenerate, DG_TARGET_FPS_X, DG_TARGET_FPS_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
}

/*
	updateText - updates the text. Note that the text must already have been added
	to the text manager via the initText method. Also not that there should be a
	separate TextGenerator for each line of text we wish to render.
*/
void YATextGenerator::updateText(Game *game)
{
	// THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
	// SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
	// WHAT WE WANT.
	textToGenerate.clear();
	appendTargetFPS(game);
}