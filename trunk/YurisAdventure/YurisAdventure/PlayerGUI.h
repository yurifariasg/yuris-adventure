#pragma once

#include "SSSF_SourceCode\gui\ScreenGUI.h"
#include "ImageShower.h"

class PlayerGUI : public ScreenGUI, public ImageShower
{
private:
	// Player Bar
	OverlayImage* playerBar;
	OverlayImage* alternatePlayerBar;
	bool showAlternatePlayerBar;

	// HP and MP Bars
	OverlayImage* hpBar;
	int maxHPBarImages;
	OverlayImage* manaBar;
	int maxManaBarImages;

	// GameOver Screen
	unsigned int blackImageID;
	unsigned int gameOverImageID;
	int alpha;
	bool isPlayerDead;

	map<wstring, OverlayImage*> centerMessages;
	int count;

	// Information to put on screen
	int hpBarQuantity;
	int manaBarQuantity;
	OverlayImage* centerImage;

public:
	PlayerGUI(void);
	~PlayerGUI(void);
	void updateGUI(Game*);
	void addRenderItemsToRenderList(RenderList *renderList);

	void addHPBar(OverlayImage*, int);
	void addManaBar(OverlayImage*, int);

	void addCenterMessage(wstring, OverlayImage*);
	void showImage(wstring);

	void setPlayerBars(OverlayImage* bar1, OverlayImage* bar2) {
		playerBar = bar1;
		alternatePlayerBar = bar2;
	}

	void setBlackAndGameOverImageID(unsigned int i, unsigned int j) { blackImageID = i; gameOverImageID = j; }

};

