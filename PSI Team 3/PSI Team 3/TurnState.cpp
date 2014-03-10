#include "TurnState.h"

TurnState::TurnState(void)
{
}

TurnState::TurnState(IrrlichtDevice* device)
{
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	//guienv->addStaticText(text,
		//rect<s32>(10,10,260,22), true);

	//TurnActive(guienv);
}

TurnState::~TurnState(void)
{
}

void TurnState::TurnActive()
{
	

}

void TurnState::TurnPassive()
{

}
