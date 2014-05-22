#pragma once
#include <irrlicht.h>
#include <menuReceiver.h>
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


class MenuEventReceiver : public irr::menuReceiver
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
			selectedUnit = hoveredUnit = NULL;
			allUnits = NULL;
			shootingMode = false;
		};
		bool OnEvent(const SEvent& event);

		void setIsUnitSelected(bool value) {
			isUnitSelected = value;
		};
		void setObstacles(std::vector<Obstacle*>* obstacles) {
			this->obstacles = obstacles;
		};
	    SAppContext & Context;

		//implementation of the functions from the engine.
		void JOIN_GAME();
		void HOST_GAME();
		void JOIN_GAME_SECOND();
		void JOIN_WSDL();
		void START_GAME();

		std::string getSelectedUnitData(BaseUnit *unit);

		int portNo;
		// members for the unit controll
	private:
		
		void MouseOverUnit();
		void setDirection(irr::EKEY_CODE keyCode);
		

		BaseUnit::direction moveDirection;
		bool isUnitSelected;
		bool isHoveringUnit;
		BaseUnit *selectedUnit;
		BaseUnit* hoveredUnit;
		vector<BaseUnit*>* unitList;
		vector<BaseUnit*>* allUnits;
		vector<Obstacle*>* obstacles;
		ICameraSceneNode* camera;
		ISceneManager* smgr;
		bool shootingMode;
		std::wstring* unitModeLabelText;

};