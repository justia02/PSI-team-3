#include "menu.h"

/**
  * Implementation of menu
  */
using namespace irrlicht_nonrealtimenetworking;

menu::menu(IrrlichtDevice* device, IVideoDriver* driver, ISceneManager* smgr, IGUIEnvironment* guienv) 
{
	this->device = device;
	this->driver = driver;
	this->smgr = smgr;
	this->guienv = guienv;

	// initalizes graphical menu
	init();
}

menu::~menu(void)
{
}

void menu::run(game* g) {

	// setup context
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.game_ = g;

	// runs the console menu
	init_console(context);
}

/**
 * inializes the graphical menu
 */
void menu::init(void)
{
	// TODO: add text?!
	guienv->addStaticText(L"Welcome! Please select an option.",
        rect<s32>(235,50,360,50+10), true);
	// add gui elements
	guienv->addButton(rect<s32>(240,100,350,150 + 10), 0, GUI_ID_JOIN_GAME, L"Join Game", L"Joins a game");
	guienv->addButton(rect<s32>(240,200,350,250 + 10), 0, GUI_ID_HOST_GAME, L"Host Game", L"Hosts a game");
	guienv->addButton(rect<s32>(240,300,350,350 + 10), 0, GUI_ID_JOIN_WSDL, L"Quick Match", L"Starts a quick match, opponent will be automatically assigned.");
	guienv->addButton(rect<s32>(240,400,350,450 + 10), 0, GUI_ID_START_GAME, L"Start Game", L"Start game without network for test");
}

/**
 * initializes an alternative console menu, mainly for testing purpose
 */
void menu::init_console(SAppContext context)
{
		int portNo = 6;
		int option;
		char* ipAddress = new char[15];

		while (true) {
		std::cout << "welcome! would you like to host(1) a game or join(2) a game, connect(3) to the Web Service or just start(4)?";
		std::cin >> option;

		if (option == 1) {
			try {
				context.game_->startGame(true);
			}
			catch(NonRealtimeNetworkingException e) {
				std::cout << "NonRealtimeNetworkingException: " << e.what() << std::endl;
			}
			break;
		} else if (option == 2) {
			std::cout << "Please enter the IP address of your opponent" << std::endl;
			std::cin >> ipAddress;
			try {
				context.game_->startGame(false, ipAddress);
			}
			catch(NonRealtimeNetworkingException e) {
				std::cout << "NonRealtimeNetworkingException: " << e.what() << std::endl;
			}
			break;
		} else if (option == 3) {
			try {
				context.game_->startGame();
			}
			catch(NonRealtimeNetworkingException e) {
				std::cout << "NonRealtimeNetworkingException: " << e.what() << std::endl;
			}
			break;
		} else if (option == 4) {
			context.game_->localPlayer->setPlayer1(true);
			context.game_->opposingPlayer->setPlayer1(false);
			context.game_->localPlayer->initUnits();
			context.game_->opposingPlayer->initUnits();
			break;
		}
	}
}