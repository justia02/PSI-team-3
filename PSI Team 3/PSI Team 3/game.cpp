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

	device = createDevice(video::EDT_OPENGL, dimension2d<u32>(horizontal, vertical), 16, false, false, false, 0);

	device->setWindowCaption(L"PSI TEAM 3");
	device->setResizable(false);

	// initialize driver, scenemanager and gui environment
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	playerCamera = new PlayerCamera(device);

	IGUISkin* skin = guienv->getSkin();
	IGUIFont* font = guienv->getFont("../media/fonts/candara14.bmp");
	skin->setFont(font);

	// initialize networkUtilities (all networking stuff is handled in in this class)
	networkUtilities = new NonRealtimeNetworkingUtilities();

	// initialize gameStateDTO
	gameState = new GameStateDTO(16);

	// initialize players
	localPlayer = new Player(device);
	opposingPlayer = new Player(device);

	fields = new std::vector<vector3df>();

	smgr->addCameraSceneNode(0, vector3df(0,8,-8), vector3df(0,0,0));


}

game::~game(void)
{
}

void game::init_names()
{

	names[0] = L"test1";
	names[1] = L"test2";
	names[2] = L"test3";
	names[3] = L"test4";
	names[4] = L"test5";
	names[5] = L"test6";

	sub_names[0] = L"test";
	sub_names[1] = L"test";
	sub_names[2] = L"test";
	sub_names[3] = L"test";
	sub_names[4] = L"test";
	sub_names[5] = L"test";
}

rect<s32> makeButton(int width, int height, int i)
{

	rect<s32> button = rect<s32>((width / 3), (height/ 35) * ((i * 5) + 1), (width / 3) * 2, (height/ 35) * ((i * 5) + 5));

	return button;
}

int game::run(void)
{
		endOfGame = false;
		// setup context
		SAppContext context;
		context.device = device;
		context.counter = 0;
		context.networkUtilities = networkUtilities;
		context.game_ = this;
		
		// setup event receiver to handle user input on menu            
		MenuEventReceiver receiver(context);
		receiver.init(guienv, horizontal, vertical);

		//init_names();
		//receiver.init_custom(guienv, horizontal, vertical, 6, names, sub_names);//, (menuReceiver::makeButton)(&game::makeButton));
		receiver.setIsUnitSelected(false);
		receiver.menuDone = false;

		// Create obstacles
		obstacles = new std::vector<Obstacle*>();
		obstacles->push_back(new Obstacle(type::PYRAMID, context.device));
		obstacles->push_back(new Obstacle(type::CAT, context.device));
		obstacles->push_back(new Obstacle(type::SPIDER, context.device));
		receiver.setObstacles(obstacles);

		// Get fields
		getFieldsList();

		// Pass them to the receiver
		receiver.setFields(fields);

		// specify our custom event receiver in the device	
		device->setEventReceiver(&receiver);
		
		while (device->run() && driver)
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, SColor(0,200,200,200));
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();
		}

	return 0;
}

void game::getFieldsList() {

	ifstream infile("../assets/fields.txt");
	float x, y, z;
	
	while(infile >> x >> y >> z) {
		fields->push_back(vector3df(x, y, z));
	}

}

void game::startGame() {


	networkUtilities->initializeWS("192.168.1.133");
	networkUtilities->setGameName("PSI Team 3");
	networkUtilities->registerOnTheServer();
	//networkUtilities->playGame();

}

//void game::connect(bool asPlayer1, char* ipAddress){
//	int *ret;
//	this->asPlayer1 = asPlayer1;
//	this->ipAddress = ipAddress;
//	guienv->clear();
//	smgr->clear();
//	pthread_create(&connectThread, NULL, startGame, this);
//	pthread_join(connectThread, (void**)&ret);
//	init_map(device, obstacles);
//	init_ingame_menu();
//	pthread_create(&thread, NULL, updateGameState, this);
//}
/**	
  * starts the game from the perspective of player1/player2
  */
