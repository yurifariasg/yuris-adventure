#pragma once

#include "SSSF_SourceCode\gui\ScreenGUI.h"

const int FADE_ANIMATION_TIME = 200;
class FadeScreen : public ScreenGUI
{
private:
	vector<OverlayImage*> images;
	OverlayImage* background;
	int currentImage;
	int animationTime;
	int currentAlpha;

public:
	FadeScreen(void);
	~FadeScreen(void);
	void addImage(OverlayImage* oi) { images.push_back(oi); }
	void setBackground(OverlayImage* oi) { background = oi; }
	void addRenderItemsToRenderList(RenderList *renderList);
	bool isDone() { return currentImage >= images.size(); }
	void reset() {
		currentImage = 0;
		animationTime = 0;
		currentAlpha = 0;
	}
};

