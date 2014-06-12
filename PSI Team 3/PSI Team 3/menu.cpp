#include "menu.h"

/**
  * Implementation of menu
  */

menu::menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv, int horizontal, int vertical) 
{
	this->device = device;
	this->driver = driver;
	this->smgr = smgr;
	this->guienv = guienv;


	width = horizontal;
	height = vertical;

	// initalizes the in game info menu
	init();
}

menu::~menu(void)
{
}


/**
 * inializes the graphical menu
 */
void menu::init(void)
{
	//put font for the texts
	Font = guienv->getFont("../media/fonts/candara14.bmp");

	// Make the texts that will always be available
	playerText = guienv->addStaticText(L"You are Player 1", rect<s32>((width / 7), (height/ 30) * 1, (width / 7) * 3, (height/ 30) * 2), false);
	playerText->setOverrideFont(Font);

	turnText = guienv->addStaticText(L"It is your turn", rect<s32>((width / 7), (height/ 30) * 2, (width / 7) * 3, (height/ 30) * 3), false);
	turnText->setOverrideFont(Font);

	actionText = guienv->addStaticText(L"Actions left = 4", rect<s32>((width / 7), (height/ 30) * 3, (width / 7) * 3, (height/ 30) * 4), false);
	actionText->setOverrideFont(Font);

	unitText = guienv->addStaticText(L"Click on a unit to see his stats", rect<s32>((width / 3) , (height/ 30) * 1, (width/ 3) * 2, (height/ 30) * 4), false, false, NULL, 1, true);
	unitText->setOverrideFont(Font);

	modeText = guienv->addStaticText(L"Moving Mode ", rect<s32>((width / 7) * 5, (height/ 30) * 1, (width) , (height/ 30) * 2), false);
	modeText->setOverrideFont(Font);

	waitText = guienv->addStaticText(L" ", rect<s32>((width / 3) , (height/ 30) * 26, (width / 3) * 2, height), false);
	waitText->setOverrideFont(Font);
}

void menu::setPlayerText(std::string playertext)
{
	playerText->setText(std::wstring(playertext.begin(), playertext.end()).c_str());
}

void menu::setActionText(std::string actiontext)
{
	actionText->setText(std::wstring(actiontext.begin(), actiontext.end()).c_str());
}

void menu::setTurnText(std::string turntext)
{
	turnText->setText(std::wstring(turntext.begin(), turntext.end()).c_str());
}

void menu::setUnitText(std::string unittext)
{
	unitText->setText(std::wstring(unittext.begin(), unittext.end()).c_str());
}

void menu::setModeText(std::string modetext)
{
	modeText->setText(std::wstring(modetext.begin(), modetext.end()).c_str());
}

void menu::setWaitText(bool wait)
{
	if(wait)
	{
		waitText->setText(L"Please Wait!!!");
		waitText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	}
	else{
		waitText->setText(L" ");
	}
}
