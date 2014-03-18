#pragma once
#include <irrlicht.h>
#include "mapterrain.h"
#include <iostream>
#include <NonRealtimeNetworkingUtilities.h>

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

	const wchar_t *text;
	bool menudone;

	menu(void);
	~menu(void);

	int run(void);
	void hostGame();
	void joinGame(char* ipAddress);
	void receiveData();
	void sendData();
	void init();
};

