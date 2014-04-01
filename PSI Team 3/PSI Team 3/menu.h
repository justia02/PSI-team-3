#pragma once
#include <irrlicht.h>
#include "mapterrain.h"
#include <iostream>
#include <non-realtime-networking/NonRealtimeNetworkingUtilities.h>
#include <non-realtime-networking/NonRealtimeNetworkingException.h>
#include <iostream>
#include "game.h"

using namespace irr;
using namespace irrlicht_nonrealtimenetworking;


using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/** class for a networking menu
*   to provide facilities to establish a connection between two users
*/
class menu
{
public:
	IrrlichtDevice *device;
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	//NonRealtimeNetworkingUtilities* networkUtilities;


	const wchar_t *text;

	menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv);
	~menu(void);
	/*IrrlichtDevice* device,*/
	void run(game* g);
	void init();
};

