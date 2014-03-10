#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
//using namespace scene;
//using namespace video;
//using namespace io;
using namespace gui;

class TurnState
{
public:
	//string text;

	TurnState(void);
	TurnState(IrrlichtDevice* device);
	~TurnState(void);

	void TurnActive();
	void TurnPassive();
};

