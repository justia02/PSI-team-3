#include "game.h"
#include "MenuEventReceiver.h" // this include MUST NOT be in the .h file!
#include "Player.h"
#include "menu.h"
#include "IllegalStateException.h"

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
	gameState = new GameStateDTO(5);

	// initialize players
	localPlayer = new Player(device);
	opposingPlayer = new Player(device);

	//device->setEventReceiver(new TempReceiver(device, localPlayer));

	// run menu
	menu* m = new menu(device, driver, smgr, guienv);
	// m->run(this);
		smgr->addCameraSceneNode(0, vector3df(0,6,-8), vector3df(0,0,0));
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
		receiver.init(guienv, 640, 480);
		receiver.setIsUnitSelected(false);
		receiver.menuDone = false;

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

	networkUtilities->initializeWS("145.109.198.191");
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
		// passTurn();
	} else {
		networkUtilities->joinGame(ipAddress, portNumber); 

		// joining player is always player 2
		localPlayer->setPlayer1(false);
		opposingPlayer->setPlayer1(true);

		localPlayer->initUnits();
		opposingPlayer->initUnits();

		vector3d<float> temp = smgr->getActiveCamera()->getPosition();
		cout << temp.X << " " << temp.Y << " " << temp.Z;
		temp.Z = !temp.Z;
		temp.Y = 0;
		playerCamera->setCameraPos(temp, localPlayer->getPlayer1());

		try {
			updateGameState();
		}
		// We should have a nice error box!
		catch(NonRealtimeNetworkingException e) {
			std::cout << "Error: " << e.what() << std::endl;
		}

	}

}

void game::passTurn() {
	// in the game state both player's units will be contained --> allocate memory for all units
	gameState = new GameStateDTO(localPlayer->getUnits()->size() + opposingPlayer->getUnits()->size());
	gameState->setVictory(checkVictory());

	BaseUnitDTO* units = new BaseUnitDTO[localPlayer->getUnits()->size() + opposingPlayer->getUnits()->size()];
	int i = 0;

	// read units of this player
	std::cout<<"UNITS OF LOCAL PLAYER" << std::endl;

	for(std::vector<BaseUnit*>::iterator it = localPlayer->getUnits()->begin(); it != localPlayer->getUnits()->end(); ++it) {
		// create a DTO for each of them
		BaseUnitDTO tmp = BaseUnitDTO();
		tmp.setId((*it)->id);
		tmp.setX((*it)->position.X);
		tmp.setY((*it)->position.Y);
		tmp.setZ((*it)->position.Z);
		tmp.setPlayer(true);
		tmp.setHealth((*it)->health);

		// output properties of unit

		std::cout << "Unit ID: " << tmp.getId() << std::endl;
		std::cout << "Unit player: " << tmp.getPlayer() << std::endl;
		std::cout << "X: " << tmp.getX() << std::endl;
		std::cout << "Y: " << tmp.getY() << std::endl;
		std::cout << "Z: " << tmp.getZ() << std::endl << std::endl;
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
		tmp.setHealth((*it)->health);

		// output properties of unit
		std::cout << "Unit ID: " << tmp.getId() << std::endl;
		std::cout << "Unit player: " << tmp.getPlayer() << std::endl;
		std::cout << "X: " << tmp.getX() << std::endl;
		std::cout << "Y: " << tmp.getY() << std::endl;
		std::cout << "Z: " << tmp.getZ() << std::endl << std::endl;
		// put unitDTOs in list that is given to gamestateDTO
		units[i] = tmp;
		i++;
	}

	// put units in the game state DTO
	gameState->setUnits(units);
	// serialize the gamestateDTO (unitDTOs should be serialized along with them...)
	char* buffer = gameState->serializeGameState();
	
	if (gameState->getVictory()) {
		std::cout << "YOU WIN: " << gameState->getVictory() << std::endl;
		device->getGUIEnvironment()->addMessageBox(L"YOU WIN!", L"Congratulations, you win the game!", true, EMBF_OK);	
	}

	try {
		networkUtilities->setBuffer(buffer);
		networkUtilities->sendData();

		updateGameState();
	}
	catch(NonRealtimeNetworkingException e) {
		std::cout << "Error: " << e.what() << std::endl;
		device->getGUIEnvironment()->addMessageBox(L"Oops an Error", L"Something went wrong, probably connection lost", true, EMBF_OK);
	}

}

