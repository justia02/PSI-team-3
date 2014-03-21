#pragma once
#include <irrlicht.h>
#include "mapterrain.h"
#include <iostream>
#include <NonRealtimeNetworkingUtilities.h>
#include "BaseUnit.h"

using namespace irr;
using namespace irrlicht_nonrealtimenetworking;


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
	NonRealtimeNetworkingUtilities* networkUtilities;
	BaseUnit *unit1;
	BaseUnit *unit2;

	const wchar_t *text;
	bool menudone;

	menu(void);
	~menu(void);

	int run(void);
	void init();
};

