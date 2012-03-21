#pragma once

#include "SSSF_SourceCode\gui\ScreenGUI.h"
#include "ImageShower.h"

class PlayerGUI : public ScreenGUI, public ImageShower
{
private:
	OverlayImage* hpBar;
	int maxHPBarImages;
	OverlayImage* manaBar;
	int maxManaBarImages;

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

};

