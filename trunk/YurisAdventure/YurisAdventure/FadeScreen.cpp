#include "FadeScreen.h"


FadeScreen::FadeScreen(void)
{
	currentImage = 0;
	animationTime = 0;
	currentAlpha = 0;
}


FadeScreen::~FadeScreen(void)
{
}


void FadeScreen::addRenderItemsToRenderList(RenderList *renderList)
{
	OverlayImage* image;

	image = background;
	// Add Background
	renderList->addRenderItem(	image->imageID,
								image->x,
								image->y,
								image->z,
								255,
								image->width,
								image->height	);

	// No more images..
	if (currentImage >= images.size()) return;

	// Add current image and its transparency

	image = images.at(currentImage);

	if (animationTime < FADE_ANIMATION_TIME && currentAlpha < 255)
		currentAlpha++;

	if (animationTime > FADE_ANIMATION_TIME * 4 && currentAlpha > 0)
		currentAlpha--;	
	
	renderList->addRenderItem(	image->imageID,
							image->x,
							image->y,
							image->z,
							currentAlpha,
							image->width,
							image->height	);

	if (animationTime > FADE_ANIMATION_TIME * 5) {
		currentImage++;
		animationTime = 0;
		currentAlpha = 0;
	}

	animationTime++;
}