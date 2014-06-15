#pragma once
#include <irrlicht.h>
#include "mapterrain.h"
#include <iostream>

using namespace irr;



using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


/** class for the ingame menu
*   this menu will be visible for every player so that they can see important information about the game
*/
class menu
{
public:
	IrrlichtDevice *device;
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	wchar_t *text;

	IGUIFont *Font;
	IGUIStaticText *playerText;
	IGUIStaticText *turnText;
	IGUIStaticText *actionText;
	IGUIStaticText *unitText;
	IGUIStaticText *waitText;

	ITexture *action0;
	ITexture *action1;
	ITexture *action2;
	ITexture *action3;
	ITexture *action4;
	IGUIImage *action_image;

	int height;
	int width;

	menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv, int horizontal, int vertical);
	~menu(void);

	void init();
	void setPlayerText(std::string playertext);
	void setActionText(std::string actiontext);
	void setTurnText(std::string turntext);
	void setUnitText(std::string unittext);
	void setWaitText(bool wait);
	void menu::setActionImage(int number);
};

