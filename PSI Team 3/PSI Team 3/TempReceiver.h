#pragma once
#include <irrlicht.h>
#include <io.h>
#include "mapterrain.h"
#include "BaseUnit.h"
#include "Player.h"
#include <vector>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace std;

// declare a struct to receive the player class
struct GameData
{
	//IrrlichtDevice *device;
};

class TempReceiver : public irr::IEventReceiver
{
private:
	void MouseOverUnit();
	bool isHoveringUnit;
	BaseUnit *selectedUnit;

	void selectUnit(BaseUnit *unit);
	// temp method for testing...
	void moveUnit();

public:
	TempReceiver(IrrlichtDevice *device, Player *player);
	~TempReceiver(void);

	bool OnEvent(const SEvent& event);

	IrrlichtDevice *device;
	ICameraSceneNode* camera;
	ISceneManager* smgr;
	ISceneCollisionManager* collManager;
	line3d<f32> line;
	ISceneNode* selectedNode;
	vector<BaseUnit*>* unitList;
};

