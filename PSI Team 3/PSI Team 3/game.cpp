#include "game.h"
#include "MenuEventReceiver.h"; // this include MUST NOT be in the .h file!
#include "Player.h"
#include "menu.h"

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
	playerCamera = new PlayerCamera(device);

	// initialize networkUtilities (all networking stuff is handled in game)
	networkUtilities = new NonRealtimeNetworkingUtilities();

	// initialize gameStateDTO
	gameState = new GameStateDTO();

	// initialize players
	localPlayer = new Player(device);
	opposingPlayer = new Player(device);

	//device->setEventReceiver(new TempReceiver(device, localPlayer));

	// run menu
	menu* m = new menu(device, driver, smgr, guienv);
	//m->run(this);

	// TEST --> should only serialize game state to file (seralizationFileGameState)
	passTurn();


	// place camera and load map
	//smgr->addCameraSceneNode(0, vector3df(0,8,-8), vector3df(0,0,0));
	//mapterrain map = mapterrain(device, smgr);

	// adjust camera according to players view (different for player 1 and 2)
	if(!localPlayer->getPlayer1()){
		vector3d<float> temp = smgr->getActiveCamera()->getPosition();
		cout << temp.X << " " << temp.Y << " " << temp.Z;
		temp.Z = !temp.Z;
		temp.Y = 0;
		playerCamera->setCameraPos(temp, localPlayer->getPlayer1());
	}
	//networkUtilities = new NonRealtimeNetworkingUtilities();
	//gameStateDTO = new GameStateDTO(4);
	//gameStateDTO->setUnits(initializeUnits());

	//camera 
	smgr->addCameraSceneNode(0, vector3df(0,7,-8), vector3df(0,0,0));

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
		context.game_ = this;

		
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

void game::startGame() {

	networkUtilities->initializeWS("145.109.197.6");
	networkUtilities->setGameName("PSI Team 3");
	networkUtilities->registerOnTheServer();
	if ((networkUtilities->getSessionId() % 2) == 1)
		startGame(true);
	else
		startGame(false, networkUtilities->getOpponentsIpAddress());

}
/**	
  * starts the game from the perspective of player1/player2
  */
void game::startGame(bool asPlayer1, char* ipAddress) {

	if (asPlayer1) {
		networkUtilities->hostGame(portNumber);

		// hosting player is always player 1
		localPlayer->setPlayer1(true);
		opposingPlayer->setPlayer1(false);

		localPlayer->initUnits();
		opposingPlayer->initUnits();

	} else {
		networkUtilities->joinGame(ipAddress, portNumber); 

		// joining player is always player 2
		localPlayer->setPlayer1(false);
		opposingPlayer->setPlayer1(true);

		localPlayer->initUnits();
		opposingPlayer->initUnits();
	}
}

void game::passTurn() {

	// in the game state both player's units will be contained --> allocate memory for all units
	gameState = new GameStateDTO(localPlayer->getUnits()->size() + opposingPlayer->getUnits()->size());
	BaseUnitDTO* units = new BaseUnitDTO[localPlayer->getUnits()->size() + opposingPlayer->getUnits()->size()];
	int i = 0;

	// read units of this player
	std::cout<<"UNITS OF LOCAL PLAYER";
	std::cout<<"\n";
	for(std::vector<BaseUnit*>::iterator it = localPlayer->getUnits()->begin(); it != localPlayer->getUnits()->end(); ++it) {
		// create a DTO for each of them
		BaseUnitDTO tmp = BaseUnitDTO();
		tmp.setId((*it)->id);
		tmp.setX((*it)->position.X);
		tmp.setY((*it)->position.Y);
		tmp.setZ((*it)->position.Z);
		tmp.setPlayer(true);

		// output properties of unit

		std::cout<<tmp.getId();
		std::cout<<"\n";
		std::cout<<tmp.getPlayer();
		std::cout<<"\n";
		std::cout<<tmp.getX();
		std::cout<<"\n";
		std::cout<<tmp.getY();
		std::cout<<"\n";
		std::cout<<tmp.getZ();
		std::cout<<"\n";
		std::cout<<"\n";
		// put unitDTOs in list that is given to gamestateDTO
		units[i] = tmp;
		i++;
	}

	// read units of opponent
	std::cout<<"UNITS OF OPPOSING PLAYER";
	std::cout<<"\n";
	for(std::vector<BaseUnit*>::iterator it = opposingPlayer->getUnits()->begin(); it != opposingPlayer->getUnits()->end(); ++it) {
		// create a DTO for each of them
		BaseUnitDTO tmp = BaseUnitDTO();
		tmp.setId((*it)->id);
		tmp.setX((*it)->position.X);
		tmp.setY((*it)->position.Y);
		tmp.setZ((*it)->position.Z);
		tmp.setPlayer(false);

		// output properties of unit
		std::cout<<tmp.getId();
		std::cout<<"\n";
		std::cout<<tmp.getPlayer();
		std::cout<<"\n";
		std::cout<<tmp.getX();
		std::cout<<"\n";
		std::cout<<tmp.getY();
		std::cout<<"\n";
		std::cout<<tmp.getZ();
		std::cout<<"\n";
		std::cout<<"\n";
		// put unitDTOs in list that is given to gamestateDTO
		units[i] = tmp;
		i++;
	}

	// put units in the game state DTO
	gameState->setUnits(units);

	// serialize the gamestateDTO (unitDTOs should be serialized along with them...)
	char* buffer = gameState->serializeGameState();
	std::cout<<buffer;
	// send it it to opposing player
	// networkUtilities->setBuffer(buffer);
	// networkUtilities->sendData();
}

void game::receiveGameState() {
	// receive data from opposing player
	networkUtilities->receiveData();
	// deserialize data
}

void game::deserialize() {
	// deserialized data to DTOs (units and gamestate)
	std::string buffer = networkUtilities->getBuffer();
	gameState->deserialize(buffer);
}

void game::updateGameState(){
	// create a GameStateDTO object and fill in data we received by deserializing it
	gameState = new GameStateDTO();
	deserialize();

	// update gamestate by updating all attributes in both players


	// and updating which player is active
	// visually update what had happened (render again?)
}

void game::init_map(IrrlichtDevice *device_map)
{
	//make a new terrain
	mapterrain map = mapterrain(device_map, smgr);
}


