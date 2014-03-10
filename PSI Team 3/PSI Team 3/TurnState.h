#pragma once
#include <irrlicht.h>


using namespace irr;
using namespace core;
using namespace gui;

class TurnState
{
public:
	//iostream::string text;

	TurnState(void);
	TurnState(IrrlichtDevice* device);
	~TurnState(void);

	void TurnActive();
	void TurnPassive();
};

