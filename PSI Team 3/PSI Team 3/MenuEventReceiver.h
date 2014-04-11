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
		bool menuDone;
		wchar_t *text;
		char *ch;
		std::string s;

		MenuEventReceiver(SAppContext & context) : Context(context)
		{
			menuDone = false;
			unitList = context.game_->localPlayer->getUnits();
			smgr = context.device->getSceneManager();
			camera = smgr->getActiveCamera();
		};
		bool OnEvent(const SEvent& event);
		void setIsUnitSelected(bool value) {
			isUnitSelected = value;
		};
	    SAppContext & Context;
		int portNo;
		// members for the unit controll
	private:
		
		void MouseOverUnit();
		void setDirection(irr::EKEY_CODE keyCode);

		BaseUnit::direction moveDirection;
		bool isUnitSelected;
		bool isHoveringUnit;
		BaseUnit *selectedUnit;
		vector<BaseUnit*>* unitList;
		vector<BaseUnit*>* allUnits;
		ICameraSceneNode* camera;
		ISceneManager* smgr;
};