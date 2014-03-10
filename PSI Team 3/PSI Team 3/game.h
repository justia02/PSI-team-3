#pragma once
#include <irrlicht.h>

using namespace irr;


using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class game
{
public:
	IrrlichtDevice *device;

	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;

	game(void);
	~game(void);

	int run(void);
};

