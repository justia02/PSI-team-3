#pragma once
#include <irrlicht.h>
#include "mapterrain.h";
#include <iostream>;

using namespace irr;


using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
class menu
{
public:
	IrrlichtDevice *device;

	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;

	const wchar_t* text2;

	menu(void);
	~menu(void);

	int run(void);


};