void game::startGame(bool asPlayer1, char* ipAddress) {
	//smgr->clear();
	//guienv->clear();
	//smgr->addCameraSceneNode(0, vector3df(0,8,-8), vector3df(0,0,0));

	gameState->setPlayer1Turn(true);
	if (asPlayer1) {
		networkUtilities->hostGame(portNumber);

		// hosting player is always player 1
		localPlayer->setPlayer1(true);
		opposingPlayer->setPlayer1(false);

		localPlayer->initUnits();
		opposingPlayer->initUnits();
		localPlayer->setActionsLeft();
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
	// in the game state both player's units will be contained --> allocate memory for all units
	
	bool pl1turn = gameState->getPlayer1Turn();
	gameState = new GameStateDTO(localPlayer->getUnits()->size() + opposingPlayer->getUnits()->size());
	gameState->setGiveUp(giveUp);
	if (localPlayer->getPlayer1()) gameState->setPlayer1Turn(false);
	if (!localPlayer->getPlayer1()) gameState->setPlayer1Turn(true);
	gameState->setVictory(checkVictory());

	BaseUnitDTO* units = new BaseUnitDTO[localPlayer->getUnits()->size() + opposingPlayer->getUnits()->size()];
	int i = 0;


	m->setTurnText("It is your opponents turn");
	m->setWaitText(true);
	//m->setTurnTextColor(SColor(0, 255, 0, 0));
	
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

	std::cout<<"FLAGS IN GAMESTATE \n";
	std::cout<<"Give up: ";
	std::cout<<gameState->getGiveUp();
	std::cout<<"\n";
	std::cout<<"Player 1 Turn: ";
	std::cout<<gameState->getPlayer1Turn();
	std::cout<<"\n";
	std::cout<<"Victory: ";
	std::cout<<gameState->getVictory();
	std::cout<<"\n";

	try {
		networkUtilities->setBuffer(buffer);
		networkUtilities->sendData();
		if (! gameState->getGiveUp()) {
			pthread_create(&thread, NULL, updateGameState, this);
		} else {
			endOfGame = true;
		}
	}
	catch(NonRealtimeNetworkingException e) {
		device->getGUIEnvironment()->addMessageBox(L"Oops an Error", L"Something went wrong, probably connection lost", true, EMBF_OK);
		endOfGame = true;
	}

	try {
		if (gameState->getVictory()) networkUtilities->closeConnection();
	} catch (NonRealtimeNetworkingException e) {
		// just catch that shit already
	}
}

void * game::updateGameState(void * g){
	
	// cast parameter to game
	game* gm = (game*) g;

	// create a GameStateDTO object and fill in data we received by deserializing it
	try {
		gm->networkUtilities->receiveData();
	} catch (NonRealtimeNetworkingException e ) {
		// hooope this works
	}

	if (gm->endOfGame) return false;

	gm->gameState->deserialize(gm->networkUtilities->getBuffer());

	if ((gm->gameState->getPlayer1Turn() && gm->localPlayer->getPlayer1()) || (!gm->gameState->getPlayer1Turn() && !gm->localPlayer->getPlayer1())) {
		gm->m->setTurnText("It is your turn");
		gm->m->setWaitText(false);
	}

	if (gm->gameState->getVictory()) gm->networkUtilities->closeConnection();

	bool unitUpdated;
	// update gamestate by updating all attributes in both players
	for (int i=0; i < gm->gameState->unitLength; i++) { // calc length of array
		BaseUnitDTO tmp = gm->gameState->getUnits()[i];
		unitUpdated = false;

		// find referring unit
		for(std::vector<BaseUnit*>::iterator it = gm->localPlayer->getUnits()->begin(); it != gm->localPlayer->getUnits()->end(); ++it) {
			if (unitUpdated) break;
			if (tmp.getId() == (*it)->id) {
				if((*it)->player1 != gm->localPlayer->getPlayer1() && ! gm->endOfGame)
					throw new IllegalStateException("Unit is not assigned correctly!");

				// Update position
				vector3df newPosition = vector3df(tmp.getX(), tmp.getY(), tmp.getZ());
				(*it)->setPosition(newPosition);

				// Update other attributes
				(*it)->health = tmp.getHealth();
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
				if((*it)->player1 != gm->opposingPlayer->getPlayer1() && ! gm->endOfGame)
					throw new IllegalStateException("Unit is not assigned correctly!");				

				// Update position
				vector3df newPosition = vector3df(tmp.getX(), tmp.getY(), tmp.getZ());
				(*it)->setPosition(newPosition);

				// Update other attributes
				(*it)->health = tmp.getHealth();
				(*it)->updateHealthBar();
				(*it)->setHasMoved(false);
				(*it)->setHasShot(false);

				unitUpdated = true;
			}
		}

		/*if (! unitUpdated) 
			throw new IllegalStateException("Unit is not assigned to a player.");	*/	
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

if(gm->localPlayer->getPlayer1() && gm->gameState->getPlayer1Turn()){
		gm->localPlayer->resetActionsLeft();
		gm->m->setActionText("Actions left = ");
		gm->m->setActionImage(4);
		gm->m->setTurnText("It is your turn");
	} else if(!gm->localPlayer->getPlayer1() && !gm->gameState->getPlayer1Turn()){
		gm->localPlayer->resetActionsLeft();
		gm->m->setActionText("Actions left = ");
		gm->m->setActionImage(4);
		gm->m->setTurnText("It is your turn");
	}
	pthread_exit(NULL);
}

void game::init_map(IrrlichtDevice* device_map, std::vector<Obstacle*>* obstacles)
{
	//make a new terrain	
	mapterrain map = mapterrain(device_map, smgr, obstacles, localPlayer->getPlayer1());
	
}

void game::init_ingame_menu()
{
	m = new menu(device, driver, smgr, guienv, horizontal, vertical);

	if (localPlayer->getPlayer1() == false)
	{
		//m->setPlayerText("You are Player 2");
		m->setTurnText("It is your opponents turn");
	}
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
	//delete localPlayer;
	//delete opposingPlayer;
	guienv->clear();
	smgr->clear();

	// reset all the relevant properties
	localPlayer = new Player(device);
	opposingPlayer = new Player(device);
	try{
		//networkUtilities->closeConnection();
		networkUtilities = new NonRealtimeNetworkingUtilities();
	} catch (NonRealtimeNetworkingException e) {
		// lalaal
	}
	
	//endOfGame = false;
	gameState = new GameStateDTO(16);

	// re-run game
	smgr->addCameraSceneNode(0, vector3df(0,8,-8), vector3df(0,0,0));
	this->run();
}
