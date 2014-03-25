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
	m = new menu(device, driver, smgr, guienv);
	m->run();
	// place camera and load map
	smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));
	mapterrain map = mapterrain(device, smgr);

	/*
	 *  !! IMPORTANT !! 
	 *  Both sides (both running games) need to keep track of BOTH players
	 *  at the point of the constructor of the game it cannot be said whether they are player one or two
	 *  this needs to be set when the game "starts" 
	 *  the hosting player will always be player 1, the joining player will always be player 2
	 *  after this has been set (after a button was clicked in the menu, i'm looking at you chris ;))
	 *  their units can be initialized, not before!
 	 */
	localPlayer = new Player(device);
	opposingPlayer = new Player(device);

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

		/*
		// setup event receiver to handle user input on menu            
		MenuEventReceiver receiver(context);

		// specify our custom event receiver in the device	
		device->setEventReceiver(&receiver);
		*/

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

void game::passTurn() {
	// read the units of each player
	// transform then into DTOs
	// serialize everything
	// send it it to opposing player
}

void game::receiveGameState() {
	// receive data from opposing player
}

void game::deserialize() {
	// deserialized data to DTOs (units and gamestate)
}

void game::updateGameState(){
	// update gamestate by updating all attributes in both players 
	// and updating which player is active
	// visually update what had happened (render again?)
}


