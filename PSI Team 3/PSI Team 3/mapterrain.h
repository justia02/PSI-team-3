#pragma once
#include <irrlicht.h>
#include "Player.h"

using namespace irr;
using namespace scene;

class mapterrain
{
public:
	mapterrain(void);

	mapterrain(IrrlichtDevice *device, ISceneManager* smgr);

	~mapterrain(void);

	void loadBases(ISceneManager* smgr);


};

