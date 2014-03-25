#include "game.h"
#include "MenuEventReceiver.h";
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
	m = new menu(device, driver, smgr, guienv, networkUtilities);
	

	// place camera and load map
	//smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));
	mapterrain map = mapterrain(device, smgr);
	Player* player = new Player(device);
	player->setPlayer1(true);
	player->initUnits();

	//networkUtilities = new NonRealtimeNetworkingUtilities();
	//gameStateDTO = new GameStateDTO(4);
	//gameStateDTO->setUnits(initializeUnits());

	//camera 
	//smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));


	//make a new terrain
	//mapterrain map = mapterrain(device, smgr);




}

game::~game(void)
{
}

int game::run(void)
{

		//m->run(device);

		// setup menu
		SAppContext context;
		context.device = device;
		context.counter = 0;
		context.networkUtilities = networkUtilities;

		// setup event receiver to handle user input on menu            
		MenuEventReceiver receiver(context);

		// specify our custom event receiver in the device	
		device->setEventReceiver(&receiver);

		while (device->run() && driver)
		if (device->isWindowActive())
		{
			
			//device->run();
			driver->beginScene(true, true, SColor(0,200,200,200));
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();
		}
		device->drop();
	
	return 0;
}


