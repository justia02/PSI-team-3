#pragma once
#include <irrlicht.h>
#include <menuReceiver.h>
#include "mapterrain.h"
#include <iostream>
#include <non-realtime-networking/NonRealtimeNetworkingUtilities.h>
#include "game.h"
#include "GridNode.h"
#include <time.h>

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
		time_t timeSincePassTurn;
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
			selectedUnit = hoveredUnit = unitToBeShot = NULL;
			hoveredGridNode = NULL;
			allUnits = NULL;
			shootingMode = false;
			fieldAvailibleMesh = NULL;
			highlightedNodes = new std::vector<GridNode*>();
			highlightedEnemies = new std::vector<BaseUnit*>();
		};

		bool OnEvent(const SEvent& event);

		void setIsUnitSelected(bool value) {
			isUnitSelected = value;
		};
		void setObstacles(std::vector<Obstacle*>* obstacles) {
			this->obstacles = obstacles;
		};
		void setFields(std::vector<vector3df>* fields) {
			this->fields = fields;
		};

		void checkPossibleMoves();
		void clearPossibleMoves();
		void checkOpponentsToShoot();
		void clearHighlightedEnemies();

	    SAppContext & Context;

		//implementation of the functions from the engine.
		bool JOIN_GAME();
		bool HOST_GAME();
		bool JOIN_GAME_SECOND();
		bool END_GAME();

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
		BaseUnit* unitToBeShot;
		GridNode* hoveredGridNode;
		vector<BaseUnit*>* unitList;
		vector<BaseUnit*>* allUnits;
		vector<Obstacle*>* obstacles;
		vector<vector3df>* fields;
		ICameraSceneNode* camera;
		ISceneManager* smgr;
		bool shootingMode;
		std::wstring* unitModeLabelText;

		irr::scene::IAnimatedMesh* fieldAvailibleMesh;
		irr::scene::IAnimatedMesh* getMesh() {
			if (fieldAvailibleMesh == NULL)
				fieldAvailibleMesh = Context.device->getSceneManager()->getMesh("../media/gridNode.irrmesh");
			return fieldAvailibleMesh;
		};

		std::vector<BaseUnit*>* getAllUnits() {

			// Create a list of all units if such doesn't exist yet
			if (allUnits == NULL) {
				allUnits = new std::vector<BaseUnit*>();
				allUnits->insert(allUnits->end(), unitList->begin(), unitList->end());
				allUnits->insert(allUnits->end(), Context.game_->opposingPlayer->getUnits()->begin(), Context.game_->opposingPlayer->getUnits()->end());
			}

			return allUnits;

		}

		std::vector<GridNode*>* highlightedNodes;
		std::vector<BaseUnit*>* highlightedEnemies;
};