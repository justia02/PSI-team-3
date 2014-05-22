#include "game.h"
#include "MenuEventReceiver.h" 
#include "Player.h"
//#include "menu.h"
#include "IllegalStateException.h"
#include <non-realtime-networking/NonRealtimeNetworkingException.h>
#include "wtypes.h"

game::game(void)
{
	//create the device
	//and get desktop width and height
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	//assign the width and height
	horizontal = desktop.right;
	vertical = desktop.bottom;

	device = createDevice( video::EDT_OPENGL, dimension2d<u32>(horizontal, vertical), 16, false, false, false, 0);

	device->setWindowCaption(L"PSI TEAM 3");
	device->setResizable(false);

	// initialize driver, scenemanager and gui environment
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	playerCamera = new PlayerCamera(device);

	// initialize networkUtilities (all networking stuff is handled in in this class)
	networkUtilities = new NonRealtimeNetworkingUtilities();

	// initialize gameStateDTO
	gameState = new GameStateDTO(5);

	// initialize players
	localPlayer = new Player(device);
	opposingPlayer = new Player(device);


	smgr->addCameraSceneNode(0, vector3df(0,6,-8), vector3df(0,0,0));

	// unitModeLabel = guienv->getBuiltInFont();
	unitModeLabel = guienv->getFont("../media/fonts/candara14.bmp");
	unitModeLabelText = new std::wstring(L"");

}

game::~game(void)
{
}

int game::run(void)
{

		// setup context
		SAppContext context;
		context.device = device;
		context.counter = 0;
		context.networkUtilities = networkUtilities;
		context.game_ = this;
		
		// setup event receiver to handle user input on menu            
		MenuEventReceiver receiver(context);
		receiver.init(guienv, horizontal, vertical);
		receiver.setIsUnitSelected(false);
		receiver.setUnitModeLabelText(unitModeLabelText);
		receiver.menuDone = false;

		// Create obstacles
		std::vector<Obstacle*>* obstacles = new std::vector<Obstacle*>();
		obstacles->push_back(new Obstacle(type::PYRAMID, context.device));
		obstacles->push_back(new Obstacle(type::SPIDER, context.device));
		obstacles->push_back(new Obstacle(type::CAT, context.device));
		receiver.setObstacles(obstacles);

		// specify our custom event receiver in the device	
		device->setEventReceiver(&receiver);
		

		while (device->run() && driver)
		if (device->isWindowActive())
		{
			
			//device->run();
			driver->beginScene(true, true, SColor(0,200,200,200));
			smgr->drawAll();
			guienv->drawAll();
			if (receiver.menuDone) {
				unitModeLabel->draw((*unitModeLabelText).c_str(), core::rect<s32>(20,20,0,0), video::SColor(255,100,100,100));
			}
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
		localPlayer->setActionsLeft();
		// passTurn();
	} else {
		networkUtilities->joinGame(ipAddress, portNumber); 

		// joining player is always player 2
		localPlayer->setPlayer1(false);
		opposingPlayer->setPlayer1(true);

		localPlayer->initUnits();
		opposingPlayer->initUnits();

		vector3d<float> temp = smgr->getActiveCamera()->getPosition();
		temp.Z = !temp.Z;
		temp.Y = 0;
		playerCamera->setCameraPos(temp, localPlayer->getPlayer1());

		try {
			
			pthread_create(&thread, NULL, updateGameState, this);
			//updateGameState();
		}
		// We should have a nice error box!
		catch(NonRealtimeNetworkingException e) {
	
		}

	}

}

