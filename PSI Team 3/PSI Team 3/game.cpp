#include "game.h"
#include "menu.h"
#include "Player.h"

game::game(void)
{
	//create the device
	device = createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);

	device->setWindowCaption(L"PSI TEAM 3");
	device->setResizable(false);

	// initialize driver, scenemanager and gui environment
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	networkUtilities = new NonRealtimeNetworkingUtilities();

	// run menu
	menu *m = new menu(device, driver, smgr, guienv, networkUtilities);
	m->run();

	// place camera and load map
	smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));
	mapterrain map = mapterrain(device, smgr);

	Player* pl = new Player(device);
	//networkUtilities = new NonRealtimeNetworkingUtilities();
	//gameStateDTO = new GameStateDTO(4);
	//gameStateDTO->setUnits(initializeUnits());

	//camera 
	//smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));


	//make a new terrain
	//mapterrain map = mapterrain(device, smgr);

		while (device->run() && driver)
	{
		if (device->isWindowActive())
		{
			;
			//device->run();
			driver->beginScene(true, true, SColor(0,200,200,200));
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();
		}
	}


}

game::~game(void)
{
}


