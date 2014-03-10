#include "game.h"



game::game(void)
{
}

game::game(IrrlichtDevice* device, ISceneManager* smgr, IGUIEnvironment* guienv)
{
	//camera 
	 smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));


	 //make a new terrain
	//mapterrain map = mapterrain(device, smgr);

}

game::~game(void)
{
}