void game::passTurn(bool giveUp) {
	std::cout<<"PASS TURN!";
	// in the game state both player's units will be contained --> allocate memory for all units
	gameState = new GameStateDTO(localPlayer->getUnits()->size() + opposingPlayer->getUnits()->size());
	gameState->setVictory(checkVictory());
	gameState->setGiveUp(giveUp);

	BaseUnitDTO* units = new BaseUnitDTO[localPlayer->getUnits()->size() + opposingPlayer->getUnits()->size()];
	int i = 0;

	// read units of this player

	for(std::vector<BaseUnit*>::iterator it = localPlayer->getUnits()->begin(); it != localPlayer->getUnits()->end(); ++it) {
		// create a DTO for each of them
		BaseUnitDTO tmp = BaseUnitDTO();
		tmp.setId((*it)->id);
		tmp.setX((*it)->position.X);
		tmp.setY((*it)->position.Y);
		tmp.setZ((*it)->position.Z);
		tmp.setPlayer(true);
		tmp.setHealth((*it)->health);
		std::cout<<"PASS TURN!";
		// put unitDTOs in list that is given to gamestateDTO
		units[i] = tmp;
		i++;
	}

	// read units of opponent
	for(std::vector<BaseUnit*>::iterator it = opposingPlayer->getUnits()->begin(); it != opposingPlayer->getUnits()->end(); ++it) {
		// create a DTO for each of them
		BaseUnitDTO tmp = BaseUnitDTO();
		tmp.setId((*it)->id);
		tmp.setX((*it)->position.X);
		tmp.setY((*it)->position.Y);
		tmp.setZ((*it)->position.Z);
		tmp.setPlayer(false);
		tmp.setHealth((*it)->health);
		// put unitDTOs in list that is given to gamestateDTO
		units[i] = tmp;
		i++;
	}

	// put units in the game state DTO
	gameState->setUnits(units);
	// serialize the gamestateDTO (unitDTOs should be serialized along with them...)
	char* buffer = gameState->serializeGameState();
	if (gameState->getVictory()) {
		device->getGUIEnvironment()->addMessageBox(L"YOU WIN!", L"Congratulations, you win the game!", true, EMBF_OK);
		endOfGame = true;
	}
	if (gameState->getGiveUp()) {
		device->getGUIEnvironment()->addMessageBox(L"YOU SURRENDER!", L"Your opponent won the game. You surrendered.", true, EMBF_OK);
		endOfGame = true;
	}

	try {
		networkUtilities->setBuffer(buffer);
		networkUtilities->sendData();
		//updateGameState();
		pthread_create(&thread, NULL, updateGameState, this);
	}
	catch(NonRealtimeNetworkingException e) {
		device->getGUIEnvironment()->addMessageBox(L"Oops an Error", L"Something went wrong, probably connection lost", true, EMBF_OK);
		endOfGame = true;
	}
}

