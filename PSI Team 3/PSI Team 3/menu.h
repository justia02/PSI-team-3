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

	IGUIStaticText *text1;

	int height;
	int width;

	menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv, int horizontal, int vertical);
	~menu(void);

	void init();
	void setMainText(char t);
};

