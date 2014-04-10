#pragma once
#include <irrlicht.h>
#include "mapterrain.h"
#include <iostream>
#include <non-realtime-networking/NonRealtimeNetworkingUtilities.h>
//#include <non-realtime-networking/NonRealtimeNetworkingexception.h>
#include "game.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrlicht_nonrealtimenetworking;

// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
struct SAppContext
{
    IrrlichtDevice *device;
    s32 counter;
	NonRealtimeNetworkingUtilities* networkUtilities;
	game* game_;
};

// Define values to identify join game and host game buttons
enum
{
    GUI_ID_JOIN_GAME,
    GUI_ID_HOST_GAME,
	GUI_ID_JOIN_GAME_DONE,
	GUI_ID_JOIN_WSDL,
	GUI_ID_START_GAME
};


class MenuEventReceiver : public irr::IEventReceiver
{
	public:

		IGUIEditBox *box;

		wchar_t *text;
		char *ch;
		std::string s;

		MenuEventReceiver(SAppContext & context) : Context(context){ };
		bool OnEvent(const SEvent& event);
	    SAppContext & Context;
		int portNo;
};