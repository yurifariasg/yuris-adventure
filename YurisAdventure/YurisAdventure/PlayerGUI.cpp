#include "PlayerGUI.h"
#include "Player.h"


PlayerGUI::PlayerGUI(void)
{
	hpBar = NULL;
	manaBar = NULL;
	centerImage = NULL;
	showAlternatePlayerBar = false;
	isPlayerDead = false;
	alpha = 0;
	count = 0;
}


PlayerGUI::~PlayerGUI(void)
{
}

/*
	Get all the information to put on RenderList...
*/

void PlayerGUI::updateGUI(Game* game)
{
	// Get Player INFO
	Player* p = dynamic_cast<Player*>(game->getGSM()->getSpriteManager()->getPlayer());
	
	int hpPercentage = p->getCurrentHP() * 100 / p->getMaxHP();
	int manaPercentage = p->getCurrentMana() * 100 / p->getMaxMana();
	PlayerComboState currentCombo = p->getCurrentComboState();

	showAlternatePlayerBar = p->hasBuffActives();

	if (p->isDead() && !isPlayerDead) {
		isPlayerDead = true;
		count = 500;
	} else if (!p->isDead()) {
		isPlayerDead = false;
		alpha = 0;
	}

	// Now, throw this information on screen !
	hpBarQuantity = hpPercentage * maxHPBarImages / 100;
	manaBarQuantity = manaPercentage * maxManaBarImages / 100;

	if (centerImage != NULL) {

		// Do Animation !

		if (count == 200) {
			centerImage->x = 1024; // 500
			centerImage->y = 50;
		} else if (count > 150) {
			centerImage->x -= 20; // Velocity X
			if (centerImage->x < 350) centerImage->x = 350;
		} else if (count < 50) {
			centerImage->x += 20;
		}
		


		
		count--;
		if (count == 0) centerImage = NULL;

	}

}

void PlayerGUI::addRenderItemsToRenderList(RenderList *renderList)
{

	if (!showAlternatePlayerBar)
		renderList->addRenderItem(
			playerBar->imageID,
			playerBar->x,
			playerBar->y,
			0,
			255,
			playerBar->width,
			playerBar->height);
	else
		renderList->addRenderItem(
			alternatePlayerBar->imageID,
			alternatePlayerBar->x,
			alternatePlayerBar->y,
			0,
			255,
			alternatePlayerBar->width,
			alternatePlayerBar->height);

	if (hpBar != NULL) {
		// Theres HP Bar Registered
		int currentX = hpBar->x;
		for ( int i = 0 ; i < hpBarQuantity ; i++ ) {
			renderList->addRenderItem(
				hpBar->imageID,
				currentX,
				hpBar->y,
				0,
				255,
				hpBar->width,
				hpBar->height);

			currentX += hpBar->width;
		}
	}

	if (manaBar != NULL) {
		// Theres HP Bar Registered
		int currentX = manaBar->x;
		for ( int i = 0 ; i < manaBarQuantity ; i++ ) {
			renderList->addRenderItem(
				manaBar->imageID,
				currentX,
				manaBar->y,
				0,
				255,
				manaBar->width,
				manaBar->height);

			currentX += manaBar->width;
		}
	}

	if (centerImage != NULL) {

		renderList->addRenderItem(
			centerImage->imageID,
			centerImage->x,
			centerImage->y,
			0,
			255,
			centerImage->width,
			centerImage->height);

	}

	if (isPlayerDead) {

		renderList->addRenderItem(
			blackImageID,
			0,
			0,
			0,
			alpha,
			1024,
			768);

		alpha++;
		alpha++;

		if (alpha >= 255) {
			renderList->addRenderItem(
			gameOverImageID,
			0,
			0,
			0,
			255,
			1024,
			768);
		}
		count--;
	}
}

void PlayerGUI::addHPBar(OverlayImage* imageBar, int maxQuantity)
{
	hpBar = imageBar;
	maxHPBarImages = maxQuantity;
}
void PlayerGUI::addManaBar(OverlayImage* imageBar, int maxQuantity)
{
	manaBar = imageBar;
	maxManaBarImages = maxQuantity;
}

void PlayerGUI::addCenterMessage(wstring messageName, OverlayImage* messageImage)
{
	centerMessages[messageName] = messageImage;
}

void PlayerGUI::showImage(wstring messageName)
{
	count = 200;
	centerImage = centerMessages[messageName];
}