void * game::updateGameState(void * g){

	// cast parameter to game
	game* gm = (game*) g;

	// create a GameStateDTO object and fill in data we received by deserializing it
	gm->networkUtilities->receiveData();
	// gameState = new GameStateDTO();
	gm->gameState->deserialize(gm->networkUtilities->getBuffer());

	bool unitUpdated;
	// update gamestate by updating all attributes in both players
	for (int i=0; i < gm->gameState->unitLength; i++) { // calc length of array
		BaseUnitDTO tmp = gm->gameState->getUnits()[i];
		unitUpdated = false;

		// find referring unit
		for(std::vector<BaseUnit*>::iterator it = gm->localPlayer->getUnits()->begin(); it != gm->localPlayer->getUnits()->end(); ++it) {
			if (unitUpdated) break;
			if (tmp.getId() == (*it)->id) {
				if((*it)->player1 != gm->localPlayer->getPlayer1())
					throw new IllegalStateException("Unit is not assigned correctly!");

				// update position attributes of unit
				(*it)->position.X = tmp.getX();
				(*it)->position.Y = tmp.getY();
				(*it)->position.Z = tmp.getZ();
				(*it)->health = tmp.getHealth();

				// later on -> update other attributes of the unit
				(*it)->node->setPosition((*it)->position);
				(*it)->updateHealthBar();
				(*it)->setHasMoved(false);
				(*it)->setHasShot(false);

				unitUpdated = true;
			}
		}

		// find referring unit
		for(std::vector<BaseUnit*>::iterator it = gm->opposingPlayer->getUnits()->begin(); it != gm->opposingPlayer->getUnits()->end(); ++it) {
			if (unitUpdated) break;
			if (tmp.getId() == (*it)->id) {
				if((*it)->player1 != gm->opposingPlayer->getPlayer1())
					throw new IllegalStateException("Unit is not assigned correctly!");

				// update position attributes of unit
				(*it)->position.X = tmp.getX();
				(*it)->position.Y = tmp.getY();
				(*it)->position.Z = tmp.getZ();
				(*it)->health = tmp.getHealth();

				// updates the unit's position visually on the map (hopefully)
				(*it)->node->setPosition((*it)->position);
				(*it)->updateHealthBar();
				(*it)->setHasMoved(false);
				(*it)->setHasShot(false);

				unitUpdated = true;
			}
		}

		if (! unitUpdated) 
			throw new IllegalStateException("Unit is not assigned to a player.");		
	}

	// show message if player lost
	if (gm->gameState->getVictory()) {		
		gm->device->getGUIEnvironment()->addMessageBox(L"YOU LOSE!", L"Your opponent won the game. You lose.", true, EMBF_OK);
		gm->endOfGame = true;
	}
	if(gm->gameState->getGiveUp()) {
		gm->device->getGUIEnvironment()->addMessageBox(L"YOU WIN!", L"Your opponent surrendered.", true, EMBF_OK);
		gm->endOfGame = true;
	}

	// update which player is active (just invert)
	gm->gameState->setPlayer1Turn(!gm->gameState->getPlayer1Turn());

	if(gm->localPlayer->getPlayer1() && gm->gameState->getPlayer1Turn()){
		gm->localPlayer->resetActionsLeft();
	} else if(!gm->localPlayer->getPlayer1() && !gm->gameState->getPlayer1Turn()){
		gm->localPlayer->resetActionsLeft();
	}
}

/*
void game::updateGameState(){
 // create a GameStateDTO object and fill in data we received by deserializing it
 networkUtilities->receiveData();
 // networkUtilities->receiveDataThread();
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
    (*it)->setHasMoved(false);
    (*it)->setHasShot(false);

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
    (*it)->setHasMoved(false);
    (*it)->setHasShot(false);

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
  endOfGame = true;
 }
 if(gameState->getGiveUp()) {
  std::cout << "YOU WIN: " << gameState->getVictory() << std::endl; 
  device->getGUIEnvironment()->addMessageBox(L"YOU WIN!", L"Your opponent surrendered.", true, EMBF_OK);
  endOfGame = true;
 }

 // update which player is active (just invert)
 gameState->setPlayer1Turn(!gameState->getPlayer1Turn());

 if(localPlayer->getPlayer1() && gameState->getPlayer1Turn()){
  localPlayer->resetActionsLeft();
 } else if(!localPlayer->getPlayer1() && !gameState->getPlayer1Turn()){
  localPlayer->resetActionsLeft();
 }
}
*/
void game::init_map(IrrlichtDevice* device_map, std::vector<Obstacle*>* obstacles)
{
	//make a new terrain	
	mapterrain map = mapterrain(device_map, smgr, obstacles);
	
}

void game::init_ingame_menu()
{
	m = new menu(device, driver, smgr, guienv, horizontal, vertical);
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
				if ((*it)->onBaseCounter > 2) {
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

void game::resetGame() {
	// release some memory...	
	delete localPlayer;
	delete opposingPlayer;
	guienv->clear();
	smgr->clear();

	// reset all the relevant properties
	localPlayer = new Player(device);
	opposingPlayer = new Player(device);
	networkUtilities = new NonRealtimeNetworkingUtilities();
	endOfGame = false;
	gameState = new GameStateDTO(5);

	// re-run menu + game
	m->init();
	smgr->addCameraSceneNode(0, vector3df(0,6,-8), vector3df(0,0,0));
	this->run();
}
