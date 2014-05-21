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
	//guienv->addImage(
	
	// TODO: add text?!
	text1 = guienv->addStaticText(L"You are Player 1", rect<s32>(0, (height/ 30) * 1, (width / 5), (height/ 30) * 2), true);
	
	// add gui elements
	//guienv->addButton(rect<s32>((640 / 3), (480/ 30) * 8, (640 / 3) * 2, (480/ 30) * 11), 0, GUI_ID_JOIN_GAME, L"Join Game", L"Joins a game");
	//guienv->addButton(rect<s32>((640 / 3), (480/ 30) * 12, (640 / 3) * 2, (480/ 30) * 15), 0, GUI_ID_HOST_GAME, L"Host Game", L"Hosts a game");
	//guienv->addButton(rect<s32>((640 / 3), (480/ 30) * 16, (640 / 3) * 2, (480/ 30) * 19), 0, GUI_ID_JOIN_WSDL, L"Quick Match", L"Starts a quick match, opponent will be automatically assigned.");
	//guienv->addButton(rect<s32>((640 / 3), (480/ 30) * 20, (640 / 3) * 2, (480/ 30) * 23), 0, GUI_ID_START_GAME, L"Start Game", L"Start game without network for test");
}

void menu::setMainText(char t)
{
	text1->setText(L"You are Player 2");
	//text1->setOverrideFont();
	//text1->setText(t);
}