void game::updateGameState(){
	// create a GameStateDTO object and fill in data we received by deserializing it
	networkUtilities->receiveData();
	std::cout << "Buffer: " << networkUtilities->getBuffer() << std::endl;
	// gameState = new GameStateDTO();
	gameState->deserialize(networkUtilities->getBuffer());

	bool unitUpdated;
	// update gamestate by updating all attributes in both players
	for (int i=0; i < gameState->unitLength; i++) { // calc length of array
		BaseUnitDTO tmp = gameState->getUnits()[i];
		unitUpdated = false;

		// find referring unit
		for(std::vector<BaseUnit*>::iterator it = localPlayer->getUnits()->begin(); it != localPlayer->getUnits()->end(); ++it) {
			if (unitUpdated) break;
			if (tmp.getId() == (*it)->id) {
				if((*it)->player1 != localPlayer->getPlayer1())
					throw new IllegalStateException("Unit is not assigned correctly!");

				// update position attributes of unit
				(*it)->position.X = tmp.getX();
				(*it)->position.Y = tmp.getY();
				(*it)->position.Z = tmp.getZ();
				(*it)->health = tmp.getHealth();

				// later on -> update other attributes of the unit
				(*it)->node->setPosition((*it)->position);
				(*it)->updateHealthBar();

				unitUpdated = true;
			}
		}

		// find referring unit
		for(std::vector<BaseUnit*>::iterator it = opposingPlayer->getUnits()->begin(); it != opposingPlayer->getUnits()->end(); ++it) {
			if (unitUpdated) break;
			if (tmp.getId() == (*it)->id) {
				if((*it)->player1 != opposingPlayer->getPlayer1())
					throw new IllegalStateException("Unit is not assigned correctly!");

				// update position attributes of unit
				(*it)->position.X = tmp.getX();
				(*it)->position.Y = tmp.getY();
				(*it)->position.Z = tmp.getZ();
				(*it)->health = tmp.getHealth();

				// updates the unit's position visually on the map (hopefully)
				(*it)->node->setPosition((*it)->position);
				(*it)->updateHealthBar();

				unitUpdated = true;
			}
		}

		if (! unitUpdated) 
			throw new IllegalStateException("Unit is not assigned to a player.");		
	}
	// show message if player lost
	if (gameState->getVictory()) {
		std::cout << "YOU LOSE: " << gameState->getVictory() << std::endl;	
		device->getGUIEnvironment()->addMessageBox(L"YOU LOSE!", L"Your opponent won the game. You lose.", true, EMBF_OK);
	}

	// update which player is active (just invert)
	gameState->setPlayer1Turn(!gameState->getPlayer1Turn());
}

void game::init_map(IrrlichtDevice *device_map)
{
	//make a new terrain
	mapterrain map = mapterrain(device_map, smgr);
}

bool game::checkVictory() {
	// check victory conditions -- at the end of a turn
	bool victory = true;

	// #1 all units of opponent are dead
	for(vector<BaseUnit*>::iterator it = opposingPlayer->getUnits()->begin(); it != opposingPlayer->getUnits()->end(); ++it) {
		// if there is at least one of the opposing units is still alive, this condition isn't met
		if ((*it)->getHealth() > 0) {
			victory = false;
		}
	}

	// #2 opponent's base was captured --> local unit is on opponent's base for at least 2 turns
	bool isOnBase = false;
	for(vector<BaseUnit*>::iterator it = localPlayer->getUnits()->begin(); it != localPlayer->getUnits()->end(); ++it) {
		for (int i = 0; i < 2; i++) {
			if (opposingPlayer->basePositions[i] == (*it)->position) {
				isOnBase = true;
				(*it)->onBaseCounter++;
				if ((*it)->onBaseCounter > 1) {
					victory = true;
				}
			}
		}
		// reset the counter if unit is not on a base field
		if (!isOnBase) {
			(*it)->onBaseCounter = 0;
		}
	}

	return victory;
}

/*
bool game::checkDefeat() {
	// check defeat conditions (invert victory conditions) -- at the beginning of a turn
	bool defeat = true;

	// #1 all my units are dead
	for(vector<BaseUnit*>::iterator it = localPlayer->getUnits()->begin(); it != localPlayer->getUnits()->end(); ++it) {
		// if there is at least one of my units is still alive, this condition isn't met
		if ((*it)->getHealth() > 0) {
			defeat = false;
		}
	}

	// #2 my base was captured --> opposing unit is on my base for at least 2 turns
	bool isOnBase = false;
	for(vector<BaseUnit*>::iterator it = opposingPlayer->getUnits()->begin(); it != opposingPlayer->getUnits()->end(); ++it) {
		for (int i = 0; i < 2; i++) {
			if (localPlayer->basePositions[i] == (*it)->position) {
				isOnBase = true;
				(*it)->onBaseCounter++;
				if ((*it)->onBaseCounter > 1) {
					defeat = true;
				}
			}
		}
		// reset the counter if unit is not on a base field
		if (!isOnBase) {
			(*it)->onBaseCounter = 0;
		}
	}

	return defeat;
}
*/


