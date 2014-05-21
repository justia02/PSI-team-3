#pragma once
#include <irrlicht.h>
#include "Player.h"

using namespace irr;
using namespace scene;

class mapterrain
{
public:
	mapterrain(void);

	mapterrain(IrrlichtDevice *device, ISceneManager* smgr, std::vector<Obstacle*>* obstacles);

	~mapterrain(void);

	void loadBases(ISceneManager* smgr);


};

