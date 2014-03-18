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

// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
struct SAppContext
{
    IrrlichtDevice *device;
    s32             counter;
};

// Define values to identify join game and host game buttons
enum
{
    GUI_ID_JOIN_GAME,
    GUI_ID_HOST_GAME
};


class MenuEventReceiver : public irr::IEventReceiver
{
	public:
		MenuEventReceiver(SAppContext & context) : Context(context){ };
		bool OnEvent(const SEvent& event);
	    SAppContext & Context;
};